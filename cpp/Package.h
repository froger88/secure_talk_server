/* 
 * File:   Package.h
 * Author: mazabinski
 *
 * Created on 27 sierpień 2013, 23:34
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

#ifndef PACKAGE_H
#define	PACKAGE_H

#include <string>
#include <memory>
#include <map>
#include <exception>
#include <vector>
#include <string.h>
#include <netinet/in.h>

#include "Logger.h"

namespace SecureTalkServer {
using namespace std;

typedef enum {
	PT_CREATE_SESSION,
	PT_JOIN_SESSION,
	PT_INVITE_CLIENT,
	PT_DESTROY_SESSION,
	PT_QUIT,
	PT_UNKNOWN
} PackageType;

class Package {
public:
	Package();
	Package(const Package& orig);
	virtual ~Package();

	void create(PackageType ptype);
	void add(const char* data, size_t len);
	void add(pair<const char*, size_t> data);
	void clean();

	shared_ptr<multimap<const char*, size_t > > finish();
	shared_ptr<multimap<const char*, size_t > > get();

private:
	char* data;
	long data_len;
	long data_size;
	PackageType pkg_type;
	multimap<const char*, size_t> data_map;
	string pkg_type_str;

	long prepare_pkg();
};
}

#endif	/* PACKAGE_H */

