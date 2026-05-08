#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "ParkingSystem.h"

class ParkingScreen {
private:
	ParkingSystem _parking;
	HANDLE _hConsole;

	// ──── ألوان الطباعة ────
	enum Color {
		WHITE = 7,
		GREEN = 10,
		CYAN = 11,
		RED = 12,
		YELLOW = 14,
		BRIGHT_WHITE = 15
	};

	void _setColor(int color) const {
		SetConsoleTextAttribute(_hConsole, color);
	}

	void _resetColor() const {
		SetConsoleTextAttribute(_hConsole, WHITE);
	}

	// ──── عناصر التنسيق ────
	void _printLine() const {
		_setColor(CYAN);
		std::cout << "\t+---------------------------------------+" << std::endl;
		_resetColor();
	}

	void _printHeader(const std::string& title) {
		std::cout << std::endl;
		_setColor(CYAN);
		std::cout << "\t+=======================================+" << std::endl;
		std::cout << "\t|";
		_setColor(BRIGHT_WHITE);

		int padding = (39 - (int)title.length()) / 2;
		for (int i = 0; i < padding; i++) std::cout << " ";
		std::cout << title;
		for (int i = 0; i < 39 - padding - (int)title.length(); i++) std::cout << " ";

		_setColor(CYAN);
		std::cout << "|" << std::endl;
		std::cout << "\t+=======================================+" << std::endl;
		_resetColor();
	}

	void _printSuccess(const std::string& msg) {
		_setColor(GREEN);
		std::cout << "\n\t  [+] " << msg << std::endl;
		_resetColor();
	}

	void _printError(const std::string& msg) {
		_setColor(RED);
		std::cout << "\n\t  [!] " << msg << std::endl;
		_resetColor();
	}

	void _printInfo(const std::string& label, const std::string& value) {
		_setColor(YELLOW);
		std::cout << "\t  " << label;
		_setColor(BRIGHT_WHITE);
		std::cout << value << std::endl;
		_resetColor();
	}

	void _clearScreen() const {
		system("cls");
	}

	void _pressToContinue() const {
		_setColor(CYAN);
		std::cout << "\n\t  Press Enter to continue...";
		_resetColor();
		std::cin.get();
	}

	// ──── القائمة الرئيسية ────
	void _showMenu() {
		_printHeader("PARKING MANAGEMENT SYSTEM");
		std::cout << std::endl;

		_setColor(GREEN);
		std::cout << "\t  [1]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Entry New Car" << std::endl;

		_setColor(RED);
		std::cout << "\t  [2]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Exit Car" << std::endl;

		_setColor(CYAN);
		std::cout << "\t  [3]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Display Status" << std::endl;

		_setColor(YELLOW);
		std::cout << "\t  [4]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Find Ticket" << std::endl;

		_setColor(YELLOW);
		std::cout << "\t  [5]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Print Records" << std::endl;

		_setColor(RED);
		std::cout << "\t  [0]  ";
		_setColor(BRIGHT_WHITE);
		std::cout << "Quit" << std::endl;

		std::cout << std::endl;
		_printLine();
		_setColor(CYAN);
		std::cout << "\t  >> Choose: ";
		_setColor(BRIGHT_WHITE);
	}

	void _printTicket(const ParkingRecord& record) {
		if (record.getEntryTime() == 0) return;

		_setColor(CYAN);
		std::cout << "\t+---------------------------------------+" << std::endl;
		std::cout << "\t|";
		_setColor(BRIGHT_WHITE);
		std::cout << "           PARKING RECEIPT             ";
		_setColor(CYAN);
		std::cout << "|" << std::endl;
		std::cout << "\t+---------------------------------------+" << std::endl;

		auto printRow = [&](std::string label, std::string value) {
			_setColor(CYAN);
			std::cout << "\t| ";
			_setColor(YELLOW);
			std::cout << label;
			for (int i = 0; i < 12 - (int)label.length(); i++) std::cout << " ";
			_setColor(BRIGHT_WHITE);
			std::cout << ": " << value;
			for (int i = 0; i < 23 - (int)value.length(); i++) std::cout << " ";
			_setColor(CYAN);
			std::cout << " |" << std::endl;
		};

		printRow("Plate", record.getCar().getPlateNumber());
		printRow("Type", record.getCar().getType());
		printRow("Color", record.getCar().getColor());
		printRow("Spot", "#" + std::to_string(record.getSpotNumber()));
		printRow("Entry", clsString::getFullDate(record.getEntryTime()));

		if (record.getExitTime() != 0) {
			printRow("Exit", clsString::getFullDate(record.getExitTime()));
		}

		std::string duration = Date::getDurationFormatted(record.getEntryTime(),
			record.getExitTime() != 0 ? record.getExitTime() : time(0));
		printRow("Duration", duration);

		_setColor(CYAN);
		std::cout << "\t+---------------------------------------+" << std::endl;
		_resetColor();
	}

	void _displayParkingStatus() {
		_clearScreen();
		_printHeader("PARKING STATUS");
		std::cout << std::endl;

		_printInfo("  Total Capacity : ", std::to_string(_parking.getCapacity()));
		_printInfo("  Active Cars    : ", std::to_string(_parking.getSize()));
		_printInfo("  Free Spots     : ", std::to_string(_parking.getCapacity() - _parking.getSize()));

		std::cout << std::endl;
		_printLine();

		for (const auto& spot : _parking.getParkSpots()) {
			_setColor(CYAN);
			std::cout << "\t  Spot #";
			_setColor(BRIGHT_WHITE);
			std::cout << spot.getParkNumber();
			_setColor(CYAN);
			std::cout << " -> ";

			if (spot.isFree()) {
				_setColor(GREEN);
				std::cout << "FREE";
			} else {
				_setColor(RED);
				std::cout << "OCCUPIED";
			}
			_resetColor();
			std::cout << std::endl;
		}

		_printLine();
		_pressToContinue();
	}

