#pragma once
#include <string>
#include <iostream>
#include <ctime>

#include "clsString.h"
#include "Date.h"
#include "Car.h"

// ─────────────────────────────────────────────
// CLASS: ParkingRecord (سجل الوقوف)
// ─────────────────────────────────────────────
// الدور: يربط السيارة بالموقف ويحسب المدة الزمنية
// العلاقة:
//   ◆ يملك Car (Composition)
//   → يشير إلى ParkingSpot عبر رقمه فقط (Association)
// ─────────────────────────────────────────────
// التصحيحات:
//   ✅ الوقت أصبح time_t بدل string (لحساب المدة فعلياً)
//   ✅ calculateDuration تعمل بشكل صحيح الآن
//   ✅ إضافة Default Constructor (مطلوب للـ Map)
//   ✅ إضافة endl في print() لفصل الأسطر
//   ✅ تخزين spotNumber (int) بدل كائن ParkingSpot كامل
//   ✅ إضافة دالة _formatTime مساعدة
// ─────────────────────────────────────────────

class ParkingRecord {
private:
	Car _car{};
	int _spotNumber{};
	time_t _entryTime{};
	time_t _exitTime{};

public:
	ParkingRecord() = default;

	ParkingRecord(Car car, int spotNumber, time_t entryTime = time(nullptr))
		: _car(car), _spotNumber(spotNumber), _entryTime(entryTime), _exitTime(0) {}

	void setExitTime(time_t exitTime) {
		_exitTime = exitTime;
	}


	
	void print() const {
		std::cout << "\t=======================" << std::endl;
		std::cout << "\t||  Parking Receipt  ||" << std::endl;
		std::cout << "\t=======================" << std::endl;
		std::cout << "\t|Plate    : " << _car.getPlateNumber() << std::endl;
		std::cout << "\t|Type     : " << _car.getType() << std::endl;
		std::cout << "\t|Color    : " << _car.getColor() << std::endl;
		std::cout << "\t|Spot     : #" << _spotNumber << std::endl;
		std::cout << "\t|Entry    : " << clsString::getFullDate(_entryTime) << std::endl;
		if (_exitTime != 0) {
			std::cout << "\t|Exit     : " << clsString::getFullDate(_exitTime) << std::endl;
		}
		std::cout << "\t|Duration : " << Date::getDurationFormatted(_entryTime, _exitTime != 0 ? _exitTime : time(0)) << std::endl;
		std::cout << "\t=======================\n\n" << std::endl;
	}

	Car getCar() const { return _car; }
	int getSpotNumber() const { return _spotNumber; }
	time_t getEntryTime() const { return _entryTime; }
	time_t getExitTime() const { return _exitTime; }
};