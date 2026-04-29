; Compressor installer (Inno Setup)

#define MyAppName "Compressor"
#define MyAppCompany "SocaLabs"
#define MyAppPublisher "SocaLabs"
#define MyAppCopyright "2026 SocaLabs"
#define MyAppURL "https://socalabs.com/"
#define MyAppVersion GetStringFileInfo("bin\VST3\Compressor.vst3\Contents\x86_64-win\Compressor.vst3", "ProductVersion")
#define MyDefaultDirName "{commoncf64}\VST3"

[Setup]
AppID={{AEA0375C-B101-431D-A378-E288CCE8482F}
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
OutputBaseFilename=Compressor
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

[InstallDelete]
Type: files;          Name: "{commoncf64}\VST\Compressor.dll";   Components: vst
Type: filesandordirs; Name: "{commoncf64}\VST3\Compressor.vst3"; Components: vst3
Type: files;          Name: "{commoncf64}\CLAP\Compressor.clap"; Components: clap
Type: filesandordirs; Name: "{commonappdata}\SocaLabs\Compressor\Presets";    Components: resources

[Files]
Source: "bin\VST\Compressor.dll";    DestDir: "{commoncf64}\VST";                Flags: ignoreversion overwritereadonly; Components: vst
Source: "bin\VST3\Compressor.vst3\*"; DestDir: "{commoncf64}\VST3\Compressor.vst3\"; Flags: ignoreversion overwritereadonly recursesubdirs; Components: vst3
Source: "bin\CLAP\Compressor.clap";   DestDir: "{commoncf64}\CLAP";              Flags: ignoreversion overwritereadonly; Components: clap
Source: "..\..\plugins\Compressor\Resources\*.xml"; DestDir: "{commonappdata}\SocaLabs\Compressor\Presets\"; Flags: ignoreversion; Components: resources
