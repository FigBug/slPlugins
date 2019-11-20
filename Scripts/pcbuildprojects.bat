echo on
set MSBUILD_EXE=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\MSBuild.exe

set ROOT=%cd%
cd "%ROOT%"

cd Scripts
mkdir bin
mkdir bin\win
mkdir bin\zip

"%MSBUILD_EXE%" "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\AB Tester\Builds\VisualStudio2017\AB Tester.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\AB Tester\Builds\VisualStudio2017\AB Tester.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\AddInvert\Builds\VisualStudio2017\Add and Invert.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\AddInvert\Builds\VisualStudio2017\Add and Invert.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"%MSBUILD_EXE%" "%ROOT%\plugins\SFX8\Builds\VisualStudio2017\SFX8.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"%MSBUILD_EXE%" "%ROOT%\plugins\SFX8\Builds\VisualStudio2017\SFX8.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
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

cd bin
7z a PAPU_Win.zip PAPU_32b.dll PAPU_64b.dll
7z a RP2A03_Win.zip RP2A03_32b.dll RP2A03_64b.dll
7z a SID_Win.zip SID_32b.dll SID_64b.dll
7z a SN76489_Win.zip SN76489_32b.dll SN76489_64b.dll
7z a Voc_Win.zip Voc_32b.dll Voc_64b.dll
7z a Oscilloscope_Win.zip Oscilloscope_32b.dll Oscilloscope_64b.dll
7z a SpectrumAnalyzer_Win.zip SpectrumAnalyzer_32b.dll SpectrumAnalyzer_64b.dll
7z a ToneGenerator_Win.zip ToneGenerator_32b.dll ToneGenerator_64b.dll
7z a ChannelMute_Win.zip ChannelMute_32b.dll ChannelMute_64b.dll
7z a Maths_Win.zip Maths_32b.dll Maths_64b.dll
7z a ABTester_Win.zip ABTester_32b.dll ABTester_64b.dll
7z a AddInvert_Win.zip AddInvert_32b.dll AddInvert_64b.dll
7z a SFX8_Win.zip SFX8_32b.dll SFX8_64b.dll