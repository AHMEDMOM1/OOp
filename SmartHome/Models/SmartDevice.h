#pragma once
#include <iostream>


class SmartDevice {
    std::string _name{};
    std::string _type{};
    std::string _serialNumber{};

public:
    SmartDevice(std::string serialNumber, std::string name, std::string type)
        : _name(name), _type(type), _serialNumber(serialNumber) {}
    SmartDevice() = default;
    
    std::string getName() const { return _name; }
    std::string getSerialNumber() const { return _serialNumber; }
    std::string getType() const { return _type; }

    void setName(std::string name) { _name = name; }

    
    virtual void exitMode() = 0;
    virtual std::string getState() = 0;
    
    
    
    virtual ~SmartDevice() = default;
};
