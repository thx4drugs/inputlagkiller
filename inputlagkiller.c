#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>

#define OUTPUT_BUFFER_SIZE 1024
#define CMD_BUFFER_SIZE 512

static int is_guid_at(const char* str, size_t pos) {
    if (!str || strlen(str) < pos + 36) return 0;
    for (size_t i = 0; i < 36; i++) {
        char c = str[pos + i];
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (c != '-') return 0;
        } else {
            if (!isxdigit((unsigned char)c)) return 0;
        }
    }
    return 1;
}

static int find_guid(const char* text, char* guid, size_t guid_size) {
    if (!text || !guid || guid_size < 37) return -1;
    size_t len = strlen(text);
    for (size_t i = 0; i <= len - 36; i++) {
        if (is_guid_at(text, i)) {
            memcpy(guid, text + i, 36);
            guid[36] = '\0';
            return 0;
        }
    }
    return -1;
}

static int executecommand(const char* cmd, char* output, size_t max_len) {
    if (!cmd || !output || max_len == 0) return -1;
    FILE* fp = _popen(cmd, "r");
    if (!fp) return -1;

    size_t total_read = 0;
    while (fgets(output + total_read, (int)(max_len - total_read), fp)) {
        total_read = strlen(output);
        if (total_read >= max_len - 1) break;
    }
    _pclose(fp);
    return 0;
}

static void set_registry_dword(HKEY root, const char* key_path, const char* value_name, DWORD value_data, REGSAM access) {
    HKEY hKey = NULL;
    LONG res = RegCreateKeyExA(root, key_path, 0, NULL, REG_OPTION_NON_VOLATILE, access, NULL, &hKey, NULL);
    if (res == ERROR_SUCCESS) {
        res = RegSetValueExA(hKey, value_name, 0, REG_DWORD, (const BYTE*)&value_data, sizeof(value_data));
        if (res == ERROR_SUCCESS) {
            printf("[*] RegKey %s set to %lu in %s\n", value_name, (unsigned long)value_data, key_path);
        } else {
            fprintf(stderr, "[!] Failed to set RegKey %s in %s (Error: %ld)\n", value_name, key_path, res);
        }
        RegCloseKey(hKey);
    } else {
        fprintf(stderr, "[!] Failed to open/create key %s (Error: %ld)\n", key_path, res);
    }
}

static void disablecounters(void) {
    static const char* services[] = {
        "PerfDisk",
        "PerfOS",
        "PerfProc",
        "PerfNet",
        "PerfSys"
    };
    printf("[*] Disabling performance counters...\n");
    for (size_t i = 0; i < sizeof(services)/sizeof(services[0]); i++) {
        char key_path[256];
        snprintf(key_path, sizeof(key_path), "SYSTEM\\CurrentControlSet\\Services\\%s\\Performance", services[i]);
        set_registry_dword(HKEY_LOCAL_MACHINE, key_path, "DisablePerformanceCounters", 1, KEY_SET_VALUE | KEY_WOW64_64KEY);
    }
    system("lodctr /R >nul 2>&1");
    system("WINMGMT.EXE /RESYNCPERF >nul 2>&1");
    printf("[*] Performance counters disabled successfully\n\n");
}

static void disablesuperfetch(void) {
    printf("[*] Disabling SuperFetch...\n");
    system("sc stop SysMain >nul 2>&1");
    system("sc config SysMain start= disabled >nul 2>&1");
    printf("[*] SuperFetch disabled successfully\n\n");
}

static void disablehpet(void) {
    printf("[*] Disabling HPET...\n");
    system("bcdedit /deletevalue useplatformclock >nul 2>&1");
    system("bcdedit /set useplatformclock false >nul 2>&1");
    system("bcdedit /set disabledynamictick yes >nul 2>&1");
    printf("[*] HPET service disabled successfully\n\n");
}

static void disableacceleration(void) {
    printf("[*] Disabling mouse acceleration...\n");
    HKEY hKey = NULL;
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS && hKey) {
        DWORD zero = 0;
        RegSetValueExA(hKey, "MouseSpeed", 0, REG_SZ, (const BYTE*)"0", 2);
        RegSetValueExA(hKey, "MouseThreshold1", 0, REG_SZ, (const BYTE*)"0", 2);
        RegSetValueExA(hKey, "MouseThreshold2", 0, REG_SZ, (const BYTE*)"0", 2);
        RegCloseKey(hKey);

        system("RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters >nul 2>&1");
        printf("[*] Mouse acceleration disabled successfully\n\n");
    } else {
        fprintf(stderr, "[!] Failed to open mouse registry key\n\n");
    }
}

