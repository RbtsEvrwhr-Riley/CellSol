@echo off
cls
hugo -D -b https://www.f3.to/cellsol
rd /s /q .\resources\_gen
cd public
"C:\Program Files\7-Zip\7z.exe" a -tzip -r ..\cellsolpublic.zip * 
cd ..
rd /s /q .\public
start .\public.zip
pause
