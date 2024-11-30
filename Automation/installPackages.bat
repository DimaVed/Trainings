

cd  c:\dev\

git clone https://github.com/microsoft/vcpkg.git

cd vcpkg
bootstrap-vcpkg.bat



REM пример установки пакета с нужным триплетом и очисткой временных файлов
vcpkg install catch2   --triplet x64-windows-static --clean-after-build
