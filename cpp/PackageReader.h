/* 
 * File:   PackageReader.h
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

#ifndef PACKAGEREADER_H
#define	PACKAGEREADER_H

#include <memory>
#include <map>
#include "Package.h"
#include "Logger.h"

using namespace std;

namespace SecureTalkServer {

class PackageReader {
public:
	PackageReader();

	virtual ~PackageReader();
	shared_ptr<pair<const char*, size_t> > get_next();

	int start_read(pair<const char*, size_t>& pkg);
	int read(pair<const char*, size_t>& pkg);
	
	inline shared_ptr<multimap<const char*, short> > get_pkg_multimap()
	{
		return pkg_map;
	}

	inline PackageType get_pkg_type()
	{
		return pkg_type;
	}

private:
	/* const ptr on package to parse*/
	PackageType pkg_type;
	int32_t flags;
	int32_t pkg_len;
	int32_t pkg_iterator;
	bool finished;
	int32_t bytes_now;

	shared_ptr<multimap<const char*, short> > pkg_map;
	char* pkg_data;

	PackageReader(const PackageReader& orig);
};
}

#endif	/* PACKAGEREADER_H */

