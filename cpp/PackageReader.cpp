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
		finished = false;
		parsed = false;
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

		// read package headers
		flags = read_int32(pkg.first);
		pkg_len = read_int32(pkg.first + 4);
		pkg_type_len = read_int16(pkg.first + 8);

		// convert from network to host
		flags = ntohl(flags);
		pkg_len = ntohl(pkg_len);
		pkg_type_len = ntohs(pkg_type_len);

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

	void PackageReader::parse()
	{
		size_t pos = 0;
		// parse pkg_name:
		if (pkg_len >= pkg_type_len) {
			string type;
			type.assign(pkg_data + 10, pkg_type_len);
			
			pkg_type = package_str2type(type);
			pos = 10+pkg_type_len;
		}

		// add as much as you can
		while(pos < pkg_len)
		{
			int16_t len = read_int16(pkg_data+pos);
			pos += 2;
			len = ntohs(len);
			if(pkg_len+pos+len <= pkg_len)
				pkg_map->insert(pair<const char*, short>(pkg_data+pos, len));
			else
				break;
			
			pos += len;
		}
		
		parsed = true;
	}

	multimap<const char*, short>* PackageReader::get_pkg_multimap()
	{
		// finished and pkg_map created and data has been parsed into it
		if (finished && pkg_map && parsed)
			return pkg_map;

		// pkg map doesn't exists - create new one, then return with empty
		if (!pkg_map)
			pkg_map = new multimap<const char*, short>;


		if (!finished && !parsed)
			return pkg_map;

		if (finished && !parsed)
			parse();

		return pkg_map;
	}

	PackageReader::~PackageReader()
	{
		if (!pkg_data && !pkg_data[0])
			delete [] pkg_data;
		pkg_data = NULL;
		
		if (!pkg_map)
			delete pkg_map;
		pkg_map = NULL;
	}
}
