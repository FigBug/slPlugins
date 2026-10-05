; Loudness installer (Inno Setup)

#define MyAppName "Loudness"
#define MyAppCompany "SocaLabs"
#define MyAppPublisher "SocaLabs"
#define MyAppCopyright "2026 SocaLabs"
#define MyAppURL "https://socalabs.com/"
#define MyAppVersion GetStringFileInfo("bin\VST3\Loudness.vst3\Contents\x86_64-win\Loudness.vst3", "ProductVersion")
#define MyDefaultDirName "{commoncf64}\VST3"

[Setup]
AppID={{B5BDCB4C-887E-4C7E-9386-F141E7F89CB6}
AppName={#MyAppCompany} {#MyAppName} {#MyAppVersion}
AppVerName={#MyAppCompany} {#MyAppName} {#MyAppVersion}
AppVersion={#MyAppVersion}
AppCopyright={#MyAppCopyright}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={#MyDefaultDirName}
DisableProgramGroupPage=yes
OutputDir=.\bin
OutputBaseFilename=Loudness
Compression=lzma/ultra
SolidCompression=true
ShowLanguageDialog=auto
LicenseFile=..\EULA.rtf
InternalCompressLevel=ultra
MinVersion=0,6.1.7600
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
DisableDirPage=yes
DisableWelcomePage=no
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoCopyright={#MyAppCopyright}
VersionInfoProductName={#MyAppCompany} {#MyAppName} {#MyAppVersion} (64-bit)
VersionInfoProductVersion={#MyAppVersion}
Uninstallable=no
PrivilegesRequired=admin

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Components]
Name: "vst";  Description: "VST plug-in";  Types: full custom; Flags: checkablealone
Name: "vst3"; Description: "VST3 plug-in"; Types: full custom; Flags: checkablealone
Name: "clap"; Description: "CLAP plug-in"; Types: full custom; Flags: checkablealone


Name: "crashreporter"; Description: "Crash reporter (shared component, only updated if newer)"; Types: full custom; Flags: checkablealone

[InstallDelete]
Type: files;          Name: "{commoncf64}\VST2\Loudness.dll";   Components: vst
Type: filesandordirs; Name: "{commoncf64}\VST3\Loudness.vst3"; Components: vst3
Type: files;          Name: "{commoncf64}\CLAP\Loudness.clap"; Components: clap


[Files]
Source: "bin\VST\Loudness.dll";    DestDir: "{commoncf64}\VST2";                Flags: ignoreversion overwritereadonly; Components: vst
Source: "bin\VST3\Loudness.vst3\*"; DestDir: "{commoncf64}\VST3\Loudness.vst3\"; Flags: ignoreversion overwritereadonly recursesubdirs; Components: vst3
Source: "bin\CLAP\Loudness.clap";   DestDir: "{commoncf64}\CLAP";              Flags: ignoreversion overwritereadonly; Components: clap
; CrashReporter (shared): app only updated if newer + never removed; JSON always installed, never removed.
Source: "bin\CrashReporter\CrashReporter.exe"; DestDir: "{commonpf}\Rabien Software\Crash Reporter"; Flags: skipifsourcedoesntexist uninsneveruninstall; Components: crashreporter
Source: "bin\CrashReporter\loudness.json";   DestDir: "{commonappdata}\Rabien Software\Crash Reporter\Plugins"; Flags: ignoreversion uninsneveruninstall

