# Straight-Forward UI
# Main CMakeLists

if(NOT FIPS_IMPORT)
    project(SFUI)
    get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
    include(${FIPS_ROOT_DIR}/cmake/fips.cmake)
    fips_setup()
endif()

add_subdirectory(code)
add_subdirectory(samples)