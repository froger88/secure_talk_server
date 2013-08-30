/* 
 * File:   Logger.cpp
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

#include <string.h>
#include <stdlib.h>

#include "Logger.h"

using namespace std;

Logger::Logger() { }

Logger::Logger(const Logger& orig) { }

Logger::~Logger() { }

char* Logger::get_time_str()
{
	time_t t;
	struct tm tm;
	time(&t);

	char* date_time = new char[30];
	memset(date_time, 0, 30);

	localtime_r(&t, &tm);
	strftime(date_time, 30, "%Y-%m-%d %T", &tm);

	return date_time;
}

void Logger::log(LogPriority prior, const char *__restrict fmt, ...)
{
	char buffer[1024];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 1023, fmt, args);

	char* date_time = get_time_str();

	switch (prior) {
		case L_DEBUG:
			if (Config::log_prior & L_DEBUG)
				printf("[DEBUG]   [%s] %s\n", date_time, buffer);
			break;
		case L_NOTICE:
			if (Config::log_prior & L_NOTICE)
				printf("[NOTICE]  [%s] %s\n", date_time, buffer);
			break;
		case L_INFO:
			if (Config::log_prior & L_INFO)
				printf("[INFO]    [%s] %s\n", date_time, buffer);
			break;
		case L_SUCCESS:
			if (Config::log_prior & L_SUCCESS)
				printf("[SUCCESS] [%s] %s\n", date_time, buffer);
			break;
		case L_WARNING:
			if (Config::log_prior & L_WARNING)
				fprintf(stderr, "[WARNING] [%s] %s\n", date_time, buffer);
		case L_ERROR:
			if (Config::log_prior & L_ERROR)
				fprintf(stderr, "[ERROR]   [%s] %s\n", date_time, buffer);
			break;
		case L_FATAL:
			if (Config::log_prior & L_FATAL)
				fprintf(stderr, "[FATAL]   [%s] %s\n", date_time, buffer);
			break;
	}
	va_end(args);

	delete [] date_time;
}

SecureLogger::SecureLogger() { }

SecureLogger::~SecureLogger() { }

SecureLogger::SecureLogger(const SecureLogger&) { }

void SecureLogger::log(LogPriority prio, __const char* __restrict fmt, ...)
{
	try {
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