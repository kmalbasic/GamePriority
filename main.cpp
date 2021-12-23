#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include "helper.hpp"

int main(int argc, char* argv[])
{
    SetConsoleTitle(L"GamePriority - currently inactive");

    int default_priority = NORMAL; // in case we don't specify the priority
    std::string game = "";
    int desired_priority = 0; // unset at the start

    std::cout << "GamePriority - alpha build [in development - report bugs at github/kmalbasic]" << std::endl << std::endl;
    std::cout << "NOTE: Do not use this program on any program that doesn't allow opening handles to alter priorities (anticheats, antivirus's)" << std::endl << std::endl;

    if (argc > 1) {
        game = argv[2];
        desired_priority = atoi(argv[3]);
        std::cout << "Game process -> " << game << std::endl;
        std::cout << "Desired priority -> " << desired_priority << std::endl;
    }
    else {
    start:
        std::cout << "Please input the game executable name (example: RocketLeague.exe): ";
        std::cin >> game; // process name found in task manager

        std::cout << "Specify priority (1 - LOW, 2 - BELOW NORMAL, 3 - NORMAL, 4 - HIGH, 5 - REALTIME):";
        std::cin >> desired_priority; // input the prio
    }

    switch (desired_priority) {
    case 1:
        desired_priority = LOW;
        break;
    case 2:
        desired_priority = BELOW_NORMAL;
        break;
    case 3:
        desired_priority = NORMAL;
        break;
    case 4:
        desired_priority = HIGH;
        break;
    case 5:
        desired_priority = REALTIME;
        break;
    default:
        desired_priority = default_priority;
        break;
    };

    std::wstring game_wstr = std::wstring(game.begin(), game.end()); // str to lpwstr

    HANDLE game_handle = get_game_handle(&game_wstr[0]); // store the game handle

    int prio_loop = 0;
    bool visible = false;
    std::cout << std::endl << "Program will now start correcting priorities to increase performance." << std::endl;
    std::cout << "Press HOME to hide/show this window." << std::endl;
    SetConsoleTitle(L"GamePriority - currently active");
    while (true) {
        if (GetAsyncKeyState(VK_HOME) && visible) {
            visible = false;
            ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        }
        else if (GetAsyncKeyState(VK_HOME) && !visible) {
            visible = true;
            ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        }
        if (!is_game_running(&game_wstr[0])) {              // if the game is not running, reset all other priorities back to normal 
            std::cout << "Game has been closed. Reverting changes..." << std::endl << std::endl;
            SetConsoleTitle(L"GamePriority - currently inactive");
            set_rest_priorities(NORMAL, &game_wstr[0]);
            break;
        }

        prio_loop = get_priority(game_handle);              // get priority as usual

        if (prio_loop != desired_priority)                  // if the game priority changed, set it to specified one
            set_priority(desired_priority, game_handle);

        set_rest_priorities(LOW, &game_wstr[0]);            // if the game is running, set all other priorities to low/idle

        Sleep(1);                                           // no need to rape our performance
    }

    goto start;

}