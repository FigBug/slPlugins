echo on
set MSBUILD_EXE=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\MSBuild.exe

set ROOT=%cd%
cd "%ROOT%"

cd Scripts
mkdir bin
mkdir bin\win
mkdir bin\zip

"MSBUILD_EXE" "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\AB Tester\Builds\VisualStudio2017\AB Tester.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\AB Tester\Builds\VisualStudio2017\AB Tester.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\AddInvert\Builds\VisualStudio2017\Add and Invert.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\AddInvert\Builds\VisualStudio2017\Add and Invert.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"MSBUILD_EXE" "%ROOT%\plugins\SFX8\Builds\VisualStudio2017\SFX8.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"MSBUILD_EXE" "%ROOT%\plugins\SFX8\Builds\VisualStudio2017\SFX8.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

copy "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\x64\Release64\VST\Oscilloscope_64b.dll" bin
copy "%ROOT%\plugin\Spectrum Analyzer\Builds\VisualStudio2017\x64\Release64\VST\SpectrumAnalyzer_64b.dll" bin
copy "%ROOT%\plugin\Tone Generator\Builds\VisualStudio2017\x64\Release64\VST\ToneGenerator_64b.dll" bin
copy "%ROOT%\plugin\Channel Mute\Builds\VisualStudio2017\x64\Release64\VST\ChannelMute_64b.dll" bin
copy "%ROOT%\plugin\Maths\Builds\VisualStudio2017\x64\Release64\VST\Maths_64b.dll" bin
copy "%ROOT%\plugin\AB Tester\Builds\VisualStudio2017\x64\Release64\VST\ABTester_64b.dll" bin
copy "%ROOT%\plugin\AddInvert\Builds\VisualStudio2017\x64\Release64\VST\AddInvert_64b.dll" bin
copy "%ROOT%\plugin\SFX8\Builds\VisualStudio2017\x64\Release64\VST\SFX8_64b.dll" bin

copy "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\Win32\Release\VST\Oscilloscope_32b.dll" bin
copy "%ROOT%\plugin\Spectrum Analyzer\Builds\VisualStudio2017\Win32\Release\VST\SpectrumAnalyzer_32b.dll" bin
copy "%ROOT%\plugin\Tone Generator\Builds\VisualStudio2017\Win32\Release\VST\ToneGenerator_32b.dll" bin
copy "%ROOT%\plugin\Channel Mute\Builds\VisualStudio2017\Win32\Release\VST\ChannelMute_32b.dll" bin
copy "%ROOT%\plugin\Maths\Builds\VisualStudio2017\Win32\Release\VST\Maths_32b.dll" bin
copy "%ROOT%\plugin\AB Tester\Builds\VisualStudio2017\Win32\Release\VST\ABTester_32b.dll" bin
copy "%ROOT%\plugin\AddInvert\Builds\VisualStudio2017\Win32\Release\VST\AddInvert_32b.dll" bin
copy "%ROOT%\plugin\SFX8\Builds\VisualStudio2017\Win32\Release\VST\SFX8_32b.dll" bin


