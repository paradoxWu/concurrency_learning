cmake_minimum_required(VERSION 3.16)
# 设置工程名称
set(PROJECT_NAME gaussian_blur)
# 设置工程版本号
set(PROJECT_VERSION "0.0.1" CACHE STRING "default")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME}
    LANGUAGES CXX C 
    VERSION ${PROJECT_VERSION}
)

OPTION(DEBUG    "Output log to terminal for debugging" OFF)

IF(DEBUG)
        ADD_DEFINITIONS(-DDEBUG=1)
ENDIF()

find_package( OpenCV REQUIRED )
IF (OpenCV_FOUND)
    # message(STATUS "OpenCV library: ${OpenCV_INSTALL_PATH}")
    message(STATUS "    version: ${OpenCV_VERSION}")
    # message(STATUS "    libraries: ${OpenCV_LIBS}")
    # message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
    include_directories(${OpenCV_INCLUDE_DIRS})
ELSE ()
    ADD_DEFINITIONS(-DWITHOUT_CV)
ENDIF ()

add_executable(${PROJECT_NAME} gaussian_blur.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION install)