static void cleartempfolder(void) {
	printf("[*] Clearing the temp folder...\n");
    char temp_path[MAX_PATH] = {0};
    if (!ExpandEnvironmentStringsA("%TEMP%", temp_path, MAX_PATH)) {
        return;
    }

    char cmd[CMD_BUFFER_SIZE];
    snprintf(cmd, sizeof(cmd), "cmd /Q /c @del /f /q \"%s\\*\" >nul 2>&1 & @for /d %%i in (\"%s\\*\") do @rd /s /q \"%%i\" >nul 2>&1", temp_path, temp_path);

    system(cmd);
	printf("[*] Temp folder cleared successfully\n\n");
}

static void disablecortana(void) {
    printf("[*] Disabling Cortana services...\n");
    struct {
        const char* key_path;
        struct {
            const char* value_name;
            DWORD value_data;
        } values[5];
        size_t values_count;
    } registryLocal[] = {
        {
            "SOFTWARE\\Microsoft\\Speech_OneCore\\Preferences",
            {
                {"ModelDownloadAllowed", 0}
            },
            1
        },
        {
            "SOFTWARE\\Policies\\Microsoft\\Windows\\Windows Search",
            {
                {"AllowCloudSearch", 0},
                {"AllowCortana", 0},
                {"AllowSearchToUseLocation", 0},
                {"ConnectedSearchUseWeb", 0},
                {"DisableWebSearch", 1}
            },
            5
        }
    };
    for (size_t i = 0; i < sizeof(registryLocal) / sizeof(registryLocal[0]); i++) {
        HKEY hKey = NULL;
        LONG res = RegCreateKeyExA(HKEY_LOCAL_MACHINE, registryLocal[i].key_path, 0, NULL,
                                   REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKey, NULL);
        if (res == ERROR_SUCCESS && hKey) {
            for (size_t v = 0; v < registryLocal[i].values_count; v++) {
                set_registry_dword(HKEY_LOCAL_MACHINE, registryLocal[i].key_path, registryLocal[i].values[v].value_name,
                                  registryLocal[i].values[v].value_data, KEY_WRITE | KEY_WOW64_64KEY);
            }
            RegCloseKey(hKey);
        } else {
            fprintf(stderr, "[!] Failed to open/create Cortana registry key: %s\n", registryLocal[i].key_path);
        }
    }
    printf("[*] Cortana services disabled successfully\n\n");
}

static void disablediagnostics(void) {
    static const char* tasks[] = {
        "schtasks /change /tn \"Microsoft\\Windows\\MemoryDiagnostic\\ProcessMemoryDiagnosticEvents\" /disable >nul 2>&1",
        "schtasks /change /tn \"Microsoft\\Windows\\MemoryDiagnostic\\RunFullMemoryDiagnostic\" /disable >nul 2>&1"
    };
    printf("[*] Disabling persistent memory diagnostics...\n");
    for (size_t i = 0; i < sizeof(tasks)/sizeof(tasks[0]); ++i) {
        system(tasks[i]);
    }
    printf("[*] Memory diagnostics disabled successfully\n\n");
}

static void uninstallonedrive(void) {
    static const char* commands[] = {
        "taskkill /f /im OneDrive.exe >nul 2>&1",
        "%systemroot%\\SysWOW64\\OneDriveSetup.exe /uninstall >nul 2>&1",
        "rd /s /q %userprofile%\\OneDrive >nul 2>&1",
        "rd /s /q %userprofile%\\AppData\\Local\\Microsoft\\OneDrive >nul 2>&1",
        "rd /s /q \"%allusersprofile%\\Microsoft OneDrive\" >nul 2>&1"
    };
    printf("[*] Disabling OneDrive services...\n");
    for (size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        system(commands[i]);
    }
    printf("[*] OneDrive services disabled successfully\n\n");
}

