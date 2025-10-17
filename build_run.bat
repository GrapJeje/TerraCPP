@echo off
cls
echo Compiling Java and generating JNI header...
javac -h src\main\cpp src\main\java\nl\grapjeje\nativechunks\NativeChunkGenerator.java

echo Building C++ native library...
mkdir src\main\cpp\build 2>nul
cd src\main\cpp\build
cmake ..
cmake --build .
