## Registry MRU Cleanup (`RemoveDllFromRegistry`)

This function scans a user registry MRU key and removes entries that contain a specific DLL name (`dll1.dll`).

### What it does

- Opens:  
  `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\ComDlg32\OpenSavePidlMRU\dll`
- Enumerates all values in the key.
- Checks value data for `dll1.dll` in:
  - `REG_SZ` (string values via substring search),
  - `REG_BINARY` (raw bytes via memory comparison).
- Deletes matching values.
- Handles index shifting correctly after deletion (does not increment index when delete succeeds).

### Notes

- Works only on Windows (`windows.h` / WinAPI).
- Requires permission to read/write the target registry key.
- Use carefully: deleting MRU entries can affect recent-file history behavior.
- Always back up the registry before testing on a real machine.
