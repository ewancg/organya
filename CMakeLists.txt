cmake_minimum_required(VERSION 3.15...3.25)
set(CMAKE_CXX_STANDARD 11)

project(organon
  VERSION
    1.0
  DESCRIPTION
    "im taking a shit"
  LANGUAGES
    CXX
)

set(PROJECT_SOURCES 
    ./source/Click.cpp
    ./source/DlgEdit.cpp
    ./source/DlgEZCopy.cpp
    ./source/DlgPlayer.cpp
    ./source/DlgSet.cpp
    ./source/DlgTrack.cpp
    ./source/Filer.cpp
    ./source/Gdi.cpp
    ./source/MainPut.cpp
    ./source/Mouse.cpp
    ./source/OrgData.cpp
    ./source/OrgEdit.cpp
    ./source/OrgFile.cpp
    ./source/OrgFileMIDI.cpp
    ./source/OrgPlay.cpp
    ./source/rxoFunction.cpp
    ./source/Scroll.cpp
    ./source/Sound.cpp
    ./source/WinMain.cpp
    ./source/WinTimer.cpp
)

set(PROJECT_HEADERS
    ./source/Click.h
    ./source/DefOrg.h
    ./source/Filer.h
    ./source/Gdi.h
    ./source/Mouse.h
    ./source/OrgData.h
    ./source/resource.h
    ./source/rxoFunction.h
    ./source/Scroll.h
    ./source/Setting.h
    ./source/Sound.h
    ./source/Timer.h
)

set(PROJECT_RESOURCES
    ./source/Script1.rc
)

add_compile_definitions(
    BOOL=bool
)

# ld can't handle llvm-rc's swag
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fuse-ld=lld")

# this is unset on my system for some reason
set(CMAKE_INCLUDE_PATH "C:/msys64/ucrt64/include")

# gnu windres can't handle the swag
set(CMAKE_RC_COMPILER llvm-rc)

# code page (-C) 932 is shift JIS, we need to override with UTF-8 (65001)
set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <DEFINES> -I ${CMAKE_INCLUDE_PATH} -C 65001 ${rc_flags} -FO <OBJECT> <SOURCE>")

add_executable(organon ${PROJECT_SOURCES} ${PROJECT_RESOURCES})
target_link_libraries(organon dsound winmm kernel32 gdi32 comdlg32 shell32)
