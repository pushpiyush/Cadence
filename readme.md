# Functions of User32 module used:
* **SetWindowsHookExA():** To register the listener with OS especially with WH_KEYBOARD_LL flag.
    ``` 
    HHOOK SetWindowsHookExA(
    [in] int       idHook,
    [in] HOOKPROC  lpfn,
    [in] HINSTANCE hmod,
    [in] DWORD     dwThreadId
    ); 
    ```
    <hr>

* **CallNextHookEx():** To make sure the control is passed to next program.
    ```
    LRESULT CallNextHookEx(
    [in, optional] HHOOK  hhk,
    [in]           int    nCode,
    [in]           WPARAM wParam,
    [in]           LPARAM lParam
    );
    ```
    <hr>

* **UnhookWindowsHookEx():** To turnoff the listener.
    ```
    BOOL UnhookWindowsHookEx(
        [in] HHOOK hhk
    );
    ```

## Also used:
* **KBDLLHOOKSTRUCT:** 
    ```
    typedef struct tagKBDLLHOOKSTRUCT {
        DWORD     vkCode;      // The virtual-key code (e.g., 0x41 for 'A')
        DWORD     scanCode;    // Hardware-dependent scan code
        DWORD     flags;       // Extended-key flags, event-injected flags, etc.
        DWORD     time;        // Timestamp for this message
        ULONG_PTR dwExtraInfo; // Extra information associated with the message
    } KBDLLHOOKSTRUCT, *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;
    ```

* **DATA TYPE:** 
    1. DWORD: `%lu` for decimal unsigned and `%lx` for hexadecimal unsigned.
    2. WPARAM/LPARAM: 

## Virtual Key Codes:

## Windows Message Loop:
```
MSG msg;
while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
```

# For timer
* **CreateThreadpoolTimer():** Creates a timer object and bind it to background callback function.
    ```
    PTP_TIMER CreateThreadpoolTimer(
        [in]                PTP_TIMER_CALLBACK   pfnti,
        [in, out, optional] PVOID                pv,
        [in, optional]      PTP_CALLBACK_ENVIRON pcbe
    );
    ```
    Return: PTP_TIMER structure that defines timer object or NULL if fails.
    <hr>

* **SetThreadpoolTimer():** To reset the clock everytime this function is triggered.
    ```
    VOID SetThreadpoolTimer(
        [in, out]      PTP_TIMER pti,
        [in, optional] PFILETIME pftDueTime,
        [in]           DWORD     msPeriod,
        [in]           DWORD     msWindowLength
        );
    ```
    <hr>

* **CloseThreadpoolTimer():** Releases the specified timer object.
    ```
    VOID CloseThreadpoolTimer(
        [in, out] PTP_TIMER pti
    );
    ```
    Return: None
    <hr>

* **TimerCallback():** The PTP_TIMER_CALLBACK type defines a pointer to this callback function.
    ```
    VOID CALLBACK TimerCallback(
        _Inout_     PTP_CALLBACK_INSTANCE Instance,
        _Inout_opt_ PVOID                 Context,
        _Inout_     PTP_TIMER             Timer
    );
    ```
    Return: None