#pragma once
#include <iostream>

#include "SmartDevice.h"

class SmartLock : public SmartDevice {
	bool _isLocked{};

public:
	SmartLock(std::string serialNumber, std::string name, std::string type) :
		SmartDevice(serialNumber, name, type), _isLocked(false) {}
	SmartLock() = default;

	void lock() {
		if (_isLocked) {
			std::cout << "Already locked!\n";
			return;
		}
		_isLocked = true;
	}

	void unlock() {
		if (!_isLocked) {
			std::cout << "Already unlocked!\n";
			return ;
		}
		_isLocked = false;
	}

	bool isLocked() {
		return _isLocked;
	}

	void exitMode() override{
		lock();
	}

	std::string getState() override {
		return _isLocked ? "Locked" : "Opened";
	}



};