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

extern SecureLogger* sLog;

namespace SecureTalkServer {
	using namespace std;

	PackageType package_type2str(string& str)
	{
		if (str == "create_session") {
			return PT_CREATE_SESSION;
		} else if (str == "destroy_session") {
			return PT_DESTROY_SESSION;
		} else if (str == "join_session") {
			return PT_JOIN_SESSION;
		} else if (str == "invite") {
			return PT_INVITE_CLIENT;
		} else if (str == "quit") {
			return PT_QUIT;
		} else if (str == "ping") {
			return PT_PING;
		} else if (str == "pong") {
			return PT_PONG;
		}

		return PT_UNKNOWN;
	}

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
		clean();
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
			case PT_PING:
				pkg_type_str = "ping";
				break;
			case PT_PONG:
				pkg_type_str = "pong";
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
		if (data && data[0]) {
			delete [] data;
			data = NULL;
		}
		data_len = 0;
		data_map.clear();

		if (data_pair)
			delete data_pair;
		data_pair = NULL;
	}

	const pair<char*, size_t>* Package::finish()
	{

		data_len = prepare_pkg();
		data = new char [data_len];

		write_int32(data, htonl(0));
		write_int32(data + 4, htonl(data_len));
		write_int16(data + 8, htons(pkg_type_str.size()));
		write_str(data + 10, pkg_type_str.c_str(), pkg_type_str.size());

		int32_t p_data_now = 4 + 4 + 2 + pkg_type_str.size();

		for (auto i = data_map.begin(); i != data_map.end(); ++i) {
			write_int16(data + p_data_now, htons(i->second));
			p_data_now += 2;
			write_str(data + p_data_now, i->first, i->second);
		}

		data_pair = new pair<char*, size_t>(data, data_len);
		return data_pair;
	}

	const pair<char*, size_t>* Package::get()
	{
		sLog->log(L_INFO, "Package::get(): data_len = %d", data_len);
		return data_pair;
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

		for (auto i = data_map.begin(); i != data_map.end(); ++i) {
			l += 2 + i->second;
		}

		return l;
	}
}
