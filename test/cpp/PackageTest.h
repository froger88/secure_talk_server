/* 
 * File:   PackageTest.h
 * Author: mazabinski
 *
 * Created on 2 wrzesień 2013, 23:06
 */

#ifndef PACKAGETEST_H
#define	PACKAGETEST_H

#include <unittest++/UnitTest++.h>
#include <memory>
#include <map>
#include "../../cpp/Logger.h"
#include "../../cpp/Package.h"


using namespace UnitTest;

TEST(PackageTest1)
{
	SecureTalkServer::Package x;
	
	x.create(SecureTalkServer::PT_CREATE_SESSION);
	x.add("test_value", sizeof("test_value"));
	x.finish();
	
	shared_ptr <multimap<const char*, size_t> > sptr_pkg = x.get();
	
	CHECK(true);
}

#endif	/* PACKAGETEST_H */

