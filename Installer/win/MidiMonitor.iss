; MidiMonitor installer (Inno Setup)

#define MyAppName "MidiMonitor"
#define MyAppCompany "SocaLabs"
#define MyAppPublisher "SocaLabs"
#define MyAppCopyright "2026 SocaLabs"
#define MyAppURL "https://socalabs.com/"
#define MyAppVersion GetStringFileInfo("bin\VST3\MidiMonitor.vst3\Contents\x86_64-win\MidiMonitor.vst3", "ProductVersion")
#define MyDefaultDirName "{commoncf64}\VST3"

[Setup]
AppID={{16A2859A-CE08-4201-89E3-99D486243ED0}
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
OutputBaseFilename=MidiMonitor
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


[InstallDelete]
Type: files;          Name: "{commoncf64}\VST2\MidiMonitor.dll";   Components: vst
Type: filesandordirs; Name: "{commoncf64}\VST3\MidiMonitor.vst3"; Components: vst3
Type: files;          Name: "{commoncf64}\CLAP\MidiMonitor.clap"; Components: clap


[Files]
Source: "bin\VST\MidiMonitor.dll";    DestDir: "{commoncf64}\VST2";                Flags: ignoreversion overwritereadonly; Components: vst
Source: "bin\VST3\MidiMonitor.vst3\*"; DestDir: "{commoncf64}\VST3\MidiMonitor.vst3\"; Flags: ignoreversion overwritereadonly recursesubdirs; Components: vst3
Source: "bin\CLAP\MidiMonitor.clap";   DestDir: "{commoncf64}\CLAP";              Flags: ignoreversion overwritereadonly; Components: clap

