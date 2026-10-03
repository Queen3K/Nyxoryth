cmake_minimum_required(VERSION 3.20)

project(
    NyxorythCalculator
    VERSION 1.1.0
    LANGUAGES CXX RC
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Nyxoryth WIN32
    Source/App/Main.cpp
    Source/UI/MainWindow.cpp
    Source/Resources/Nyxoryth.rc
)

target_include_directories(Nyxoryth PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Resources"
)

target_compile_definitions(Nyxoryth PRIVATE
    UNICODE
    _UNICODE
    NOMINMAX
)

target_link_libraries(Nyxoryth PRIVATE
    gdiplus
    comdlg32
    comctl32
)

if(MINGW)
    target_link_options(Nyxoryth PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()
