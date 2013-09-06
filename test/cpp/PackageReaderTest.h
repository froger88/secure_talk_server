/* 
 * File:   PackageReader.h
 * Author: mazabinski
 *
 * Created on 2 wrzesień 2013, 23:35
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

#ifndef PACKAGEREADERTEST_H
#define	PACKAGEREADERTEST_H

#include <unittest++/UnitTest++.h>
#include <memory>
#include <map>
#include "../../cpp/Logger.h"
#include "../../cpp/Package.h"
#include "../../cpp/PackageReader.h"


using namespace UnitTest;
using namespace std;

TEST(PackageReaderTest1)
{
	SecureTalkServer::Package x;
	const pair<char*, size_t>* pkg_ptr;
	/*
	 * pkg should contain:
	 * [4b][4b][2b][create_session][2b][test_value]
	 * = 10 + 14 + 2 + 10
	 * = 36B
	 */
	x.create(SecureTalkServer::PT_CREATE_SESSION);
	x.add("test_value", 10);
	pkg_ptr = x.finish();
	
	// now start PackageReaderTest1
	const char* pkg_data = pkg_ptr->first;
	size_t pkg_len = pkg_ptr->second;
	
	pair<const char*, size_t> pkg_pair (pkg_data, pkg_len);
	
	CHECK_EQUAL(36, pkg_pair.second);
	SecureTalkServer::PackageReader pkg_reader;
	CHECK_EQUAL(0, pkg_reader.start_read(pkg_pair));
}

TEST(PackageReaderTest2)
{
		SecureTalkServer::Package x;
	const pair<char*, size_t>* pkg_ptr;
	/*
	 * pkg should contain:
	 * [4b][4b][2b][create_session][2b][test_value]
	 * = 10 + 14 + 2 + 10
	 * = 36B
	 */
	x.create(SecureTalkServer::PT_CREATE_SESSION);
	x.add("test_value", 10);
	pkg_ptr = x.finish();
	
	// now start PackageReaderTest1
	const char* pkg_data = pkg_ptr->first;
	size_t pkg_len = pkg_ptr->second;
	
	pair<const char*, size_t> pkg_pair (pkg_data, pkg_len);
	
	CHECK_EQUAL(36, pkg_pair.second);
	SecureTalkServer::PackageReader pkg_reader;
	CHECK_EQUAL(0, pkg_reader.start_read(pkg_pair));
	
	CHECK_EQUAL(SecureTalkServer::PT_CREATE_SESSION,  pkg_reader.get_pkg_type());
}

#endif	/* PACKAGEREADER_H */

