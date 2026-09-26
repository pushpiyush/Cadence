# Functions of User32 module used:
* **SetWindowsHookExA():** to register the listener with OS especially with WH_KEYBOARD_LL flag.
    ``` 
    HHOOK SetWindowsHookExA(
    [in] int       idHook,
    [in] HOOKPROC  lpfn,
    [in] HINSTANCE hmod,
    [in] DWORD     dwThreadId
    ); 
    ```

* **CallNextHookEx():** To make sure the control is passed to next program.
    ```
    LRESULT CallNextHookEx(
    [in, optional] HHOOK  hhk,
    [in]           int    nCode,
    [in]           WPARAM wParam,
    [in]           LPARAM lParam
    );
    ```

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
    DWORD     vkCode;
    DWORD     scanCode;
    DWORD     flags;
    DWORD     time;
    ULONG_PTR dwExtraInfo;
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