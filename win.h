#ifndef WIN_H
#define WIN_H

#include <windows.h>


#include <qlist.h>
#include <qstring.h>
#include <qstringlist.h>



LPTSTR GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize, DWORD res);

LPTSTR GetLastErrorQText(QString * QBuf, DWORD res);

long showBrowser();


#endif // WIN_H
