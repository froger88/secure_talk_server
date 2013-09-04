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
		finished = false;
		bytes_now = 0;
	}

	PackageReader::PackageReader(const PackageReader& orig) { }

	/*
	 * return: 0 - if complete pkg was read
	 * 
	 * value less than 0 when incomplete value was read (the number of bytes
	 * lees than 0 is the number of lacking bytes)
	 * 
	 * value more than 0, if pakage has been read postive but there there are 
	 * additional bytes in buffer
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
		// read package headers
		flags = read_int32(pkg.first);
		pkg_len = read_int32(pkg.first+4);
		ptype_len = read_int16(pkg.first+8);
		
		// convert from network to host
		flags = ntohl(flags);
		pkg_len = ntohl(pkg_len);
		ptype_len = ntohs(ptype_len);

		// create buffers and copy data into it
		pkg_data = new char [pkg_len];

		// copy as much as is in this package
		if (pkg_len > pkg.second) {
			// copy pkg.second size of data
			memcpy(pkg_data, pkg.first, pkg.second);
			bytes_now = pkg.second;
		}

		// copy whole package
		if (pkg_len <= pkg.second) {
			// copy pkg_len 
			memcpy(pkg_data, pkg.first, pkg_len);
			bytes_now = pkg_len;
			finished = true;
		}

		return pkg.second - pkg_len;
	}

	/*
	 * return: 0 - if complete pkg was read
	 * 
	 * value less than 0 when incomplete value was read (the number of bytes
	 * lees than 0 is the number of lacking bytes)
	 * 
	 * value more than 0, if pakage has been read postive but there there are 
	 * additional bytes in buffer
	 */
	int PackageReader::read(pair<const char*, size_t>& pkg)
	{

		//error, too much data
		if (bytes_now + pkg.second > pkg_len) {

			// complete pkg anyway
			memcpy(pkg_data + bytes_now, pkg.first, pkg_len - bytes_now);
			bytes_now = pkg_len;

			sLog->log(L_ERROR,
			  "PackageReader::read: too much data, pkg_len = %d, bytes_now = %d, over = %d",
			  pkg_len, bytes_now, bytes_now + pkg.second - pkg_len
			  );

			return bytes_now + pkg.second - pkg_len;
		}

		// perfect, pkg is now complete
		if (bytes_now + pkg.second == pkg_len) {
			sLog->log(L_DEBUG, "PackageReader::read: PKG complete, pkg_len = %d", pkg_len);
			memcpy(pkg_data + bytes_now, pkg.first, pkg.second);
			bytes_now = pkg_len;
			return 0;
		}

		// still package is not complete
		memcpy(pkg_data + bytes_now, pkg.first, pkg.second);
		bytes_now += pkg.second;
		
		sLog->log(L_DEBUG,
		  "PackageReader::read: PKG still not complete, pkg_len = %d, bytes_now = %d, bytes still waiting for = %d",
		  pkg_len, bytes_now, pkg_len - bytes_now);

		return pkg_len - bytes_now;
	}

	PackageReader::~PackageReader()
	{
		if (!pkg_data)
			delete pkg_data;
	}
}
