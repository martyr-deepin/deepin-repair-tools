
project(password_reset)
set(LIB_NAME "password_reset")

# Sources files
file(GLOB SRCS "*.h" "*.cpp")

find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_PLUGIN)
add_library(password_reset SHARED ${SRCS})
target_link_libraries(${LIB_NAME}
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
)
