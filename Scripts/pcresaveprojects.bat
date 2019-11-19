set MSBUILD_EXE=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\MSBuild.exe

set ROOT=%cd%

cd "%ROOT%\modules\juce\extras\Projucer\Builds\VisualStudio2017\"
"%MSBUILD_EXE%" Projucer.sln /p:VisualStudioVersion=15.0 /m /t:Build /p:Configuration=Release /p:Platform=x64 /p:PreferredToolArchitecture=x64 
if %errorlevel% neq 0 exit /b %errorlevel%

.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\slOscilloscope\slOscilloscope.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\Spectrum Analyzer\Spectrum Analyzer.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\Tone Generator\Tone Generator.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\Channel Mute\Channel Mute.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\Maths\Maths.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\AB Tester\AB Tester.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\AddInvert\AddInvert.jucer"
.\x64\Release\App\Projucer.exe --resave "%ROOT%\plugins\SFX8\SFX8.jucer"

cd "%ROOT%"