	// ──── معالجة الدخول ────
	void _handleEntry() {
		_clearScreen();
		_printHeader("NEW CAR ENTRY");
		std::cout << std::endl;

		std::string plate, type, color;

		_setColor(YELLOW);
		std::cout << "\t  Plate Number : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, plate);

		_setColor(YELLOW);
		std::cout << "\t  Car Type     : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, type);

		_setColor(YELLOW);
		std::cout << "\t  Car Color    : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, color);

		std::cout << std::endl;
		_parking.EntryCar(Car(plate, type, color));
		_pressToContinue();
	}

	// ──── معالجة الخروج ────
	void _handleExit() {
		_clearScreen();
		_printHeader("CAR EXIT");
		std::cout << std::endl;

		std::string plate;
		_setColor(YELLOW);
		std::cout << "\t  Plate Number : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, plate);

		std::cout << std::endl;
		_parking.Exit(plate);
		_pressToContinue();
	}

	void _handleFindTicket() {
		_clearScreen();
		_printHeader("FIND ACTIVE TICKET");
		std::cout << std::endl;

		std::string plate;
		_setColor(YELLOW);
		std::cout << "\t  Plate Number : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, plate);

		std::cout << std::endl;
		ParkingRecord ticket = _parking.findTicket(plate);
		if (ticket.getEntryTime() != 0) {
			_printTicket(ticket);
		} else {
			_printError("No active ticket found for " + plate);
		}
		_pressToContinue();
	}

	void _handlePrintRecords() {
		_clearScreen();
		_printHeader("VEHICLE HISTORY");
		std::cout << std::endl;

		std::string plate;
		_setColor(YELLOW);
		std::cout << "\t  Plate Number : ";
		_setColor(BRIGHT_WHITE);
		std::getline(std::cin, plate);

		bool found = false;

		// Active Ticket
		ParkingRecord active = _parking.findTicket(plate);
		if (active.getEntryTime() != 0) {
			_setColor(YELLOW);
			std::cout << "\n\t  [ CURRENTLY PARKED ]" << std::endl;
			_printTicket(active);
			found = true;
		}

		// Archives
		for (const auto& record : _parking.getArchives()) {
			if (record.getCar().getPlateNumber() == plate) {
				if (!found) std::cout << std::endl;
				_setColor(CYAN);
				std::cout << "\t  [ ARCHIVED RECORD ]" << std::endl;
				_printTicket(record);
				found = true;
			}
		}

		if (!found) {
			_printError("No records found for " + plate);
		}

		_pressToContinue();
	}

public:
	ParkingScreen(int capacity)
		: _parking(capacity), _hConsole(GetStdHandle(STD_OUTPUT_HANDLE)) {}

	// ──── شاشة الإعداد الأولية ────
	static ParkingScreen setup() {
		system("cls");
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << std::endl;
		SetConsoleTextAttribute(h, 11);
		std::cout << "\t+=======================================+" << std::endl;
		std::cout << "\t|                                       |" << std::endl;
		SetConsoleTextAttribute(h, 15);
		std::cout << "\t|";
		SetConsoleTextAttribute(h, 14);
		std::cout << "      PARKING SYSTEM  v1.0        ";
		SetConsoleTextAttribute(h, 11);
		std::cout << "|" << std::endl;
		std::cout << "\t|                                       |" << std::endl;
		std::cout << "\t+=======================================+" << std::endl;
		SetConsoleTextAttribute(h, 7);

		std::cout << std::endl;
		SetConsoleTextAttribute(h, 11);
		std::cout << "\t  Welcome! Let's configure your parking." << std::endl;
		std::cout << std::endl;

		SetConsoleTextAttribute(h, 14);
		std::cout << "\t  How many spots? : ";
		SetConsoleTextAttribute(h, 15);

		int capacity;
		if (!(std::cin >> capacity)) {
			capacity = 5;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		} else {
			std::cin.ignore();
		}

		std::cout << std::endl;
		SetConsoleTextAttribute(h, 10);
		std::cout << "\t  [+] Parking created with " << capacity << " spots!" << std::endl;
		SetConsoleTextAttribute(h, 11);
		std::cout << "\n\t  Press Enter to start...";
		SetConsoleTextAttribute(h, 7);
		std::cin.get();
		system("cls");

		return ParkingScreen(capacity);
	}

	void run() {
		int choice = -1;

		while (choice != 0) {
			_clearScreen();
			_showMenu();

			if (!(std::cin >> choice)) {
				choice = -1;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
			} else {
				std::cin.ignore();
			}

			switch (choice) {
			case 1: _handleEntry();              break;
			case 2: _handleExit();               break;
			case 3: _displayParkingStatus();     break;
			case 4: _handleFindTicket();         break;
			case 5: _handlePrintRecords();       break;
			case 0:
				_setColor(GREEN);
				std::cout << "\n\t  Goodbye! \n" << std::endl;
				_resetColor();
				break;
			default:
				_printError("Invalid choice!");
				_pressToContinue();
				break;
			}
		}
	}
};
