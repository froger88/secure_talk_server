/* 
 * File:   SecureCounter.cpp
 * Author: mazabinski
 * 
 * Created on 26 sierpień 2013, 00:21
 */

#include "SecureCounter.h"

SecureCounter::SecureCounter(int count) 
{
	this->count = count;
}

SecureCounter::SecureCounter(const SecureCounter& orig) { }

SecureCounter::~SecureCounter() { }

