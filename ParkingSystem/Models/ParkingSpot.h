#pragma once

// ─────────────────────────────────────────────
// CLASS: ParkingSpot (الموقف الأرضي)
// ─────────────────────────────────────────────
// الدور: يعرف رقمه وحالته، ويحمي نفسه من التغيير غير الشرعي
// العلاقة: مملوك من ParkingSystem عبر Composition
// ─────────────────────────────────────────────
// التصحيحات:
//   ✅ struct → class (لضمان Private بشكل صريح)
//   ✅ vocate → vacate (تصحيح إملائي)
//   ✅ إضافة isFree() (كانت ناقصة)
//   ✅ إزالة #include "Car.h" (غير مطلوب)
//   ✅ enum class بدل enum عادي (أكثر أماناً)
// ─────────────────────────────────────────────

// 1.
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