; BitCrusher installer (Inno Setup)

#define MyAppName "BitCrusher"
#define MyAppCompany "SocaLabs"
#define MyAppPublisher "SocaLabs"
#define MyAppCopyright "2026 SocaLabs"
#define MyAppURL "https://socalabs.com/"
#define MyAppVersion GetStringFileInfo("bin\VST3\BitCrusher.vst3\Contents\x86_64-win\BitCrusher.vst3", "ProductVersion")
#define MyDefaultDirName "{commoncf64}\VST3"

[Setup]
AppID={{53E03347-89A3-4AB6-8AA4-B13C74BBA297}
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
OutputBaseFilename=BitCrusher
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
Name: "resources"; Description: "Factory presets"; Types: full custom; Flags: fixed

Name: "crashreporter"; Description: "Crash reporter (shared component, only updated if newer)"; Types: full custom; Flags: checkablealone

[InstallDelete]
Type: files;          Name: "{commoncf64}\VST2\BitCrusher.dll";   Components: vst
Type: filesandordirs; Name: "{commoncf64}\VST3\BitCrusher.vst3"; Components: vst3
Type: files;          Name: "{commoncf64}\CLAP\BitCrusher.clap"; Components: clap
Type: filesandordirs; Name: "{commonappdata}\SocaLabs\BitCrusher\Presets";    Components: resources

[Files]
Source: "bin\VST\BitCrusher.dll";    DestDir: "{commoncf64}\VST2";                Flags: ignoreversion overwritereadonly; Components: vst
Source: "bin\VST3\BitCrusher.vst3\*"; DestDir: "{commoncf64}\VST3\BitCrusher.vst3\"; Flags: ignoreversion overwritereadonly recursesubdirs; Components: vst3
Source: "bin\CLAP\BitCrusher.clap";   DestDir: "{commoncf64}\CLAP";              Flags: ignoreversion overwritereadonly; Components: clap
Source: "..\..\plugins\BitCrusher\Resources\*.xml"; DestDir: "{commonappdata}\SocaLabs\BitCrusher\Presets\"; Flags: ignoreversion; Components: resources
; CrashReporter (shared): app only updated if newer + never removed; JSON always installed, never removed.
Source: "bin\CrashReporter\CrashReporter.exe"; DestDir: "{commonpf}\Rabien Software\Crash Reporter"; Flags: skipifsourcedoesntexist uninsneveruninstall; Components: crashreporter
Source: "bin\CrashReporter\bitcrusher.json";   DestDir: "{commonappdata}\Rabien Software\Crash Reporter\Plugins"; Flags: ignoreversion uninsneveruninstall
