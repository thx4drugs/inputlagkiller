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
> Useful information that users should know, even when skimming content.

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

# Benefits
* Reduced Input Lag: Minimizes delays between your hardware input and on-screen reactions, providing a more responsive gaming experience.
* Improved Frame Rates: Frees up system resources by disabling unnecessary background services, potentially increasing FPS in games.
* Faster System Responsiveness: Disabling resource-heavy services and clearing temporary files helps your PC run smoother overall.
* Enhanced Privacy: By turning off telemetry, diagnostic services, and Cortana, the program reduces background data collection.
* Simplified User Interface: Removes distractions like notifications and quick access to help maintain focus during gameplay.
* Lightweight and Free: Does not require heavy installation or additional paid licenses — straightforward and accessible for any gamer.

## License
[GNU GPL v2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
