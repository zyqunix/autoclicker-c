#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"

#define PI 3.14159265
#define RADIUS_X 40
#define RADIUS_Y 20

bool killing = false;
bool clicking = false;

void left_click() {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void move_mouse_circle() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    
    double angle = 0.0;
    while (killing) {
        int newX = cursorPos.x + (int)(RADIUS_X * cos(angle * PI / 180));
        int newY = cursorPos.y + (int)(RADIUS_Y * sin(angle * PI / 180));
        
        SetCursorPos(newX, newY);
        left_click();
        
        angle += 10.0;
        if (angle >= 360.0) {
            angle = 0.0;
        }
        
        Sleep(50);
    }
}

DWORD WINAPI kill_enemy(LPVOID lpParam) {
    move_mouse_circle();
    return 0;
}

void toggle_killing() {
    if (!killing) {
        killing = true;
        CreateThread(NULL, 0, kill_enemy, NULL, 0, NULL);
    } else {
        killing = false;
    }
}

void toggle_clicking() {
    clicking = !clicking;
}

DWORD WINAPI click_continuously(LPVOID lpParam) {
    while (clicking) {
        left_click();
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    HWND hwnd = GetConsoleWindow();
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    printf("Press Shift + Alt + 1 to start/stop mouse circle movement.\n");
    printf("Press Shift + Alt + 2 to start/stop clicking without spinning.\n");
    printf("Press ESC to exit.\n");

    while (true) {
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState('1') & 0x8000) {
            toggle_killing();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState('2') & 0x8000) {
            if (!clicking) {
                clicking = true;
                CreateThread(NULL, 0, click_continuously, NULL, 0, NULL);
            } else {
                clicking = false;
            }
            Sleep(300);
        }       
    }

    return 0;
}