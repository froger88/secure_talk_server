/* 
 * File:   SecureCounter.h
 * Author: mazabinski
 *
 * Created on 26 sierpień 2013, 00:21
 */

#ifndef SECURECOUNTER_H
#define	SECURECOUNTER_H

#include <mutex>

using namespace std;

class SecureCounter {
public:
	SecureCounter(int count=0);
	SecureCounter(const SecureCounter& orig);
	virtual ~SecureCounter();
	
	inline void inc(int count=1) {
		mut.lock();
		this->count += count;
		mut.unlock();
	}
	
	inline void dec(int count=1) {
		mut.lock();
		this->count -= count;
		mut.unlock();
	}
	
	inline int64_t get() {
		return count;
	}
private:
	int64_t count;
	mutex mut;
};

#endif	/* SECURECOUNTER_H */

