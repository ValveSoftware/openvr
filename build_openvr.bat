setlocal EnableDelayedExpansion

@echo off
set TOOLCHAIN=%1

if %TOOLCHAIN% equ msvc2015 (
    set CMAKE_GENERATOR=Visual Studio 14 2015 Win64
    call "%VS140COMNTOOLS%..\..\VC\bin\amd64\vcvars64.bat"
    if !errorlevel! neq 0 exit /b !errorlevel!
) else (
    echo Failure: unknown msvc version: %TOOLCHAIN%
    exit /b 1
)

set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%/build.%TOOLCHAIN%
set INSTALL_DIR=%ROOT_DIR%/artifacts

cmake -E make_directory "%BUILD_DIR%"
if !errorlevel! neq 0 exit /b !errorlevel!
cmake -E chdir "%BUILD_DIR%" cmake -G "%CMAKE_GENERATOR%" -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ..
if !errorlevel! neq 0 exit /b !errorlevel!
cmake --build "%BUILD_DIR%" --clean-first --config Release --target install -- "/m"
if !errorlevel! neq 0 exit /b !errorlevel!
cmake -E copy_directory %ROOT_DIR%/headers %INSTALL_DIR%/include
if !errorlevel! neq 0 exit /b !errorlevel!
