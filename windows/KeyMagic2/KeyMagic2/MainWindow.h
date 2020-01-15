#pragma once

#include "stdafx.h"
#include <cstdint>
#include <cstdio>
#include <new>

#include "resource.h"
#include "Constants.h"
#include "KeyboardManager.h"

struct MainWindow {
    const HWND hWnd;

    const int kRightColumnWidth = 150;
    const int kRightColumnPadding = 10;
    const int kListViewMargin = 10;
    const int kButtonWidth = kRightColumnWidth - kRightColumnPadding * 2;
    const int kButtonHeight = 30;
private:
    static WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
    BOOL updateVersionNotificationWasShown = FALSE;
    HFONT bodyFont = NULL;
    
    explicit MainWindow(HWND hWnd)
        : hWnd(hWnd) {};
public:
    static LPCTSTR Initialize(HINSTANCE hInstance) {
        LoadStringW(hInstance, IDC_KEYMAGIC2, szWindowClass, MAX_LOADSTRING);

        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = Procedure;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC2));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KEYMAGIC2);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC2));

        return (LPCTSTR)(std::intptr_t) RegisterClassEx(&wcex);
    }
private:
    HFONT GetFont();
    HINSTANCE GetInstance();
    void OpenGithubReleasePage();
    void ShowTrayContextMenu();

    LRESULT Dispatch(UINT message, WPARAM wParam, LPARAM lParam);

    void OnKeyboardDidChange(bool);
    void CheckForUpdateAndNotify();
    void CheckForUpdateDialogEnabled();

    void Layout();

    BOOL InsertListViewItems(HWND hWndListView, Keyboard keyboard);
    BOOL InitListViewColumns(HWND hWndListView);
    BOOL CreateListView();
    HWND CreateButton(LPTSTR title, HMENU buttonId);
    HWND CreateAddKeyboardButton();
    HWND CreateRemoveKeyboardButton();
    HWND CreateReportBugButton();
    HWND CreateLabel();
    void SetHotKeyTexts();
    void RegisterKeyboardFile(LPCTSTR fileName);
    void UnregisterKeyboard(Keyboard& keyboard);
    void AddKeyboardFile(LPCTSTR filePath);
    void ReloadKeyboards();
    void ShellExecuteAndWait(LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd);
    void UpdateMenuState();
    void SyncWindowMode();
    void ToggleKeyboardPerWindow();

    // Procedure
    //  - initialization and forwarding to actual procedure (member function)
    //  - if we want to avoid the 'if' we can split this into two procedures and SetWindowLongPtr (..., GWLP_WNDPROC, ...)
    //  - we are also eating exceptions so they don't escape to foreign frames
    //
    static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        try {
            if (auto window = GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
                return reinterpret_cast <MainWindow*> (window)->Dispatch(message, wParam, lParam);

            }
            else {
                switch (message) {
                case WM_NCCREATE:
                    if (auto window = new (std::nothrow) MainWindow(hWnd)) {
                        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
                        return window->Dispatch(WM_NCCREATE, wParam, lParam);
                    }
                    else
                        return FALSE;

                case WM_DESTROY:
                    PostQuitMessage(0);
                    break;
                }
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        catch (...) {
            DestroyWindow(hWnd);
            return 0;
        }
    }
};
