/* 
 * File:   SecureCounterTest.h
 * Author: mazabinski
 *
 * Created on 2 wrzesień 2013, 23:36
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

#ifndef SECURECOUNTERTEST_H
#define	SECURECOUNTERTEST_H

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <unittest++/UnitTest++.h>

using namespace std;
#include "../../cpp/SecureCounter.h"

/*
 * inc(value)
 */
TEST(SecureCounterINCTest)
{
	SecureCounter cnt;
	CHECK_EQUAL(0, cnt.get());
	cnt.inc(10);
	CHECK_EQUAL(10, cnt.get());
	cout << "." << flush;
}

/*
 * dec(value)
 */
TEST(SecureCounterDECTest)
{
	SecureCounter cnt;
	CHECK_EQUAL(0, cnt.get());
	cnt.dec(10);
	CHECK_EQUAL(-10, cnt.get());
	cout << "." << flush;
}

/*
 * constructor with value. then inc, then dec
 */
TEST(SecureCounterBasicTest)
{
	SecureCounter cnt(100);
	CHECK_EQUAL(100, cnt.get());
	cnt.inc(10);
	CHECK_EQUAL(110, cnt.get());
	cnt.dec(110);
	CHECK_EQUAL(0, cnt.get());
	cout << "." << flush;
}

void thread_test_secure_counter(SecureCounter* cnt, bool increase, bool* finish)
{
	for (int64_t i = 0; i < 1000000; ++i) {
		if (increase)
			cnt->inc();
		else
			cnt->dec();
	}
	*finish = true;
}

/*
 * test SecureCounter in threaded environment
 */
TEST(SecureCounterThreadedTest)
{
	SecureCounter cnt;
	bool finish_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	CHECK_EQUAL(0, cnt.get());

	for (int i = 0; i < 10; i += 2) {
		thread t(thread_test_secure_counter, &cnt, true, &finish_array[i]);
		t.detach();
		thread y(thread_test_secure_counter, &cnt, false, &finish_array[i + 1]);
		y.detach();
	}


	// wait till threads finish
	for (int i = 0; i < 10; ++i) {
		if (finish_array[i] == false) {
			i = 0;
			sleep(1);
			continue;
		}
	}

	CHECK_EQUAL(0, cnt.get());
	cout << "." << flush;
}

#endif	/* SECURECOUNTERTEST_H */

