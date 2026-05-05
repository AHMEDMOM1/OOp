#pragma once
#include <string>
#include <ctime>

class Date {
public:
	
	static tm getDate(time_t date = time(nullptr)) {
		tm ltm{};
		localtime_s(&ltm, &date);
		return ltm;
	}

	
	static int calculateDuration(time_t from, time_t to = time(nullptr)) {
		return (int)difftime(to, from);
	}

	// not understanded!!
	static std::string getDurationFormatted(time_t from, time_t to = time(nullptr)) {
		int totalSeconds = calculateDuration(from, to);

		
		int days = totalSeconds / 86400;
		int hours = (totalSeconds % 86400) / 3600;
		int minutes = (totalSeconds % 3600) / 60;
		int seconds = totalSeconds % 60;

		std::string result = "";

		if (days > 0)
			result += std::to_string(days) + " Day, ";
		if (hours > 0)
			result += std::to_string(hours) + " Hours, ";
		if (minutes > 0)
			result += std::to_string(minutes) + " Minutes, ";

		result += std::to_string(seconds) + " Seconds";

		return result;
	}
};
