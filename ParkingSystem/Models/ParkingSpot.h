#pragma once

enum class ParkState { Free, Occupied };

class ParkingSpot {
private:
	int _parkNumber{};
	ParkState _parkState{};

public:
	ParkingSpot() : _parkNumber(-1), _parkState(ParkState::Free) {}

	ParkingSpot(int parkNumber)
		: _parkNumber(parkNumber), _parkState(ParkState::Free) {}

	bool isFree() const { return _parkState == ParkState::Free; }

	
	void occupy() {
		if (_parkState == ParkState::Occupied) {
			return;
		}
		_parkState = ParkState::Occupied;
	}

	
	void vacate() {
		if (_parkState == ParkState::Free) {
			return;
		}
		_parkState = ParkState::Free;
	}

	int getParkNumber() const { return _parkNumber; }
	ParkState getParkState() const { return _parkState; }
};
