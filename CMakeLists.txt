cmake_minimum_required(VERSION 3.7)

project(deepin-repair-tools
        VERSION 1.0.0)

set(BIN_NAME deepin-repair-tools)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)
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

# Plugins
add_subdirectory("plugins")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)

pkg_check_modules(DtkWidget REQUIRED dtkwidget)

# driver-manager
add_executable(${BIN_NAME} ${SRCS} resources.qrc)
target_include_directories(${BIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS})
target_link_libraries(${BIN_NAME} PRIVATE
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

# bin
install(FILES deepin-repair-tools-pkexec DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${BIN_NAME} DESTINATION sbin)

# desktop
install(FILES deepin-repair-tools.desktop DESTINATION share/applications)

# icon
install(FILES deepin-repair-tools.svg
        DESTINATION share/icons/hicolor/scalable/apps)

# scripts
file(GLOB SCRIPTS "src/*.sh")
install(FILES ${SCRIPTS} DESTINATION lib/deepin-repair-tools)

# qm files
file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES}
        DESTINATION share/deepin-repair-tools/translations)

# policy kit
install(FILES com.deepin.repair-tools.policy
        DESTINATION share/polkit-1/actions/)

# dev files
install (FILES
            src/repairtoolsproxy.h
            src/repairtoolsinterface.h
         DESTINATION include/deepin-repair-tools)

# manual files
install (DIRECTORY manual/
         DESTINATION share/dman/deepin-repair)
