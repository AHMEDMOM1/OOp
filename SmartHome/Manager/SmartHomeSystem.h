#pragma once
#include <iostream>
#include <vector>

#include "Room.h"
#include "SmartDevice.h"

class SmartHomeSystem {
	std::string _name{};
	std::vector<Room*> _rooms{};
	int _maxCapacityRooms{};
public:
	SmartHomeSystem(std::string name, int maxCapacityRooms) : _name(name), _maxCapacityRooms(maxCapacityRooms), _rooms() {}
	SmartHomeSystem() = default;

	void setName(std::string name) {
		_name = name;
	}
	std::string getName()const {
		return _name;
	}

	void setMaxCapacityRooms(int capacity) {
		_maxCapacityRooms = capacity;
	}
	int getMaxCapacityRooms()const {
		return _maxCapacityRooms;
	}

	std::vector<Room*>& getRooms() {
		return _rooms;
	}

	int findRoom(std::string name) {
		for (int i{}; i < (int)_rooms.size(); i++) {
			if (_rooms.at(i)->getName() == name) {
				return i;
			}
		}
		return -1;
	}

	void addRoom(Room* room) {
		if (_rooms.size() >= _maxCapacityRooms) {
			std::cout << "Your Home is Full!!\n";
			return;
		}
		if (findRoom(room->getName()) != -1) {
			std::cout << "This Name Already Used!!\n";
			return;
		}

		_rooms.emplace_back(room);
	}

	void deleteRoom(std::string name) {
		int index{ findRoom(name) };
		if (index == -1) {
			std::cout << "Room is not Found!!\n";
			return;
		}
		_rooms.erase(_rooms.begin() + index);
	}
	
	void exitMode() {
		for (Room* room : _rooms) {
			room->exitModeAll();
		}
	}

};
