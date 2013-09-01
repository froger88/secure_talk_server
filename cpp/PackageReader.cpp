/* 
 * File:   PackageReader.cpp
 * Author: mazabinski
 * 
 * Created on 30 sierpień 2013, 19:36
 */

/*
					+IMMMM .~MMZ.
				 .MM NMMMMM  .MMMM
				MMM. MMMMMMZ   MMMM.
			  .MMM, .MMMMMMM  ..MMMM
			  .MMM. ZMMMMMMM.   MMMM.
			  .MMM  =MMMMMMM.   MMMM.
			  .MMM . MMMMMMM.  MMMM
				MMM: MMMMMMM .ZMMM
				  MMM MMMMMM.~MO
					  ~MMMN..   ...M.
						.?M8 .. +.NI
					   . .....  MNM D
						: D..Z...MO.?.
						  NM . M..  .~
						 .~I...     .,
						  .M.       M.
						 .M.        :
						.M        .MM
					   .7           M.
					   M            MO
					  M.            .8       .=MMMMMMM .
					 M.             .I    MM$          ,M
					.                M MM .             .M
					M                 N              .   M
				   .:                                M   ,.
													 +   .
					,                                Z  .M
				   .M                               ..  ,
					M                  .            M.  M..  =+, .
					 M        ?        D           :+   7  ..M$ ..
					 .Z        M.       ,         DMM.     .. =M.
					  ,M      .8,       M        M.. .MMM,...
			   .. N  M$ MI.    MM.      :.   ..M$
			 .$...  =  MM . D,,MM7       MMMM,
				   Z...    MN   MM.      MMOMMM7..
					  .D.,8      M.      :..:NM$
								 MM.      .MM~.,
								  MM.  ~=7DMMM$.

		  S   E   A    L       O   F      T   H   E       D   A   Y
 */

#include "PackageReader.h"

extern SecureLogger* sLog;

namespace SecureTalkServer {

	PackageReader::PackageReader()
	{
		pkg_iterator = 0;
	}

	PackageReader::PackageReader(const PackageReader& orig) { }

	/*
	 * return: 0 - if complete pkg was read
	 * 
	 * value less than 0 when incomplete value was read (the number of bytes
	 * lees than 0 is the number of lacking bytes)
	 * 
	 * value more than 0, if pakage has been read postive but there is not complete buffer
	 * (there are additional bytes in it)
	 */

	int PackageReader::start_read(pair<const char*, size_t>& pkg)
	{
		sLog->log(L_DEBUG, "PackageReader()");

		// check package buffer length
		if (pkg.second < 10) {
			return 10 - pkg.second;
		}
		/* read headers ([4B][4B][2B][n])
		 * first 4B for future flags
		 * second 4B for package length,
		 * 2B for package type length
		 * package length
		 */

		int32_t ptype;
		short ptype_len;
		sscanf(pkg.first, "%ld%ld%hd", &flags, &ptype, &pkg_len, &ptype_len);
		// convert from network to host
		flags = ntohl(flags);
		ptype = ntohl(ptype);
		ptype_len = ntohs(ptype_len);
		
		// create buffers and copy data into it
		pkg_data = new char [pkg_len];

		// copy as much as is in this package
		if (pkg_len > pkg.second) {
			// copy pkg.second size of data
			memcpy(pkg_data, pkg.first, pkg.second);
		}

		// copy whole package
		if (pkg_len <= pkg.second) {
			// copy pkg_len 
			memcpy(pkg_data, pkg.first, pkg_len);
		}
		
		return pkg.second - pkg_len;
	}

	int PackageReader::read(pair<const char*, size_t>& pkg) { }

	PackageReader::~PackageReader()
	{
		if (!pkg_data)
			delete pkg_data;
	}
}
