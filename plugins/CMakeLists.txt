
add_subdirectory("password_reset")
add_subdirectory("disk_clean")
add_subdirectory("dpkg_repair")
add_subdirectory("permissions_repair")

if (NOT DEFINED NO_GRUB_REPAIR_PLUGIN)
    add_subdirectory("grub_repair")
endif ()
