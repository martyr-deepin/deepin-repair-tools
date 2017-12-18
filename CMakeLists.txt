cmake_minimum_required(VERSION 3.7)

project(deepin-repair-tools
        VERSION 1.0.0)

set(BIN_NAME deepin-repair-tools)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS -Wall)

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS -O3)

    # generate qm
    execute_process(COMMAND bash "translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

# Sources files
file(GLOB SRCS "src/*.h" "src/*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)

pkg_check_modules(DtkWidget REQUIRED dtkwidget)

# driver-manager
add_executable(${BIN_NAME} ${SRCS})
target_include_directories(${BIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS})
target_link_libraries(${BIN_NAME}
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

# bin
install(TARGETS ${BIN_NAME} DESTINATION bin)
