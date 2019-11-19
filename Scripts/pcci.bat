echo on
cd "%~dp0..%"
set ROOT=%cd%
cd "%ROOT%"
echo "%ROOT%"

call Scripts\pcresaveprojects.bat
call Scripts\pcbuildprojects.bat
