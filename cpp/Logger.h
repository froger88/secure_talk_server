/* 
 * File:   Logger.h
 * Author: mazabinski
 *
 * Created on 30 sierpień 2013, 00:22
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

#ifndef LOGGER_H
#define	LOGGER_H


/*
 INFO:
 * this is basic version of logger.
 * logging to file and file rotations are not supported for now
 * it is good idea, to have one, thread-secured instance of logger
 * but... it is not implemented for now
 * time format is not supported for now 
 * __FILE and __LINE is not supported for now too ..
 * max buffer size is const 1024bytes -1 (for NULL char)
 */

#include <cstdio>
#include <cstdarg>
#include <sys/time.h>
#include <mutex>

#include "Config.h"
using namespace std;

typedef enum {
	L_DEBUG = 1,
	L_NOTICE = 2,
	L_INFO = 4,
	L_SUCCESS = 8,
	L_WARNING = 16,
	L_ERROR = 32,
	L_FATAL = 64
} LogPriority;

class Logger {
public:
	Logger();
	virtual ~Logger();
	virtual void log(LogPriority prio, __const char* __restrict fmt, ...);

private:
	char* get_time_str();
	Logger(const Logger& orig);

};

class SecureLogger : public Logger
{
	
public:
	SecureLogger() {}
	virtual ~SecureLogger() {}
	
	virtual void log(LogPriority prio, __const char* __restrict fmt, ...)
	{
		try{
			mut.lock();
			va_list args;
			va_start(args, fmt);
			Logger::log(prio, fmt, args);
			va_end(args);
			mut.unlock();
		} catch (...) {
			mut.unlock();
			throw;
		}
	}
	
private:
	mutex mut;
};

#endif	/* LOGGER_H */

