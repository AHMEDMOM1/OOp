#pragma once
#include <iostream>

#include "SmartDevice.h"

enum class LightOperation { Sensor, Time, Always };

class SmartLight : public SmartDevice {
	LightOperation _lightOper{};
	int _brightness{};
	bool _isOn{};
public:

	SmartLight(std::string serialNumber, std::string name, std::string type, int brightness) : SmartDevice(serialNumber, name, type),
		_brightness(brightness), _lightOper(LightOperation::Sensor) {}

	SmartLight() = default;
	void setBrightness(int brightness){
		if (brightness < 0 || brightness > 100) {
			std::cout << "Invalid brightness Value!!\n";
			return;
		}
		_brightness = brightness;
	}
	int getBrightness()const {
		return _brightness;
	}

	void setLightOperation(LightOperation& oper) {
		_lightOper = oper;
	}
	LightOperation getLightOperation()const {
		return _lightOper;
	}

	void turnOn() {
		if (_isOn) {
			std::cout << "Light Already is On!!\n";
			return;
		}
		_isOn = true;
	}
	void turnOff() {
		if (!_isOn) {
			std::cout << "Light Already is Off\n";
			return;
		}
		_isOn = false;
	}

	bool isOn() const{
		return _isOn;
	}

	void exitMode() override {
		turnOff();
	}

	std::string getState() override {
		return _isOn ? "On" : "Off";
	}
};