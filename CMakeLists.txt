cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME testa-delta)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# QT setup
 set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.3.1/mingw_64/lib/cmake/")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort SerialBus)
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

# Enable GTest
include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

# ----------------- main app
set(SORCES
    src/communication.h
    src/controller.h
    src/idataholder.h
    src/mainwindow.h
    src/mainwindow.cpp
    src/deltawidget.h
    src/deltawidget.cpp
    src/iprocess.h
)

add_executable(${PROJECT_NAME}
    ${SORCES}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6Core_INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)
# ----------------- /main app

## ----------------- tests app
#add_executable(tests
#    ${SORCES}
#    tests/communication_test.cpp
#)

#target_link_libraries(tests PRIVATE
#    Qt6::Core
#    Qt6::Widgets
#    gtest_main
#)

#gtest_discover_tests(tests)
## ----------------- tests app

# ----------------- tests with qt
add_executable(tests
    ${SORCES}
    tests/test.cpp
    tests/communication_helper.h
)

target_link_libraries(tests PRIVATE
    Qt6::Core
    Qt6::Widgets
#    Qt6::SerialPort
#    Qt6::SerialBus
    gtest_main
)

set_target_properties(tests PROPERTIES
    WIN32_EXECUTABLE ON
)

#qt_finalize_executable(test)

gtest_discover_tests(tests)
# ----------------- tests with qt

# windeployqt(tests)
