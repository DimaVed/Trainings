
cd  c:\dev\

git clone https://github.com/microsoft/vcpkg.git

cd vcpkg
bootstrap-vcpkg.bat




vcpkg install catch2   --triplet x64-windows-static --clean-after-build
