#pragma once
#include <map>
#include <iostream>

#include "SmartDevice.h"

class Room {
  std::string _name{};
  int _maxDevicesCapacity{};

  std::map<std::string, SmartDevice *> _devices{};

public:
  Room(std::string roomName, int maxDeviceCapactiy)
      : _name(roomName), _maxDevicesCapacity(maxDeviceCapactiy) {}
  Room() = default;

  void setName(std::string name) { _name = name; }
  std::string getName() const { return _name; }
  void setMaxDeviceCapacity(int maxCapacity) { _maxDevicesCapacity = maxCapacity; }
  int getMaxDeviceCapacity() const { return _maxDevicesCapacity; }
  int getDeviceSize() const { return (int)_devices.size(); }
  std::map<std::string, SmartDevice *>& getDevices() { return _devices; }

  bool addDevice(SmartDevice* device) {
    if (_devices.size() >= _maxDevicesCapacity) {
      std::cout << "Room is Full!!\n";
      return false;
    }

    if (_devices.contains(device->getSerialNumber())) {
        std::cout << "Device already exists!!\n";
        return false;
    }

    _devices.emplace(device->getSerialNumber(), device);
    return true;
  }

  bool deleteDevice(std::string serialNumber) {
    if (_devices.find(serialNumber) == _devices.end()) {
      std::cout << "Device Not Found!!\n";
      return false;
    }

    _devices.erase(serialNumber);
    return true;
  }

  auto findDevice(std::string serialNumber) {
    return _devices.find(serialNumber);
  }

  void exitModeAll() {
    for (auto &[serialNumber, device] : _devices) {
      device->exitMode();
    }
  }

  bool isEmpty() { return _devices.empty(); }
};