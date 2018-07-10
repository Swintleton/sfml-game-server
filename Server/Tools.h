#pragma once
#include <time.h>
#include <iostream>

const std::string currentTime() {
	struct tm newtime;
	__time64_t long_time;
	char timebuf[26];
	errno_t err;

	_time64(&long_time);							//Get current time as 8-byte integer
	err = _localtime64_s(&newtime, &long_time);		//Convert to local time

	if (err) {
		std::cout << "Error: _localtime64_s" << std::endl;
		exit(1);
	}

	err = asctime_s(timebuf, 26, &newtime);			// Convert to an ASCII
	if (err) {
		std::cout << "Error: asctime_s" << std::endl;
		exit(1);
	}

	strftime(timebuf, sizeof(timebuf), "%X", &newtime);

	return timebuf;
}

const std::string CurrentTime(void) {
	return "[" + currentTime() + "] ";
}