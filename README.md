# InputLag Killer
InputLag Killer is a free PC optimization application specifically designed to improve gaming performance. It automatically disables unnecessary system services and background processes that can negatively affect input response time and frame rates. Thanks to these optimizations, the application helps reduce input lag and increase system responsiveness, providing a smoother and more enjoyable gaming experience.

# Usage
1) Download the latest version of InputLag Killer from the [Releases](https://github.com/thx4drugs/inputlagkiller/releases) page on GitHub.
2) Run the application as Administrator:
Right-click the executable file and select "Run as administrator".
3) Upon launch, InputLag Killer will automatically disable unnecessary services and background applications, apply system tweaks, and optimize your PC for gaming performance.
4) Wait a few moments while changes are applied. There is no additional user input required.
5) Restart your computer if prompted, to ensure all changes take effect.
6) Enjoy improved system responsiveness and reduced input lag while gaming.
> [!NOTE]
> Note: Running the program without administrator privileges will prevent it from applying system-level tweaks and optimizations.

# Tweaks applied by the application
* Disables the High Precision Event Timer (HPET), which is a hardware timer used by Windows for precise timing. Disabling HPET may reduce latency and improve game or system performance in some cases, though it can also introduce instability in others.
```
bcdedit /deletevalue useplatformclock
bcdedit /set useplatformclock false
bcdedit /set disabledynamictick yes
```
* Likely disables certain performance or system counters (e.g., CPU counters) that track events or metrics. This might be used to reduce overhead or prevent certain monitoring features.
* Disables the Superfetch service in Windows, which preloads frequently used applications into memory to speed up startup. Disabling it may reduce disk usage on slower drives or improve performance in some scenarios.
```
sc stop SysMain
sc config SysMain start= disabled
```
* Probably disables hardware or software acceleration features in Windows, such as GPU acceleration or hardware acceleration for rendering, to troubleshoot or stabilize system performance.
* Clears the temporary files folder on the system, freeing up disk space and potentially improving system responsiveness.
* Disables Cortana, the Windows digital assistant, to reduce background resource usage or for privacy reasons.
* Disables system diagnostic services or telemetry data collection to reduce background activity or enhance privacy.
```
schtasks /change /tn \"Microsoft\\Windows\\MemoryDiagnostic\\ProcessMemoryDiagnosticEvents\"
schtasks /change /tn \"Microsoft\\Windows\\MemoryDiagnostic\\RunFullMemoryDiagnostic\"
```
* Uninstalls Microsoft OneDrive from the system, removing the cloud storage integration.
```
taskkill /f /im OneDrive.exe
%systemroot%\\SysWOW64\\OneDriveSetup.exe /uninstall
rd /s /q %userprofile%\\OneDrive
rd /s /q %userprofile%\\AppData\\Local\\Microsoft\\OneDrive
rd /s /q \"%allusersprofile%\\Microsoft OneDrive\"
```
* Disables Quick Access in File Explorer, which shows frequently used folders and recent files, likely to improve privacy or simplify the interface.
* Enables the full system tray (notification area) display, possibly showing all icons instead of hiding some.
* Disables system notifications, preventing popup alerts and notifications from apps or the OS.
* Disables Windows telemetry, stopping the collection and transmission of diagnostic and usage data to Microsoft. This helps reduce background data traffic, enhance privacy, and lower the load from telemetry-related services.
```
reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Device Metadata" /v PreventDeviceMetadataFromNetwork /t REG_DWORD /d 1 /f
reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\DataCollection" /v AllowTelemetry /t REG_DWORD /d 0 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Policies\DataCollection" /v AllowTelemetry /t REG_DWORD /d 0 /f
reg add "HKLM\SOFTWARE\Policies\Microsoft\MRT" /v DontOfferThroughWUAU /t REG_DWORD /d 1 /f
reg add "HKLM\SOFTWARE\Policies\Microsoft\SQMClient\Windows" /v CEIPEnable /t REG_DWORD /d 0 /f
reg add "HKLM\Software\Microsoft\SQMClient\Windows" /v CEIPEnable /d 0 /t REG_DWORD /f
reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\DataCollection" /v AllowTelemetry /t REG_DWORD /d 0 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\AutoLogger-Diagtrack-Listener" /v Start /t REG_DWORD /d 0 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\WMI\AutoLogger\SQMLogger" /v Start /t REG_DWORD /d 0 /f
```
* Disables Remote Desktop Protocol (RDP) and Remote Assistance services to prevent remote access to the system, improving security and reducing background processes related to remote connections.
```
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Remote Assistance" /v fAllowFullControl /t REG_DWORD /d 0 /f
reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v AllowCortana /t REG_DWORD /d 0 /f
```
* Disables Windows search features that query the internet (such as Bing Search integration and web search in the Start menu). This reduces network traffic, speeds up local search, and prevents sending search queries to Microsoft servers.
```
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v AllowCortana /t REG_DWORD /d 0 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v AllowSearchToUseLocation /t REG_DWORD /d 0 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v ConnectedSearchUseWeb /t REG_DWORD /d 0 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v DisableWebSearch /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v PreventIndexingOutlook /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v PreventIndexingEmailAttachments /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Windows Search" /v AutoIndexSharedFolders /t REG_DWORD /d 1 /f
reg add "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Search" /v BingSearchEnabled /t REG_DWORD /d 0 /f
reg add "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Search" /v CortanaConsent /t REG_DWORD /d 0 /f
```
* Prevents applications from running or updating in the background, reducing CPU, memory, and network resource consumption, which improves overall system performance.
```
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\AppPrivacy" /v LetAppsRunInBackground /t REG_DWORD /d 2 /f
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\BackgroundAccessApplications" /v GlobalUserDisabled /t REG_DWORD /d 1 /f
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Search" /v BackgroundAppGlobalToggle /t REG_DWORD /d 0 /f
```
* Disables certain Microsoft Edge browser features including automatic startup, favorites synchronization, tab preloading, and data collection. This reduces browser-related background activity and lowers system resource usage.
```
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\MicrosoftEdge\Main" /v SyncFavoritesBetweenIEAndMicrosoftEdge /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\MicrosoftEdge\Main" /v PreventLiveTileDataCollection /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\MicrosoftEdge\Main" /v AllowPrelaunch /t REG_DWORD /d 0 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\MicrosoftEdge\TabPreloader" /v PreventTabPreloading /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\MicrosoftEdge\TabPreloader" /v AllowTabPreloading /t REG_DWORD /d 0 /f
```
* Resets Windows network settings by flushing the DNS cache, resetting IP parameters, resetting firewall settings. This helps fix common network issues and frees resources for optimal network performance.
```
ipconfig /flushdns
netsh int ip reset reset.log
netsh advfirewall reset
```

# Benefits
* Reduced Input Lag: Minimizes delays between your hardware input and on-screen reactions, providing a more responsive gaming experience.
* Improved Frame Rates: Frees up system resources by disabling unnecessary background services, potentially increasing FPS in games.
* Faster System Responsiveness: Disabling resource-heavy services and clearing temporary files helps your PC run smoother overall.
* Enhanced Privacy: By turning off telemetry, diagnostic services, and Cortana, the program reduces background data collection.
* Simplified User Interface: Removes distractions like notifications and quick access to help maintain focus during gameplay.
* Lightweight and Free: Does not require heavy installation or additional paid licenses — straightforward and accessible for any gamer.

## License
[GNU GPL v2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
