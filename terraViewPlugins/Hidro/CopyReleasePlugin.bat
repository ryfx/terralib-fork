REM Verifica se existe a pasta plugins
if not exist ..\..\Release\ exit
if not exist ..\..\Release\Plugins mkdir ..\..\Release\Plugins

REM copia tudo
copy /Y ..\Release\win32-msvc\Hidro.* ..\..\Release\Plugins

pause
