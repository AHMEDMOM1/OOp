#pragma once
#include <iostream>
#include "UIHelper.h"
#include "RoomsScreen.h"
#include "DevicesScreen.h"
#include "../Manager/SmartHomeSystem.h"

using namespace std;

class MainMenuScreen {
    enum enMainMenuOptions {
        eShowStatus = 1,
        eManageRooms = 2,
        eManageDevices = 3,
        eExitHomeMode = 4,
        eQuit = 5
    };

    static void _ShowStatusScreen(SmartHomeSystem* system) {
        UIHelper::DrawScreenHeader("Smart Home Status", UIHelper::ICON_INFO);
        
        if (system->getRooms().empty()) {
            UIHelper::PrintWarning("The home has no rooms yet.");
            UIHelper::PrintInfo("Go to 'Manage Rooms' to add your first room.");
            return;
        }

        // Summary header
        int totalDevices = 0;
        int onDevices = 0;
        for (Room* room : system->getRooms()) {
            for (auto& [serial, device] : room->getDevices()) {
                totalDevices++;
                if (device->getState() == "On" || device->getState() == "Opened") {
                    onDevices++;
                }
            }
        }
        
        cout << "\n";
        cout << "  " << UIHelper::BOLD << UIHelper::WHITE << "System Overview" << UIHelper::RESET << "\n";
        cout << "  " << UIHelper::GRAY << "Rooms: " << UIHelper::RESET
             << UIHelper::BOLD << UIHelper::CYAN << system->getRooms().size() 
             << UIHelper::RESET << UIHelper::GRAY << "/" << system->getMaxCapacityRooms() << UIHelper::RESET;
        cout << "    " << UIHelper::GRAY << "Devices: " << UIHelper::RESET
             << UIHelper::BOLD << UIHelper::CYAN << totalDevices << UIHelper::RESET;
        cout << "    " << UIHelper::GRAY << "Active: " << UIHelper::RESET
             << UIHelper::BOLD << UIHelper::GREEN << onDevices << UIHelper::RESET << "\n";
        
        UIHelper::DrawMenuDivider();

        for (Room* room : system->getRooms()) {
            cout << "\n";
            // Room header with capacity bar
            cout << "  " << UIHelper::ICON_DOOR << " " << UIHelper::BOLD << UIHelper::TEAL 
                 << room->getName() << UIHelper::RESET << "  ";
            UIHelper::PrintCapacityBar(room->getDeviceSize(), room->getMaxDeviceCapacity(), 15);
            cout << "\n";

            if (room->getDevices().empty()) {
                cout << "     " << UIHelper::DIM << UIHelper::GRAY << "(No devices in this room)" << UIHelper::RESET << "\n";
            } else {
                for (auto& [serial, device] : room->getDevices()) {
                    string icon = UIHelper::GetDeviceIcon(device->getType());
                    cout << "     " << icon << " " 
                         << UIHelper::WHITE << device->getName() << UIHelper::RESET
                         << UIHelper::GRAY << " (" << device->getType() << ")" << UIHelper::RESET
                         << "  " << UIHelper::GRAY << UIHelper::BOX_VT << UIHelper::RESET << "  ";
                    UIHelper::PrintDeviceState(device->getState());
                    cout << "\n";
                }
            }
        }
        cout << "\n";
    }

    static void _ShowManageRoomsScreen(SmartHomeSystem* system) {
        RoomsScreen::Show(system);
    }

    static void _ShowManageDevicesScreen(SmartHomeSystem* system) {
        DevicesScreen::Show(system);
    }

    static void _PerformExitHomeMode(SmartHomeSystem* system) {
        UIHelper::DrawScreenHeader("Exit Home Mode", UIHelper::ICON_POWER);
        
        if (system->getRooms().empty()) {
            UIHelper::PrintWarning("No rooms or devices to turn off.");
            return;
        }

        cout << "\n  " << UIHelper::YELLOW << "Activating Exit Home Mode..." << UIHelper::RESET << "\n";
        system->exitMode();
        cout << "\n";
        
        for (Room* room : system->getRooms()) {
            cout << "  " << UIHelper::GREEN << UIHelper::ICON_CHECK << UIHelper::RESET
                 << UIHelper::GRAY << " Room: " << UIHelper::RESET 
                 << UIHelper::WHITE << room->getName() << UIHelper::RESET
                 << UIHelper::GRAY << " - All devices secured" << UIHelper::RESET << "\n";
        }
        
        UIHelper::PrintSuccess("All devices have been turned off or locked.");
    }

    static void _PerformOption(enMainMenuOptions option, SmartHomeSystem* homeSystem) {
        switch (option) {
            case eShowStatus:
                ::system("cls");
                _ShowStatusScreen(homeSystem);
                UIHelper::PauseScreen("Press Enter to return to Main Menu...");
                break;
            case eManageRooms:
                ::system("cls");
                _ShowManageRoomsScreen(homeSystem);
                break;
            case eManageDevices:
                ::system("cls");
                _ShowManageDevicesScreen(homeSystem);
                break;
            case eExitHomeMode:
                ::system("cls");
                _PerformExitHomeMode(homeSystem);
                UIHelper::PauseScreen("Press Enter to return to Main Menu...");
                break;
            case eQuit:
                break;
        }
    }

public:
    static void Show(SmartHomeSystem* homeSystem) {
        bool quit = false;
        while (!quit) {
            ::system("cls");
            UIHelper::DrawWelcomeBanner(homeSystem->getName());

            cout << "\n";
            UIHelper::DrawMenuItem(1, "Show Home Status", UIHelper::ICON_INFO);
            UIHelper::DrawMenuItem(2, "Manage Rooms", UIHelper::ICON_DOOR);
            UIHelper::DrawMenuItem(3, "Manage Devices", UIHelper::ICON_GEAR);
            UIHelper::DrawMenuItem(4, "Exit Home Mode", UIHelper::ICON_POWER);
            UIHelper::DrawMenuDivider();
            UIHelper::DrawMenuItem(5, "Quit", UIHelper::ICON_CROSS);
            cout << "\n";

            int choice = UIHelper::ReadInt("Choose an option [1-5]: ");
            
            if (choice >= 1 && choice <= 5) {
                enMainMenuOptions option = (enMainMenuOptions)choice;
                if (option == eQuit) {
                    ::system("cls");
                    cout << "\n\n";
                    cout << "  " << UIHelper::CYAN << UIHelper::ICON_HOME << "  " 
                         << UIHelper::BOLD << "Thank you for using Smart Home System!" 
                         << UIHelper::RESET << "\n";
                    cout << "  " << UIHelper::DIM << UIHelper::GRAY 
                         << "All systems shutting down safely..." 
                         << UIHelper::RESET << "\n\n";
                    quit = true;
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
