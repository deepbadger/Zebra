//#ifdef  Q_OS_WIN32


#include "win.h"
#include <QDebug>
volatile long n;


LPTSTR
GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize, DWORD res)
{
    DWORD dwRet;
    LPTSTR lpszTemp = NULL;

    dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                          | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, res, // GetLastError(),
                          LANG_NEUTRAL, (LPTSTR) &lpszTemp, 0, NULL);

    // supplied buffer is not long enough
    if (!dwRet || ((long) dwSize < (long) dwRet + 14))
        lpszBuf[0] = TEXT ('\0');
    else
    {
        lpszTemp[lstrlen(lpszTemp) - 2] = TEXT ('\0'); //remove cr and newline character
        wsprintf(lpszBuf, TEXT ("%s (0x%x)"), lpszTemp, GetLastError());
    }

    if (lpszTemp)
        LocalFree((HLOCAL) lpszTemp);

    return lpszBuf;
}

LPTSTR
GetLastErrorQText(QString * QBuf, DWORD res)
{
    DWORD dwRet;
    LPTSTR lpszTemp = NULL;

    TCHAR lpszBuf[300];
    DWORD dwSize = 300;

    dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                          | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, res, // GetLastError(),
                          LANG_NEUTRAL, (LPTSTR) &lpszTemp, 0, NULL);

    // supplied buffer is not long enough
    if (!dwRet || ((long) dwSize < (long) dwRet + 14))
        lpszBuf[0] = TEXT ('\0');
    else
    {
        lpszTemp[lstrlen(lpszTemp) - 2] = TEXT ('\0'); //remove cr and newline character
        wsprintf(lpszBuf, TEXT ("%s (0x%x)"), lpszTemp, GetLastError());
    }

    if (lpszTemp)
        LocalFree((HLOCAL) lpszTemp);

    QBuf->clear();
    *QBuf = QString::fromUtf16((ushort*) lpszBuf);
    return lpszBuf;
}


BOOL CALLBACK EnumChildProc(HWND hWnd,LPARAM lParam)
{
    TCHAR buff[560];
    CHAR buff1[560];



    DWORD pid, wid;
    bool flag = false;

    GetWindowThreadProcessId(hWnd, &pid);


    GetClassName(hWnd, buff, 260);

    if (!wcscmp(buff, L"Chrome_WindowImpl_0"))
    {flag = true;}
    if (!wcscmp(buff, L"Chrome_WidgetWin_0"))
    {flag = true;}
    if (!wcscmp(buff, L"MozillaUIWindowClass"))
    {flag = true;}
    if (!wcscmp(buff, L"MozillaWindowClass"))
    {flag = true;}
    if (!wcscmp(buff, L"IEFrame"))
    {flag = true;}
    if (!wcscmp(buff, L"TabWindowClass"))
    {flag = true;}

    if (flag){
        CharToOem(buff, buff1); // russian text support



        GetWindowText(hWnd, buff, 260);
        if (!wcsncmp(buff, L"Zimbra: ", 8)){


            CharToOem(buff, buff1); // russian text support


            HWND needTopWindow = hWnd;
            BYTE keyState[256] = {0};
            //to unlock SetForegroundWindow we need to imitate Alt pressing
            if(::GetKeyboardState((LPBYTE)&keyState))
            {
                if(!(keyState[VK_MENU] & 0x80))
                {
                    ::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
                }
            }

            ::SetForegroundWindow(needTopWindow);

            if(::GetKeyboardState((LPBYTE)&keyState))
            {
                if(!(keyState[VK_MENU] & 0x80))
                {
                    ::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
                }
            }


            DWORD dwThreadID = GetWindowThreadProcessId(needTopWindow, NULL);
            AttachThreadInput( dwThreadID, GetCurrentThreadId(), true);

            SetWindowPos(needTopWindow, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
            SetWindowPos(needTopWindow, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);


            SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
            SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            PostMessage(needTopWindow, 0x201, 1, 1);
            ShowWindowAsync(hWnd, 3);

            SetForegroundWindow(needTopWindow);
            SetActiveWindow(needTopWindow);
            SetFocus(needTopWindow);


            AttachThreadInput( dwThreadID, GetCurrentThreadId(), false);
            SetForegroundWindow(needTopWindow);
            SendMessage(needTopWindow,WM_SYSCOMMAND,SC_MINIMIZE,0);
            SendMessage(needTopWindow,WM_SYSCOMMAND,SC_RESTORE,0);
            SetForegroundWindow(needTopWindow);
            //BringWindowToTop(needTopWindow);

        }
    }

    if(lParam == 0) EnumChildWindows(hWnd, EnumChildProc, 0);
    return TRUE;    // continue enumeration
}

BOOL CALLBACK EnumChildProc1(HWND hWnd,LPARAM lParam)
{
    if(!IsWindow(hWnd)) return TRUE;

    TCHAR buff[560];
    TCHAR szBuf[560];
    int flag = 0;

    GetClassName(hWnd, buff, 260);
qDebug() <<  "\\\\\\\\\\\\\\\\\ "<<QString().fromWCharArray(buff);

    if (!wcscmp(buff, L"Chrome_WindowImpl_0"))
    {flag = 1;}
    if (!wcscmp(buff, L"Chrome_WindowImpl_1"))
    {flag = 1;}
    if (!wcscmp(buff, L"Chrome_WidgetWin_0"))
    {flag = 1;}
    if (!wcscmp(buff, L"Chrome_WidgetWin_1"))
    {flag = 1;}

    if (!wcscmp(buff, L"MozillaUIWindowClass"))
    {flag = 5;}
    if (!wcscmp(buff, L"MozillaWindowClass"))
    {flag = 5;}
    if (!wcscmp(buff, L"IEFrame"))
    {flag = 3;}
    if (!wcscmp(buff, L"TabWindowClass"))
    {flag = 2;}

    if (flag){
qDebug() <<  QString().fromWCharArray(buff);
        GetWindowText(hWnd, buff, 260);
qDebug() <<  QString().fromWCharArray(buff);
        if (!(wcsstr(buff, L"Zimbra: ") == NULL)){
            qDebug() <<  QString().fromWCharArray(buff);
            Sleep(100);
            SendMessage(hWnd, WM_GETTEXT, 560, (LPARAM)szBuf);

            qDebug() <<  QString().fromWCharArray(szBuf);
            if (!wcsncmp(buff, szBuf, 8)){
                HWND iexpl;
                InterlockedIncrement((long*)&n);
                SetForegroundWindow(hWnd);
                switch(flag){
                case 1: SendMessage(hWnd, 0x201, 0, 0);// Chrome
                    break;
                case 2: iexpl = FindWindow(L"IEFrame",NULL);
                    if(!IsWindow(iexpl)) return TRUE;
                    SetForegroundWindow(iexpl);
                    SendMessage(iexpl, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                    return FALSE;
                    break;
                case 3: SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);// Explorer
                    break;
                case 5: ShowWindow(hWnd, 3);// Firefox
                    break;
                }
                return FALSE;
            }
        }
    }
    if(lParam == 0) EnumChildWindows(hWnd, EnumChildProc1, 1);
    return TRUE;    // continue enumeration
}


long showBrowser()
{
    qDebug() <<"->";
    EnumChildWindows(NULL, EnumChildProc1, 0);
    long goods;
    InterlockedExchange(&goods, n);

    InterlockedExchange((long*) &n, 0);
    qDebug() <<"<-" << goods;
    return goods;
}
//#endif
