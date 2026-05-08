#pragma once
#include <iostream>

#include "SmartDevice.h"
enum class Direction{Up, Down, Left, Right};

class SmartAirConditioner : public SmartDevice {
	int _temperature{};
	Direction _direction{};
	bool _isOn{};
public:
	SmartAirConditioner(std::string serialNumber, std::string name, std::string type, int temperature, Direction direction = Direction::Up) : SmartDevice(serialNumber, name, type) {
		_temperature = temperature;
		_direction = direction;
		_isOn = false;
	}
	SmartAirConditioner() = default;
	void setTemperature(int temp) {
		if (temp < 16 || temp > 30) {
			std::cout << "Invalid temperature!!\n";
			return;
		}

		_temperature = temp;
	}

	int getTemperature()const {
		return _temperature;
	}

	void setDirection(Direction& dir) {
		_direction = dir;
	}

	Direction getDirection()const {
		return _direction;
	}

	void exitMode()override {
		_isOn = false;
	}

	void turnOn() {
		if (_isOn) {
			std::cout << "Air Conditioner Already is On!!\n";
			return;
		}
		_isOn = true;
	}
	void turnOff() {
		if (!_isOn) {
			std::cout << "Air Conditioner Already is Off!!\n";
			return;
		}
		_isOn = false;
	}

	std::string getState() override {
		return _isOn ? "On" : "Off";
	}
};