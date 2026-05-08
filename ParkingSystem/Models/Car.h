#pragma once
#include <string>

class Car {
private:
	std::string _plateNumber{};
	std::string _type{};
	std::string _color{};

public:
	Car() = default;

	Car(std::string plateNumber, std::string type, std::string color)
		: _plateNumber(plateNumber), _type(type), _color(color) {}

	std::string getPlateNumber() const { return _plateNumber; }
	std::string getType() const { return _type; }
	std::string getColor() const { return _color; }
};