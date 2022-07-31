cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME vscode-qt6.3-cmake-ninja)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# QT setup
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.3.1/mingw_64/lib/cmake/")
find_package(Qt6 REQUIRED COMPONENTS Widgets Core QUIET)
qt_standard_project_setup()

#
# copy builded .dll to .exe folder
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include Qt deployment helper function
include(windeployqt)

# ----------------- main app
set(SORCES
    src/communication.hpp
    src/controller.hpp
)

add_executable(${PROJECT_NAME}
    ${SORCES}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Core_INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)
# ----------------- /main app

# ----------------- tests app
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    ${SORCES}
    tests/communication_test.cpp
)

target_link_libraries(
    tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
# windeployqt(tests)

## ----------------- tests app