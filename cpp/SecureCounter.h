/* 
 * File:   SecureCounter.h
 * Author: mazabinski
 *
 * Created on 26 sierpień 2013, 00:21
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

