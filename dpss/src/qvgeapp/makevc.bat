call "c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat" 

cd ..\..\
mkdir _builds_qvge_msvc
cd .\_builds_qvge_msvc
c:\Qt5.7\5.10.0\msvc2015_64\bin\qmake ..\src\qvgeapp\qvgeapp.pro -tp vc