static void disablequickaccess(void) {
    printf("[*] Disabling quick access history...\n");
    struct {
        const char* key_path;
        struct {
            const char* value_name;
            DWORD value_data;
        } values[2];
        size_t values_count;
    } registryAccess[] = {
        {
            "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",
            {
                {"ShowFrequent", 0},
                {"ShowRecent", 0}
            },
            2
        },
        {
            "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
            {
                {"Start_TrackDocs", 0},
                {"Start_TrackProgs", 0}
            },
            2
        }
    };
    for (size_t i = 0; i < sizeof(registryAccess) / sizeof(registryAccess[0]); i++) {
        HKEY hKey = NULL;
        LONG res = RegCreateKeyExA(HKEY_CURRENT_USER, registryAccess[i].key_path, 0, NULL,
                                   REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
        if (res == ERROR_SUCCESS && hKey) {
            for (size_t v = 0; v < registryAccess[i].values_count; v++) {
                set_registry_dword(HKEY_CURRENT_USER, registryAccess[i].key_path, registryAccess[i].values[v].value_name,
                                  registryAccess[i].values[v].value_data, KEY_SET_VALUE);
            }
            RegCloseKey(hKey);
        } else {
            fprintf(stderr, "[!] Failed to open/create quick access registry key: %s\n", registryAccess[i].key_path);
        }
    }
    printf("[*] Quick access history disabled successfully\n\n");
}

static void enablefulltray(void) {
    printf("[*] Enabling full tray...\n");
    const char* key_path = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer";
    const char* value_name = "EnableAutoTray";
    DWORD value_data = 0;
    set_registry_dword(HKEY_CURRENT_USER, key_path, value_name, value_data, KEY_SET_VALUE);
    printf("[*] Full tray enabled successfully\n\n");
}

static void disablenotification(void) {
    printf("[*] Disabling Notification Center...\n");
    const char* key_path = "Software\\Policies\\Microsoft\\Windows\\Explorer";
    const struct {
        const char* value_name;
        DWORD value_data;
    } values[] = {
        { "DisableNotificationCenter", 1 }
    };

    HKEY hKey = NULL;
    LONG res = RegCreateKeyExA(HKEY_CURRENT_USER, key_path, 0, NULL,
                               REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (res == ERROR_SUCCESS && hKey) {
        for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            res = RegSetValueExA(hKey, values[i].value_name, 0, REG_DWORD, (const BYTE*)&values[i].value_data, sizeof(values[i].value_data));
            if (res == ERROR_SUCCESS) {
                printf("[*] RegKey %s set to %lu\n", values[i].value_name, (unsigned long)values[i].value_data);
            } else {
                fprintf(stderr, "[!] Error setting %s (Error: %ld)\n\n", values[i].value_name, res);
            }
        }
        RegCloseKey(hKey);
        printf("[*] Notification Center disabled successfully\n\n");
    } else {
        fprintf(stderr, "[!] Failed to create/open registry key %s\n\n", key_path);
    }
}

int main(void) {
	SetConsoleTitleA("InputLag Killer");
	printf("InputLag Killer - one-click PC optimization solution\n");
	printf("Github: https://github.com/thx4drugs/inputlagkiller\n\n");
	
    printf("[*] Setting up a power plan...\n");
    char output[OUTPUT_BUFFER_SIZE] = {0};
    const char* duplicate_cmd = "powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61";
    if (executecommand(duplicate_cmd, output, sizeof(output)) != 0) {
        fprintf(stderr, "[!] Failed to execute duplicate power scheme command\n");
        return 1;
    }

    char guid[37] = {0};
    if (find_guid(output, guid, sizeof(guid)) == 0) {
        char cmd1[CMD_BUFFER_SIZE];
        char cmd2[CMD_BUFFER_SIZE];
        snprintf(cmd1, sizeof(cmd1), "powercfg -changename %s \"Ultimate Performance (inputlagkiller)\"", guid);
        snprintf(cmd2, sizeof(cmd2), "powercfg -setactive %s", guid);
        executecommand(cmd1, output, sizeof(output));
        executecommand(cmd2, output, sizeof(output));
        printf("[*] Power plan set to 'Ultimate performance'\n\n");
    } else {
        fprintf(stderr, "[!] An error occurred while changing the power plan\n\n");
    }

    disablehpet();
    disablecounters();
    disablesuperfetch();
    disableacceleration();
    cleartempfolder();
    disablecortana();
    disablediagnostics();
    uninstallonedrive();
    disablequickaccess();
    enablefulltray();
    disablenotification();
	
	printf("PC optimization is complete. You need to restart your PC to apply the tweaks!\n");
	printf("If our utility helped you, please like our Github repository\n\n");
	printf("Press any key to exit...\n");
	
	MessageBoxA(NULL, "Attention! Restart your PC immediately to apply the tweaks!", "InputLag Killer", MB_OK | MB_ICONINFORMATION);
    _getch();

    return 0;
}
