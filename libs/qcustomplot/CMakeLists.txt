cmake_minimum_required(VERSION 3.19)

project(qcustomplot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt dependancies
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

add_library(qcustomplot SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qcustomplot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/qcustomplot.h
    )

target_include_directories(qcustomplot PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(qcustomplot PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::PrintSupport
)

target_compile_definitions(qcustomplot PRIVATE
  QCUSTOMPLOT_COMPILE_LIBRARY
)
