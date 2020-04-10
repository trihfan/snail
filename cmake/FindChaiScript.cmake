if (CHAISCRIPT_INCLUDE_DIR)
    set(CHAISCRIPT_FOUND ${CHAISCRIPT_VERSION_OK})
else()
    find_path(CHAISCRIPT_INCLUDE_DIR NAMES ChaiScript
        HINTS
        PATHS
        ${CMAKE_INSTALL_PREFIX}/include)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(ChaiScript DEFAULT_MSG CHAISCRIPT_INCLUDE_DIR)
    mark_as_advanced(CHAISCRIPT)
endif()
