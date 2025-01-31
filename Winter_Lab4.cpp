#include <windows.h>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

struct Snowflake {
    float x;
    float y;
    char symbol;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SnowOverlay";
    RegisterClassEx(&wc);

    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create window with layered and transparent style
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        L"SnowOverlay", L"Snow",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL
    );

    // Make the window transparent and clickthrough
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    MARGINS margins = {-1};
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Initialize snowflakes
    std::vector<Snowflake> snowflakes;
    const int NUM_SNOWFLAKES = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(0, screenWidth);
    
    for (int i = 0; i < NUM_SNOWFLAKES; i++) {
        snowflakes.push_back({disX(gen), static_cast<float>(rand() % screenHeight), '*'});
    }

    // Main loop
    const float FALL_SPEED = 0.5f;
    HDC hdc = GetDC(hwnd);
    HBRUSH snowBrush = CreateSolidBrush(RGB(255, 255, 255));

    while (true) {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Clear window
        RECT rect = {0, 0, screenWidth, screenHeight};
        FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Update and draw snowflakes
        for (auto& flake : snowflakes) {
            SetPixel(hdc, (int)flake.x, (int)flake.y, RGB(255, 255, 255));
            
            flake.y += FALL_SPEED;
            if (flake.y >= screenHeight) {
                flake.y = 0;
                flake.x = disX(gen);
            }
        }

        Sleep(1000/120); // 120 FPS
    }

    DeleteObject(snowBrush);
    ReleaseDC(hwnd, hdc);
    return 0;
}