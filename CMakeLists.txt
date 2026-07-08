cmake_minimum_required(VERSION 3.19) 

project(db48x LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 QuickWidgets Widgets Multimedia OpenGLWidgets)

# Set the target and platform
# TODO: control other flags depending on target
set(TARGET db50x)
set(COLOR color)
set(VARIANT dm32)
set(MEMORY 500)
set(SDK dmcp5/dmcp)
set(PGM pgm5)
set(PGM_TARGET ${TARGET}.${PGM})
set(HELPFILE_NAME help/${TARGET}.md)
set(HELPINDEX_NAME help/${TARGET}.idx)

execute_process(COMMAND tools/build_id OUTPUT_VARIABLE BUILD_ID OUTPUT_STRIP_TRAILING_WHITESPACE)

qt_standard_project_setup()

include(CheckSymbolExists)
include(CheckIncludeFile)

check_symbol_exists(sigaction signal.h HAVE_SIGACTION)
check_include_file(regex.h HAVE_REGEX_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(signal.h HAVE_SIGNAL_H)
check_symbol_exists(drand48 stdlib.h HAVE_DRAND48)
check_symbol_exists(setlinebuf stdio.h HAVE_SETLINEBUF)
check_symbol_exists(SIGSTKSZ signal.h HAVE_SIGSTKSZ)
check_symbol_exists(strsignal string.h HAVE_STRSIGNAL)

configure_file(
    cmake/recorder-config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/recorder/config.h
)

add_library(recorder STATIC
    recorder/recorder.c
    recorder/recorder_ring.c
)

target_include_directories(recorder
    PUBLIC recorder
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/recorder
)
target_compile_definitions(recorder PRIVATE
    $<$<BOOL:${HAVE_SIGACTION}>:HAVE_SIGACTION>
    $<$<BOOL:${HAVE_REGEX_H}>:HAVE_REGEX_H>
    $<$<BOOL:${HAVE_SYS_MMAN_H}>:HAVE_SYS_MMAN_H>
    $<$<BOOL:${HAVE_SIGNAL_H}>:HAVE_SIGNAL_H>
    $<$<BOOL:${HAVE_DRAND48}>:HAVE_DRAND48>
    $<$<BOOL:${HAVE_SETLINEBUF}>:HAVE_SETLINEBUF>
    $<$<BOOL:${HAVE_SIGSTKSZ}>:HAVE_SIGSTKSZ>
    $<$<BOOL:${HAVE_STRSIGNAL}>:HAVE_STRSIGNAL>
)

# Run the shell command and capture its output
execute_process(
    COMMAND "git describe --dirty=Z --abbrev=4"
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Process the output to match the sed and cut commands
string(REGEX REPLACE "^v" "" GIT_VERSION "${GIT_VERSION}")
string(REGEX REPLACE "-g" "-" GIT_VERSION "${GIT_VERSION}")
string(SUBSTRING "${GIT_VERSION}" 0 16 DB48X_VERSION)

add_compile_definitions(DB48X_VERSION=\"${DB48X_VERSION}\")

add_subdirectory(sim)
