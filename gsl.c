#include "gsl.h"
#include <string.h>

static HWND g_hwnd = NULL;
static HDC g_hdc = NULL;
static HDC g_memdc = NULL;
static HBITMAP g_bitmap = NULL;
static int g_width = 0;
static int g_height = 0;
static int g_running = 1;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        g_running = 0;
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (g_memdc) BitBlt(hdc, 0, 0, g_width, g_height, g_memdc, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int gsl_init(const char* name, int width, int height) {
    g_width = width;
    g_height = height;
    HINSTANCE hInstance = GetModuleHandleA(NULL);

    WNDCLASSEXA wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "GSLClass";
    RegisterClassExA(&wc);

    g_hwnd = CreateWindowExA(0, "GSLClass", name, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, NULL);
    if (!g_hwnd) return 0;

    g_hdc = GetDC(g_hwnd);
    g_memdc = CreateCompatibleDC(g_hdc);
    g_bitmap = CreateCompatibleBitmap(g_hdc, width, height);
    SelectObject(g_memdc, g_bitmap);

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);
    return 1;
}

void gsl_shutdown(void) {
    if (g_bitmap) DeleteObject(g_bitmap);
    if (g_memdc) DeleteDC(g_memdc);
    if (g_hdc) ReleaseDC(g_hwnd, g_hdc);
    if (g_hwnd) DestroyWindow(g_hwnd);
}

void gsl_fill(int color) {
    HBRUSH brush = CreateSolidBrush(color);
    RECT rect = { 0, 0, g_width, g_height };
    FillRect(g_memdc, &rect, brush);
    DeleteObject(brush);
    InvalidateRect(g_hwnd, NULL, FALSE);
}

void gsl_draw_text(int x, int y, const char* text, int color) {
    SetTextColor(g_memdc, color);
    SetBkMode(g_memdc, TRANSPARENT);
    TextOutA(g_memdc, x, y, text, strlen(text));
    InvalidateRect(g_hwnd, NULL, FALSE);
}

int gsl_handle_events(void) {
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) g_running = 0;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return g_running;
}
