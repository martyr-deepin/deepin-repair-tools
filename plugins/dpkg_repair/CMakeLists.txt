
set(PLUGIN_NAME "dpkg-repair")

project(${PLUGIN_NAME})

# Sources files
file(GLOB SRCS "*.h" "*.cpp")

find_package(Qt5Widgets REQUIRED)
find_package(DtkWidget REQUIRED dtkwidget)

add_definitions(-DQT_PLUGIN)
add_definitions(${QT_DEFINITIONS})
add_library(${PLUGIN_NAME} SHARED ${SRCS} resources_dpkg_repair.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
target_include_directories(${PLUGIN_NAME} PUBLIC ${DtkWidget_INCLUDE_DIRS})
target_link_libraries(${PLUGIN_NAME} PRIVATE
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/deepin-repair-tools/plugins)
install(FILES dpkg_repair.sh
        DESTINATION lib/deepin-repair-tools/plugins/${PLUGIN_NAME}
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
