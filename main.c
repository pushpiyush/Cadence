#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK LowLevelKeyboardProc(
    int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    PKBDLLHOOKSTRUCT pKeyStruct = (PKBDLLHOOKSTRUCT)lParam;

    printf("wParam: %p |\n", (void *)wParam);

    printf("vkCode: 0x%lX, scanCode: %lu\n", (unsigned long)pKeyStruct->vkCode, (unsigned long)pKeyStruct->scanCode);
    printf("flags: %lu, time: %lu\n\n", pKeyStruct->flags, pKeyStruct->time);

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    printf("Program's started.\n");

    HHOOK keyboardHook = SetWindowsHookExA(
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        GetModuleHandleA(NULL),
        0);

    if (keyboardHook == NULL)
    {
        printf("Failed to attach keyboad hook! %lu\n", GetLastError());
        return 1;
    }
    else
    {
        printf("Attached keyboard hook!\n");
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}