#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include "ParkingSpot.h"
#include "ParkingRecord.h"

// ─────────────────────────────────────────────
// CLASS: ParkingSystem (مدير المواقف)
// ─────────────────────────────────────────────
// الدور: العقل المدبر — يدير الدخول والخروج والبحث والعرض
// العلاقات:
//   ◆ يملك vector<ParkingSpot>              (المواقف الأرضية)
//   ◆ يملك map<string, ParkingRecord>       (السجلات النشطة)
//   ◆ يملك vector<ParkingRecord>            (الأرشيف)
// ─────────────────────────────────────────────
// التصحيحات:
//   ✅ إكمال دالة entryNewCar (كانت ناقصة)
//   ✅ إصلاح findFirstFreeSpot لترجع index (بدل نسخة)
//   ✅ إضافة دالة Exit (كانت غير موجودة)
//   ✅ إضافة displayStatus (كانت غير موجودة)
//   ✅ إضافة printRecords (كانت غير موجودة)
//   ✅ إصلاح bug إشغال النسخة بدل الأصل
//   ✅ إصلاح الترقيم ليبدأ من 1 (وليس 0)
//   ✅ إزالة #include "UseFullLib.h" (غير موجود)
// ─────────────────────────────────────────────

class ParkingSystem {
private:
	std::vector<ParkingSpot> _parkSpots{};
	std::unordered_map<std::string, ParkingRecord> _tickets{};
	std::vector<ParkingRecord> _archives{};
	int _capacity{};

	int _findFirstFreeSpotIndex() const {
		for (int i = 0; i < (int)_parkSpots.size(); i++) {
			if (_parkSpots[i].isFree()) {
				return i;
			}
		}
		return -1;
	}

public:
	
	ParkingSystem(int capacity) : _capacity(capacity) {
		for (int i = 1; i <= capacity; i++) {
			_parkSpots.push_back(ParkingSpot(i));
		}
	}

	
	void EntryCar(Car car) {
		std::string plateNumber = car.getPlateNumber();

		
		if (_tickets.count(plateNumber) > 0) {
			std::cout << "\t[!] Car " << plateNumber << " is already parked!" << std::endl;
			return;
		}

		
		int spotIndex = _findFirstFreeSpotIndex();
		if (spotIndex == -1) {
			std::cout << "\t[!] Parking is FULL! No available spots." << std::endl;
			return;
		}

		
		_parkSpots[spotIndex].occupy();
		ParkingRecord record(car, _parkSpots[spotIndex].getParkNumber());
		_tickets[plateNumber] = record;
		std::cout << "\t[+] Car " << '{' << plateNumber << '}' << " entered -> Spot #"
			<< _parkSpots[spotIndex].getParkNumber()
			<< " (" << _tickets.size() << "/" << _capacity << ")" << std::endl;
	}

	
	void Exit(const std::string& plateNumber) {
		
		if (_tickets.count(plateNumber) == 0) {
			std::cout << "\t[!] Car " << plateNumber << " is NOT in the parking!" << std::endl;
			return;
		}

		
		ParkingRecord record = _tickets[plateNumber];
		record.setExitTime(time(0));

		
		int spotIndex = record.getSpotNumber() - 1;
		_parkSpots[spotIndex].vacate();

		
		record.print();

		
		_archives.push_back(record);
		_tickets.erase(plateNumber);
	}


	ParkingRecord findTicket(const std::string& plateNumber) const {
		if (_tickets.count(plateNumber) > 0) {
			return _tickets.at(plateNumber);
		}
		std::cout << "\t[!] No active ticket found for " << plateNumber << std::endl;
		return ParkingRecord();
	}

	
	void displayStatus() const {
		std::cout << "\n\t===============================" << std::endl;
		std::cout << "\t       Parking Status" << std::endl;
		std::cout << "\t===============================" << std::endl;
		std::cout << "\tCapacity : " << _capacity << std::endl;
		std::cout << "\tParked   : " << _tickets.size() << std::endl;
		std::cout << "\tFree     : " << (_capacity - (int)_tickets.size()) << std::endl;
		std::cout << "\t-------------------------------" << std::endl;
		for (const auto& spot : _parkSpots) {
			std::cout << "\tSpot #" << spot.getParkNumber() << " -> "
				<< (spot.isFree() ? "FREE" : "OCCUPIED") << std::endl;
		}
		std::cout << "\t===============================" << std::endl;
	}

	
	void printRecords(const std::string& plateNumber) const {
		bool found = false;

		
		if (_tickets.count(plateNumber) > 0) {
			std::cout << "\n\t--- Active Record ---" << std::endl;
			_tickets.at(plateNumber).print();
			found = true;
		}

		
		for (const auto& record : _archives) {
			if (record.getCar().getPlateNumber() == plateNumber) {
				std::cout << "\n\t--- Archive Record ---" << std::endl;
				record.print();
				found = true;
			}
		}

		if (!found) {
			std::cout << "\t[!] No records found for " << plateNumber << std::endl;
		}
	}

	
	int getCapacity() const { return _capacity; }
	int getSize() const { return (int)_tickets.size(); }

	const std::vector<ParkingSpot>& getParkSpots() const { return _parkSpots; }
	const std::unordered_map<std::string, ParkingRecord>& getTickets() const { return _tickets; }
	const std::vector<ParkingRecord>& getArchives() const { return _archives; }
};