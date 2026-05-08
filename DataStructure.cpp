#include <iostream>
#include "SmartHome/Manager/SmartHomeSystem.h"
#include "SmartHome/UI/MainMenuScreen.h"

using namespace std;

int main() {
    // Enable UTF-8 and ANSI color support
    UIHelper::EnableUTF8();

    // 1. Initialize the system
    SmartHomeSystem* myHome = new SmartHomeSystem("My Smart Home", 10);

    // 2. Start the UI
    MainMenuScreen::Show(myHome);

    // 3. Cleanup - free room and device memory
    for (Room* room : myHome->getRooms()) {
        for (auto& [serial, device] : room->getDevices()) {
            delete device;
        }
        delete room;
    }
    delete myHome;

    return 0;
}