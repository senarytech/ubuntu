if("${SYNAP_BUILD_TYPE}" STREQUAL "VSSDK_REL")
    install(FILES CMakeLists.txt DESTINATION framework/lib)
    install(DIRECTORY
        stb synap_utils synapnb synap_app synap_img synap_preprocess synap_postprocess nlohmann synap_base
        DESTINATION framework/lib
        PATTERN "allocator_*_debug.*" EXCLUDE
        PATTERN "allocator_secure.cpp" EXCLUDE
        PATTERN "allocator_osal.*" EXCLUDE
        )
    return()
endif()

add_subdirectory(stb)
add_subdirectory(nlohmann)
add_subdirectory(synap_utils)
add_subdirectory(synap_base)
add_subdirectory(synap_app)

if(CMAKE_CROSSCOMPILING OR ENABLE_SYNAP_DEVICE_SIMULATOR)
    add_subdirectory(synapnb)
    add_subdirectory(synap_img)
    add_subdirectory(synap_preprocess)
    add_subdirectory(synap_postprocess)
endif()
