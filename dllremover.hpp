#pragma once
#include <windows.h>
#include <cstring>   // voor strstr, memcmp

inline void RemoveDllFromRegistry()
{
    HKEY hKey;
    LPCSTR subKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU\\dll";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, subKey, 0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return; // key niet gevonden of geen rechten
    }

    DWORD index = 0;
    char valueName[256];
    DWORD valueNameSize;
    BYTE data[1024];
    DWORD dataSize;
    DWORD type;

    while (true) {
        valueNameSize = sizeof(valueName);
        dataSize = sizeof(data);

        LONG res = RegEnumValueA(
            hKey,
            index,
            valueName,
            &valueNameSize,
            NULL,
            &type,
            data,
            &dataSize
        );

        if (res == ERROR_NO_MORE_ITEMS) break;
        if (res != ERROR_SUCCESS) break;

        bool match = false;

        if (type == REG_SZ) {
            if (strstr(reinterpret_cast<const char*>(data), "dll1.dll") != NULL) {
                match = true;
            }
        }
        else if (type == REG_BINARY) {
            for (DWORD i = 0; i + 8 < dataSize; i++) {
                if (memcmp(data + i, "dll1.dll", 8) == 0) {
                    match = true;
                    break;
                }
            }
        }

        if (match) {
            if (RegDeleteValueA(hKey, valueName) == ERROR_SUCCESS) {
                continue; // alles schuift op → dus niet index++
            }
        }

        index++;
    }

    RegCloseKey(hKey);
}
