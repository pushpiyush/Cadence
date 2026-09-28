#include <stdio.h>
#include <time.h>
#include <windows.h>

void typing(void);
void realTimeSpeed();

// Global pointer to our thread pool timer
PTP_TIMER g_DebounceTimer = NULL;
int key_count = 0;
time_t start_time;

// calculate wpm
VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer) {
    time_t end_time = time(NULL);

    // time spent in seconds
    double time = difftime(end_time, start_time) - 5;
    
    // wpm = (total characters)/5/(time in minutes)
    int wpm = ((double) key_count/5)/(time/60);

    printf("wpm: %d\n", wpm);

    // reset variables
    key_count = 0;
}

LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam ) {
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    PKBDLLHOOKSTRUCT pKeyStruct = (PKBDLLHOOKSTRUCT)lParam;

    if (wParam == WM_KEYDOWN) {
        typing();
    }
    
    // pass the control to next program
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main()
{
    printf("Program's started.\n");

    // add the wh_keyboard_ll hook to os
    HHOOK keyboardHook = SetWindowsHookExA(
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        GetModuleHandleA(NULL),
        0);

    // if the hook attachment fails
    if (keyboardHook == NULL)
    {
        printf("Failed to attach keyboad hook! %lu\n", GetLastError());
        return 1;
    }
    else
    {
        printf("Attached keyboard hook!\n");
        g_DebounceTimer = CreateThreadpoolTimer(TimerCallback, NULL, NULL);
        if (g_DebounceTimer == NULL) {
            printf("Failed to create timer. Error: %lu\n.", GetLastError());
            UnhookWindowsHookEx(keyboardHook);
            return 1;
        }
    }

    // message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // remove the hook from os at the end of program
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}


// every time a key is pressed, this function runs
void typing() {
    // restart the countdown timer
    ULARGE_INTEGER dueTime;
    dueTime.QuadPart = (ULONGLONG)-(50000000LL); 

    FILETIME ftDueTime;
    ftDueTime.dwLowDateTime = dueTime.LowPart;
    ftDueTime.dwHighDateTime = dueTime.HighPart;

    SetThreadpoolTimer(g_DebounceTimer, &ftDueTime, 0, 0);

    // if it's the first key, set the start_time
    if (key_count == 0) {
        time(&start_time);
    }

    // increase characters count by 1
    key_count += 1;

    realTimeSpeed();
}

void realTimeSpeed() {
    time_t end_time = time(NULL);
    
    // time spent in seconds
    double time = difftime(end_time, start_time);
    
    // wpm = (total characters)/5/(time in minutes)
    int wpm = ((double) key_count/5)/(time/60);
    
    printf("key_count: %d | ", key_count);
    printf("time: %f | ", time);
    printf("live wpm: %d\n", wpm);
}