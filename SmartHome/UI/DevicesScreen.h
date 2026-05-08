#pragma once
#include <iostream>
#include "UIHelper.h"
#include "../Manager/SmartHomeSystem.h"
#include "../Models/SmartLight.h"
#include "../Models/SmartAirConditioner.h"
#include "../Models/SmartLock.h"

using namespace std;

class DevicesScreen {
    enum enDeviceOptions {
        eAddDevice = 1,
        eDeleteDevice = 2,
        eControlDevice = 3,
        eShowDevices = 4,
        eGoBack = 5
    };

    static void _ShowDevices(Room* room) {
        UIHelper::DrawScreenHeader("Devices in " + room->getName(), UIHelper::ICON_GEAR);
        if (room->getDevices().empty()) {
            UIHelper::PrintWarning("No devices in this room.");
            UIHelper::PrintInfo("Use 'Add Device' to get started.");
            return;
        }

        cout << "\n";

        // Column widths: #(5), Name(20), Type(10), Serial(14), State(10)
        const int NUM_COLS = 5;
        const int colW[NUM_COLS] = {5, 20, 10, 14, 10};

        // Top border
        UIHelper::DrawTableBorder(UIHelper::BOX_TL, UIHelper::BOX_H, UIHelper::BOX_TR,
                                   UIHelper::BOX_H, colW, NUM_COLS);

        // Header row
        string headers[NUM_COLS] = {"#", "Device Name", "Type", "Serial", "State"};
        string headerColors[NUM_COLS] = {
            UIHelper::BOLD + UIHelper::WHITE,
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
        int idx = 1;
        for (auto& [serial, device] : room->getDevices()) {
            string name = device->getName();
            if (name.size() > 18) name = name.substr(0, 15) + "...";
            
            string type = device->getType();
            
            string ser = device->getSerialNumber();
            if (ser.size() > 12) ser = ser.substr(0, 9) + "...";
            
            string state = device->getState();
            string stateColor = (state == "On" || state == "Opened") ? UIHelper::GREEN : UIHelper::RED;

            string cells[NUM_COLS] = {
                to_string(idx),
                name,
                type,
                ser,
                state
            };
            string colors[NUM_COLS] = {
                UIHelper::GRAY,
                UIHelper::WHITE,
                UIHelper::TEAL,
                UIHelper::GOLD,
                stateColor
            };
            UIHelper::DrawTableRow(cells, colors, colW, NUM_COLS);
            idx++;
        }

        // Bottom border
        UIHelper::DrawTableBorder(UIHelper::BOX_BL, UIHelper::BOX_H, UIHelper::BOX_BR,
                                   UIHelper::BOX_H, colW, NUM_COLS);

        cout << "\n  " << UIHelper::DIM << UIHelper::GRAY 
             << "Total: " << room->getDevices().size() << " device(s)  |  Capacity: "
             << room->getDeviceSize() << "/" << room->getMaxDeviceCapacity()
             << UIHelper::RESET << "\n";
    }

    static void _AddDevice(Room* room) {
        UIHelper::DrawScreenHeader("Add New Device", UIHelper::ICON_PLUS);
        
        cout << "\n";
        UIHelper::DrawSubHeader("Select Device Type");
        cout << "\n";
        UIHelper::DrawMenuItem(1, "Smart Light", UIHelper::ICON_BULB);
        UIHelper::DrawMenuItem(2, "Smart Air Conditioner", UIHelper::ICON_SNOW);
        UIHelper::DrawMenuItem(3, "Smart Door Lock", UIHelper::ICON_LOCK);
        cout << "\n";

        int typeChoice = UIHelper::ReadInt("Choice: ");

        if (typeChoice < 1 || typeChoice > 3) {
            UIHelper::PrintError("Invalid choice.");
            return;
        }

        string serial = UIHelper::ReadString("Enter Serial Number: ");
        if (room->findDevice(serial) != room->getDevices().end()) {
            UIHelper::PrintError("Device with this serial already exists in this room.");
            return;
        }

        string name = UIHelper::ReadString("Enter Device Name: ");
        SmartDevice* newDevice = nullptr;

        if (typeChoice == 1) {
            int brightness = UIHelper::ReadInt("Enter Initial Brightness (0-100): ");
            newDevice = new SmartLight(serial, name, "Light", brightness);
            cout << "\n  " << UIHelper::ICON_BULB << " " << UIHelper::GRAY 
                 << "Light created with brightness: " << UIHelper::TEAL << brightness << "%" 
                 << UIHelper::RESET << "\n";
        } else if (typeChoice == 2) {
            int temp = UIHelper::ReadInt("Enter Initial Temperature (16-30): ");
            newDevice = new SmartAirConditioner(serial, name, "AC", temp);
            cout << "\n  " << UIHelper::ICON_SNOW << " " << UIHelper::GRAY 
                 << "AC created with temperature: " << UIHelper::TEAL << temp << "\xC2\xB0" << "C" 
                 << UIHelper::RESET << "\n";
        } else {
            newDevice = new SmartLock(serial, name, "Lock");
            cout << "\n  " << UIHelper::ICON_LOCK << " " << UIHelper::GRAY 
                 << "Door Lock created (default: unlocked)" 
                 << UIHelper::RESET << "\n";
        }

        if (room->addDevice(newDevice)) {
            UIHelper::PrintSuccess("Device '" + name + "' added successfully!");
        } else {
            delete newDevice;
        }
    }

    static void _DeleteDevice(Room* room) {
        UIHelper::DrawScreenHeader("Delete Device", UIHelper::ICON_WARN);
        
        if (room->getDevices().empty()) {
            UIHelper::PrintWarning("No devices to delete.");
            return;
        }

        // Show devices
        cout << "\n  " << UIHelper::GRAY << "Devices in this room:" << UIHelper::RESET << "\n";
        for (auto& [serial, device] : room->getDevices()) {
            string icon = UIHelper::GetDeviceIcon(device->getType());
            cout << "   " << icon << " " << UIHelper::WHITE << device->getName() 
                 << UIHelper::GRAY << " (Serial: " << UIHelper::GOLD << serial 
                 << UIHelper::GRAY << ")" << UIHelper::RESET << "\n";
        }

        string serial = UIHelper::ReadString("Enter Device Serial Number: ");
        
        auto it = room->findDevice(serial);
        if (it == room->getDevices().end()) {
            UIHelper::PrintError("Device not found.");
            return;
        }

        string deviceName = it->second->getName();
        delete it->second; // Free device memory
        room->deleteDevice(serial);
        UIHelper::PrintSuccess("Device '" + deviceName + "' deleted successfully!");
    }

    static void _ControlDevice(Room* room) {
        UIHelper::DrawScreenHeader("Control Device", UIHelper::ICON_GEAR);
        
        if (room->getDevices().empty()) {
            UIHelper::PrintWarning("No devices to control.");
            return;
        }

        // Show devices
        cout << "\n  " << UIHelper::GRAY << "Devices in this room:" << UIHelper::RESET << "\n";
        for (auto& [serial, device] : room->getDevices()) {
            string icon = UIHelper::GetDeviceIcon(device->getType());
            cout << "   " << icon << " " << UIHelper::WHITE << device->getName()
                 << UIHelper::GRAY << " (Serial: " << UIHelper::GOLD << serial 
                 << UIHelper::GRAY << ") " << UIHelper::RESET << "  ";
            UIHelper::PrintDeviceState(device->getState());
            cout << "\n";
        }

        string serial = UIHelper::ReadString("Enter Device Serial Number: ");
        
        auto it = room->findDevice(serial);
        if (it == room->getDevices().end()) {
            UIHelper::PrintError("Device not found.");
            return;
        }

        SmartDevice* device = it->second;
        
        cout << "\n  " << UIHelper::GRAY << "Current State: " << UIHelper::RESET;
        UIHelper::PrintDeviceState(device->getState());
        cout << "\n\n";
        
        // Dynamic cast to check features
        SmartLight* light = dynamic_cast<SmartLight*>(device);
        SmartAirConditioner* ac = dynamic_cast<SmartAirConditioner*>(device);
        SmartLock* lock = dynamic_cast<SmartLock*>(device);

        if (light) {
            UIHelper::DrawSubHeader("Light Controls");
            cout << "\n";
            UIHelper::DrawMenuItem(1, "Turn On", UIHelper::ICON_DOT);
            UIHelper::DrawMenuItem(2, "Turn Off", UIHelper::ICON_DOT);
            UIHelper::DrawMenuItem(3, "Change Brightness", UIHelper::ICON_STAR);
            cout << "\n";
            int action = UIHelper::ReadInt("Choice: ");
            if (action == 1) { 
                light->turnOn(); 
                UIHelper::PrintSuccess("Light turned ON.");
            }
            else if (action == 2) { 
                light->turnOff(); 
                UIHelper::PrintSuccess("Light turned OFF.");
            }
            else if (action == 3) {
                cout << "  " << UIHelper::GRAY << "Current brightness: " 
                     << UIHelper::TEAL << light->getBrightness() << "%" << UIHelper::RESET << "\n";
                int val = UIHelper::ReadInt("New Brightness (0-100): ");
                light->setBrightness(val);
                UIHelper::PrintSuccess("Brightness set to " + to_string(val) + "%.");
            }
            else {
                UIHelper::PrintError("Invalid action.");
            }
        } else if (ac) {
            UIHelper::DrawSubHeader("Air Conditioner Controls");
            cout << "\n";
            UIHelper::DrawMenuItem(1, "Turn On", UIHelper::ICON_DOT);
            UIHelper::DrawMenuItem(2, "Turn Off", UIHelper::ICON_DOT);
            UIHelper::DrawMenuItem(3, "Change Temperature", UIHelper::ICON_SNOW);
            cout << "\n";
            int action = UIHelper::ReadInt("Choice: ");
            if (action == 1) {
                ac->turnOn();
                UIHelper::PrintSuccess("Air Conditioner turned ON.");
            }
            else if (action == 2) {
                ac->turnOff();
                UIHelper::PrintSuccess("Air Conditioner turned OFF.");
            }
            else if (action == 3) {
                cout << "  " << UIHelper::GRAY << "Current temperature: " 
                     << UIHelper::TEAL << ac->getTemperature() << "\xC2\xB0" << "C" << UIHelper::RESET << "\n";
                int val = UIHelper::ReadInt("New Temperature (16-30): ");
                ac->setTemperature(val);
                UIHelper::PrintSuccess("Temperature set to " + to_string(val) + "\xC2\xB0" + "C.");
            }
            else {
                UIHelper::PrintError("Invalid action.");
            }
        } else if (lock) {
            UIHelper::DrawSubHeader("Door Lock Controls");
            cout << "\n";
            UIHelper::DrawMenuItem(1, "Lock", UIHelper::ICON_LOCK);
            UIHelper::DrawMenuItem(2, "Unlock", UIHelper::ICON_DOOR);
            cout << "\n";
            int action = UIHelper::ReadInt("Choice: ");
            if (action == 1) {
                lock->lock();
                UIHelper::PrintSuccess("Door LOCKED.");
            }
            else if (action == 2) {
                lock->unlock();
                UIHelper::PrintSuccess("Door UNLOCKED.");
            }
            else {
                UIHelper::PrintError("Invalid action.");
            }
        }
    }

    static void _RoomDevicesMenu(Room* room) {
        bool goBack = false;
        while (!goBack) {
            ::system("cls");
            UIHelper::DrawScreenHeader("Room: " + room->getName(), UIHelper::ICON_ROOM);
            
            // Show room capacity info
            cout << "\n  " << UIHelper::GRAY << "Capacity: " << UIHelper::RESET;
            UIHelper::PrintCapacityBar(room->getDeviceSize(), room->getMaxDeviceCapacity(), 20);
            cout << "\n\n";

            UIHelper::DrawMenuItem(1, "Add Device", UIHelper::ICON_PLUS);
            UIHelper::DrawMenuItem(2, "Delete Device", UIHelper::ICON_CROSS);
            UIHelper::DrawMenuItem(3, "Control Device", UIHelper::ICON_GEAR);
            UIHelper::DrawMenuItem(4, "Show All Devices", UIHelper::ICON_INFO);
            UIHelper::DrawMenuDivider();
            UIHelper::DrawMenuItem(5, "Go Back", UIHelper::ICON_ARROW);
            cout << "\n";

            int choice = UIHelper::ReadInt("Choose an option [1-5]: ");
            
            switch (choice) {
                case eAddDevice:
                    ::system("cls"); _AddDevice(room); 
                    UIHelper::PauseScreen("Press Enter to return..."); 
                    break;
                case eDeleteDevice:
                    ::system("cls"); _DeleteDevice(room); 
                    UIHelper::PauseScreen("Press Enter to return..."); 
                    break;
                case eControlDevice:
                    ::system("cls"); _ControlDevice(room); 
                    UIHelper::PauseScreen("Press Enter to return..."); 
                    break;
                case eShowDevices:
                    ::system("cls"); _ShowDevices(room); 
                    UIHelper::PauseScreen("Press Enter to return..."); 
                    break;
                case eGoBack:
                    goBack = true; break;
                default:
                    UIHelper::PrintError("Invalid option."); 
                    UIHelper::PauseScreen(); 
                    break;
            }
        }
    }

public:
    static void Show(SmartHomeSystem* homeSystem) {
        ::system("cls");
        UIHelper::DrawScreenHeader("Manage Devices", UIHelper::ICON_GEAR);
        
        if (homeSystem->getRooms().empty()) {
            UIHelper::PrintError("No rooms exist. Please create a room first.");
            UIHelper::PrintInfo("Go to 'Manage Rooms' to add a room.");
            UIHelper::PauseScreen("Press Enter to return...");
            return;
        }

        // Show available rooms with numbers
        cout << "\n";
        UIHelper::DrawSubHeader("Select a Room");
        cout << "\n";
        
        int i = 1;
        for (Room* room : homeSystem->getRooms()) {
            cout << "   " << UIHelper::BLUE << "[" << UIHelper::BOLD << UIHelper::WHITE << i 
                 << UIHelper::RESET << UIHelper::BLUE << "]" << UIHelper::RESET
                 << " " << UIHelper::ICON_DOOR << " " << UIHelper::WHITE << room->getName() << UIHelper::RESET
                 << UIHelper::GRAY << " (" << room->getDeviceSize() << "/" 
                 << room->getMaxDeviceCapacity() << " devices)" << UIHelper::RESET << "\n";
            i++;
        }

        cout << "\n";
        UIHelper::DrawMenuDivider();
        cout << "   " << UIHelper::DIM << UIHelper::GRAY << "Type 'cancel' or enter 0 to go back" << UIHelper::RESET << "\n";
        
        string roomInput = UIHelper::ReadString("Enter Room Name (or number): ");
        if (roomInput == "cancel" || roomInput == "0") return;

        // Try to find by number first
        int roomIndex = -1;
        try {
            int num = stoi(roomInput);
            if (num >= 1 && num <= (int)homeSystem->getRooms().size()) {
                roomIndex = num - 1;
            }
        } catch (...) {
            // Not a number, try by name
        }

        if (roomIndex == -1) {
            roomIndex = homeSystem->findRoom(roomInput);
        }

        if (roomIndex == -1) {
            UIHelper::PrintError("Room not found.");
            UIHelper::PauseScreen("Press Enter to return...");
            return;
        }

        Room* selectedRoom = homeSystem->getRooms().at(roomIndex);
        _RoomDevicesMenu(selectedRoom);
    }
};
