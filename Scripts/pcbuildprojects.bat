  rd /s /q bin
mkdir bin

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\PAPU\plugin\Builds\VisualStudio2017\PAPU.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\PAPU\plugin\Builds\VisualStudio2017\PAPU.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\RP2A03\plugin\Builds\VisualStudio2017\RP2A03.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\RP2A03\plugin\Builds\VisualStudio2017\RP2A03.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\SID\plugin\Builds\VisualStudio2017\SID.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\SID\plugin\Builds\VisualStudio2017\SID.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\SN76489\plugin\Builds\VisualStudio2017\SN76489.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\SN76489\plugin\Builds\VisualStudio2017\SN76489.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\Voc\plugin\Builds\VisualStudio2017\Voc.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\..\Voc\plugin\Builds\VisualStudio2017\Voc.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\slOscilloscope\Builds\VisualStudio2017\Oscilloscope.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Spectrum Analyzer\Builds\VisualStudio2017\Spectrum Analyzer.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Tone Generator\Builds\VisualStudio2017\Tone Generator.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Channel Mute\Builds\VisualStudio2017\Channel Mute.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release /p:PlatformTarget=x86
if %errorlevel% neq 0 exit /b %errorlevel%
"C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "..\plugins\Maths\Builds\VisualStudio2017\Maths.sln" /p:VisualStudioVersion=15.0 /m /p:Configuration=Release64 /p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%


copy "C:\Program Files\VSTPlugins\PAPU_64b.dll" bin
copy "C:\Program Files\VSTPlugins\RP2A03_64b.dll" bin
copy "C:\Program Files\VSTPlugins\SID_64b.dll" bin
copy "C:\Program Files\VSTPlugins\SN76489_64b.dll" bin
copy "C:\Program Files\VSTPlugins\Voc_64b.dll" bin
copy "C:\Program Files\VSTPlugins\Oscilloscope_64b.dll" bin
copy "C:\Program Files\VSTPlugins\SpectrumAnalyzer_64b.dll" bin
copy "C:\Program Files\VSTPlugins\ToneGenerator_64b.dll" bin
copy "C:\Program Files\VSTPlugins\ChannelMute_64b.dll" bin
copy "C:\Program Files\VSTPlugins\Maths_64b.dll" bin

copy "C:\Program Files (x86)\VSTPlugins\PAPU_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\RP2A03_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\SID_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\SN76489_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\Voc_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\Oscilloscope_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\SpectrumAnalyzer_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\ToneGenerator_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\ChannelMute_32b.dll" bin
copy "C:\Program Files (x86)\VSTPlugins\Maths_32b.dll" bin

cd bin
zip PAPU_Win.zip PAPU_32b.dll PAPU_64b.dll
zip RP2A03_Win.zip RP2A03_32b.dll RP2A03_64b.dll
zip SID_Win.zip SID_32b.dll SID_64b.dll
zip SN76489_Win.zip SN76489_32b.dll SN76489_64b.dll
zip Voc_Win.zip Voc_32b.dll Voc_64b.dll
zip Oscilloscope_Win.zip Oscilloscope_32b.dll Oscilloscope_64b.dll
zip SpectrumAnalyzer_Win.zip SpectrumAnalyzer_32b.dll SpectrumAnalyzer_64b.dll
zip ToneGenerator_Win.zip ToneGenerator_32b.dll ToneGenerator_64b.dll
zip ChannelMute_Win.zip ChannelMute_32b.dll ChannelMute_64b.dll
zip Maths_Win.zip Maths_32b.dll Maths_64b.dll
zip All_Win.zip *.dll
cd ..
