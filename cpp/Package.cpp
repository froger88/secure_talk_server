/* 
 * File:   Package.cpp
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

#include "Package.h"

namespace SecureTalkServer {
	using namespace std;

	Package::Package()
	{
		data = NULL;
	}

	Package::Package(const Package& orig)
	{
		/*
		 * copy data
		 */
		memcpy(orig.data, this->data, data_len);
	}

	Package::~Package()
	{
		/*
		 * clean up
		 */
		delete data;
		data = NULL;
	}

	void Package::create(PackageType ptype)
	{
		pkg_type = ptype;
		
		switch (ptype) {
			case PT_CREATE_SESSION:
				pkg_type_str = "create_session";
				break;
			case PT_DESTROY_SESSION:
				pkg_type_str = "destroy_session";
				break;
			case PT_INVITE_CLIENT:
				pkg_type_str = "invite";
				break;
			case PT_JOIN_SESSION:
				pkg_type_str = "join_session";
				break;
			case PT_QUIT:
				pkg_type_str = "quit";
				break;
			default:
				pkg_type = PT_UNKNOWN;
		}
	}
	
	void Package::add(const char* data, size_t len)
	{
		data_map.insert(pair<const char*, size_t> (data, len));
	}

	void Package::clean()
	{
		if (data) {
			delete data;
			data = NULL;
		}
		data_len = 0;
		data_map.clear();
	}
	
	shared_ptr<multimap<const char*, size_t> > Package::finish()
	{
		shared_ptr<multimap<const char*, size_t> > _return;
				
		data_len = prepare_pkg();
		data = new char [data_len];
		
		sprintf(data, "%d%d%hd%s",
		  htonl(0), htonl(data_len),
		  htons((unsigned short)pkg_type_str.size()), pkg_type_str.c_str());
		
		int32_t p_data_now = 4 + 4 + 2 + pkg_type_str.size();
		
		for(auto i=data_map.begin(); i != data_map.end(); ++i) {
			sprintf(data+p_data_now, "%hd%s",
			  htons((short)i->second), i->first
			  );
		}
		
		return _return;
	}
	
	shared_ptr<multimap<const char*, size_t> > Package::get()
	{
		shared_ptr <multimap<const char*, size_t> > _return;
		_return->insert(pair<const char*, size_t>(data, data_len));
		
		return _return;
	}
	
	long Package::prepare_pkg()
	{
		/*
		 * PACKAGE STRUCT:
		 * 
		 * first 4B is for future flags
		 * second 4B is for package_size (including first and second 4B sections) 
		 * 2B for pkg_type length
		 * pkg_type
		 * 
		 * and for any next data:
		 * 2B - size of entry
		 * entry
		 * 
		 * [4B][4B][2B][pkg_type][2B][DATA_1]...[2B][DATA_N]
		 * 
		 */
		long l = 8;
		l += 2;
		l += pkg_type_str.size();
		
		for(auto i =data_map.begin(); i != data_map.end(); ++i) {
			l += 2 + i->second;
		}
		
		return l;
	}

}
