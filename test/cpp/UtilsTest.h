/* 
 * File:   UtilsTest.h
 * Author: mazabinski
 *
 * Created on 4 wrzesień 2013, 09:46
 */

#ifndef UTILSTEST_H
#define	UTILSTEST_H

#include "../../cpp/utils.h"

#include <iostream>
using namespace std;

TEST(TestReadWrite64)
{
	char z[8];

	for (int64_t i = -9223372036854775800L; i < 9223372036854775800L; i += 92233720368547758L) {
		write_int64(z, i);
		int64_t res = read_int64(z);
		CHECK_EQUAL(i, res);
	}
	cout << "." << flush;
}

TEST(TestReadWrite32)
{
	char z[8];

	int32_t res;

	write_int32(z, 2147483647);
	CHECK_EQUAL(2147483647, read_int32(z));

	write_int32(z, -2147483648);
	CHECK_EQUAL(-2147483648, read_int32(z));

	for (int32_t i = -2147329666; i < 2147329666; i += 1000) {
		write_int32(z, i);
		res = read_int32(z);
		CHECK_EQUAL(i, res);
	}
	cout << "." << flush;
}

TEST(TestReadWrite16)
{
	char z[8];

	for (int16_t i = -32000; i < 32000; i += 100) {
		write_int64(z, i);
		int16_t res = read_int64(z);
		CHECK_EQUAL(i, res);
	}
	cout << "." << flush;
}

#endif	/* UTILSTEST_H */

