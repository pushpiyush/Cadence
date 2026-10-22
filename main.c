#include <stdio.h>
#include <time.h>
#include <windows.h>

VOID CALLBACK TypingWorkerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context);
void realTimeSpeed();
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam );
VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer);

// Global pointer to the thread pool timer
PTP_TIMER g_DebounceTimer = NULL;
volatile LONG key_count = 0;
time_t start_time = 0;


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

    // if the hook attachment succed
    printf("Attached keyboard hook!\n");
    
    // creates the timer object which calls TimerCallback
    g_DebounceTimer = CreateThreadpoolTimer(TimerCallback, NULL, NULL);

    // if the timer object object is not created
    if (g_DebounceTimer == NULL) {
        printf("Failed to create timer. Error: %lu\n.", GetLastError());
        UnhookWindowsHookEx(keyboardHook);
        return 1;
    }

    // message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // release the timer object
    if (g_DebounceTimer) {
        SetThreadpoolTimer(g_DebounceTimer, NULL, 0, 0);
        CloseThreadpoolTimer(g_DebounceTimer);
    }

    // remove the hook from os
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}

// function to run every time a key is pressed
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam ) {
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN) {
        TrySubmitThreadpoolCallback(TypingWorkerCallback, NULL, NULL);
    }
    
    // pass the control to next program
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// calculate final wpm after typing pause (debounce)
VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer) {
    time_t end_time = time(NULL);
    double elapsed_time = difftime(end_time, start_time) - 5; // subtract the 5 seconds debounce delay

    if (elapsed_time <=0 ) elapsed_time = 1; // safegaurd against negative or 0 time
    double elapsed_minutes = elapsed_time / 60.0;             // time spent in minutes
    
    // wpm = (total_characters/5)/(time_elapsed in minutes)
    int wpm = (int) (((double) key_count / 5.0) / elapsed_minutes);

    printf("\n--- Session Finished ---\n");
    printf("key_count: %d | time: %f | live wpm: %d\n", key_count, elapsed_time, wpm);

    // Reset variables for the next session
    InterlockedExchange(&key_count, 0);
    start_time = 0;
}

VOID CALLBACK TypingWorkerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {
    // restart the countdown timer (5 seconds = 50,000,000 100-nanosecond intervals)
    ULARGE_INTEGER dueTime;
    dueTime.QuadPart = (ULONGLONG)-(50000000LL); 
    
    FILETIME ftDueTime;
    ftDueTime.dwLowDateTime = dueTime.LowPart;
    ftDueTime.dwHighDateTime = dueTime.HighPart;
    
    // reset the clock
    SetThreadpoolTimer(g_DebounceTimer, &ftDueTime, 0, 0);
    
    // if it's the first key, set the start_time to current time
    if (key_count == 0) {
        time(&start_time);
    }
    
    
    // increment character count
    InterlockedIncrement(&key_count);
    
    realTimeSpeed();
}

void realTimeSpeed() {
    time_t end_time = time(NULL);
    
    // time spent in seconds
    double elapsed_time = difftime(end_time, start_time);

    if (elapsed_time <= 0) return;
    
    // wpm = (total characters)/5/(time in minutes)
    int wpm = (int) (((double) key_count / 5)/(elapsed_time/60));
    
    printf("key_count: %d | ", key_count);
    printf("time: %f | ", elapsed_time);
    printf("live wpm: %d\n", wpm);
}

// using print inside the callback brings lag
// Calling printf directly inside realTimeSpeed() on every single keystroke blocks the hook
//  and can cause noticeable typing lag or cause Windows to forcefully unhook your procedure.