#pragma once
#include <iostream>
#include "UIHelper.h"
#include "../Manager/SmartHomeSystem.h"

using namespace std;

class RoomsScreen {
    enum enRoomsOptions {
        eAddRoom = 1,
        eDeleteRoom = 2,
        eShowRooms = 3,
        eGoBack = 4
    };

    static void _ShowRooms(SmartHomeSystem* system) {
        UIHelper::DrawScreenHeader("List of Rooms", UIHelper::ICON_ROOM);
        if (system->getRooms().empty()) {
            UIHelper::PrintWarning("No rooms found.");
            UIHelper::PrintInfo("Use 'Add New Room' to create one.");
            return;
        }

        cout << "\n";

        // Column widths: #(5), Name(22), Capacity(12), Devices(12) = 51 + 3 separators
        const int NUM_COLS = 4;
        const int colW[NUM_COLS] = {5, 22, 12, 12};

        // Top border
        UIHelper::DrawTableBorder(UIHelper::BOX_TL, UIHelper::BOX_H, UIHelper::BOX_TR,
                                   UIHelper::BOX_H, colW, NUM_COLS);

        // Header row
        string headers[NUM_COLS] = {"#", "Room Name", "Capacity", "Devices"};
        string headerColors[NUM_COLS] = {
            UIHelper::BOLD + UIHelper::WHITE,
            UIHelper::BOLD + UIHelper::WHITE,
            UIHelper::BOLD + UIHelper::WHITE,
            UIHelper::BOLD + UIHelper::WHITE
        };
        UIHelper::DrawTableRow(headers, headerColors, colW, NUM_COLS);

        // Middle separator
        UIHelper::DrawTableBorder(UIHelper::BOX_LT, UIHelper::BOX_H, UIHelper::BOX_RT,
                                   UIHelper::BOX_H, colW, NUM_COLS);

        // Data rows
        int index = 1;
        for (Room* room : system->getRooms()) {
            string name = room->getName();
            if (name.size() > 20) name = name.substr(0, 17) + "...";

            float ratio = room->getMaxDeviceCapacity() > 0 
                ? (float)room->getDeviceSize() / room->getMaxDeviceCapacity() : 0;
            string devColor = ratio < 0.5f ? UIHelper::GREEN 
                            : ratio < 0.8f ? UIHelper::YELLOW 
                            : UIHelper::RED;

            string cells[NUM_COLS] = {
                to_string(index),
                name,
                to_string(room->getMaxDeviceCapacity()),
                to_string(room->getDeviceSize())
            };
            string colors[NUM_COLS] = {
                UIHelper::GRAY,
                UIHelper::WHITE,
                UIHelper::TEAL,
                devColor
            };
            UIHelper::DrawTableRow(cells, colors, colW, NUM_COLS);
            index++;
        }

        // Bottom border
        UIHelper::DrawTableBorder(UIHelper::BOX_BL, UIHelper::BOX_H, UIHelper::BOX_BR,
                                   UIHelper::BOX_H, colW, NUM_COLS);
        
        cout << "\n  " << UIHelper::DIM << UIHelper::GRAY 
             << "Total: " << system->getRooms().size() << " room(s)" 
             << UIHelper::RESET << "\n";
    }

    static void _AddRoom(SmartHomeSystem* system) {
        UIHelper::DrawScreenHeader("Add New Room", UIHelper::ICON_PLUS);
        
        string name = UIHelper::ReadString("Enter Room Name: ");
        
        if (system->findRoom(name) != -1) {
            UIHelper::PrintError("A room with this name already exists!");
            return;
        }

        int capacity = UIHelper::ReadInt("Enter Maximum Device Capacity: ");
        if (capacity <= 0) {
            UIHelper::PrintError("Capacity must be greater than zero.");
            return;
        }

        Room* newRoom = new Room(name, capacity);
        system->addRoom(newRoom);
        UIHelper::PrintSuccess("Room '" + name + "' added successfully!");
    }

    static void _DeleteRoom(SmartHomeSystem* system) {
        UIHelper::DrawScreenHeader("Delete Room", UIHelper::ICON_WARN);
        
        if (system->getRooms().empty()) {
            UIHelper::PrintWarning("No rooms to delete.");
            return;
        }

        // Show available rooms
        cout << "\n  " << UIHelper::GRAY << "Available rooms:" << UIHelper::RESET << "\n";
        for (Room* room : system->getRooms()) {
            cout << "   " << UIHelper::ICON_DOT << " " << UIHelper::WHITE << room->getName() << UIHelper::RESET << "\n";
        }

        string name = UIHelper::ReadString("Enter Room Name to delete: ");
        
        int idx = system->findRoom(name);
        if (idx == -1) {
            UIHelper::PrintError("Room not found!");
            return;
        }

        // Warn if room has devices
        Room* room = system->getRooms().at(idx);
        if (!room->isEmpty()) {
            UIHelper::PrintWarning("This room contains " + to_string(room->getDeviceSize()) + " device(s).");
            cout << "  " << UIHelper::YELLOW << "  All devices will be removed." << UIHelper::RESET << "\n";
        }

        // Clean up device memory before deleting room
        for (auto& [serial, device] : room->getDevices()) {
            delete device;
        }

        system->deleteRoom(name);
        delete room;
        UIHelper::PrintSuccess("Room '" + name + "' deleted successfully!");
    }

    static void _PerformOption(enRoomsOptions option, SmartHomeSystem* homeSystem) {
        switch (option) {
            case eAddRoom:
                ::system("cls");
                _AddRoom(homeSystem);
                UIHelper::PauseScreen("Press Enter to return to Rooms Menu...");
                break;
            case eDeleteRoom:
                ::system("cls");
                _DeleteRoom(homeSystem);
                UIHelper::PauseScreen("Press Enter to return to Rooms Menu...");
                break;
            case eShowRooms:
                ::system("cls");
                _ShowRooms(homeSystem);
                UIHelper::PauseScreen("Press Enter to return to Rooms Menu...");
                break;
            case eGoBack:
                break;
        }
    }

public:
    static void Show(SmartHomeSystem* homeSystem) {
        bool goBack = false;
        while (!goBack) {
            ::system("cls");
            UIHelper::DrawScreenHeader("Rooms Management", UIHelper::ICON_ROOM);

            cout << "\n";
            UIHelper::DrawMenuItem(1, "Add New Room", UIHelper::ICON_PLUS);
            UIHelper::DrawMenuItem(2, "Delete Room", UIHelper::ICON_CROSS);
            UIHelper::DrawMenuItem(3, "Show All Rooms", UIHelper::ICON_INFO);
            UIHelper::DrawMenuDivider();
            UIHelper::DrawMenuItem(4, "Go Back", UIHelper::ICON_ARROW);
            cout << "\n";

            int choice = UIHelper::ReadInt("Choose an option [1-4]: ");
            
            if (choice >= 1 && choice <= 4) {
                enRoomsOptions option = (enRoomsOptions)choice;
                if (option == eGoBack) {
                    goBack = true;
                } else {
                    _PerformOption(option, homeSystem);
                }
            } else {
                UIHelper::PrintError("Invalid option. Please try again.");
                UIHelper::PauseScreen();
            }
        }
    }
};
