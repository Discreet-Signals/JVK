/*
  ==============================================================================

    HWNDGenerator.h
    Created: 9 Oct 2023 4:46:14pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once
#include <windows.h>

namespace jvk::core::windows
{

class HWNDGenerator
{
public:
    // Constructor and destructor if necessary
    HWNDGenerator() : hwnd(nullptr) { }
    ~HWNDGenerator() { release(); }
    
    bool isValid() { return hwnd != nullptr; }
    HWND& create()
    {
        DBG("Creating HWND...");
        HINSTANCE hInstance = GetModuleHandle(nullptr);
        const char* className = "JVK";
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = className;

        if (!RegisterClassEx(&wc))
            DBG("Failed to Create Window!");

        hwnd = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TRANSPARENT,  // Extended window style (for transparency)
            className,                          // Window class name (or use your custom window class)
            "JVK Window",                       // Window name
            WS_POPUP,                           // Window style (no title bar)
            0, 0, 800, 600,                     // Initial position and size
            nullptr, nullptr,                   // Parent window and menu handle
            GetModuleHandle(nullptr),           // Application instance
            nullptr                             // Additional application data
        );
        DBG("Created HWND!");
        return hwnd;
    }
    void release() 
    { 
        DestroyWindow(hwnd);
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnregisterClass("JVK", GetModuleHandle(nullptr));
    }
private:
    HWND hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        // Handle messages here (like WM_PAINT, WM_DESTROY, etc.)
        switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            // Add more message handling as needed...
        }

        // Default message handling
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

} // jvk::core::macos
