if("${SYNAP_BUILD_TYPE}" STREQUAL "VSSDK_REL")
    install(FILES CMakeLists.txt DESTINATION framework/app)
    install(DIRECTORY synap_init synap_cli_nb synap_cli synap_cli_ic synap_cli_od synap_cli_ip
        DESTINATION framework/app)
    return()
endif()

add_subdirectory(synap_cli_nb)

if(CMAKE_CROSSCOMPILING OR ENABLE_SYNAP_DEVICE_SIMULATOR)
    add_subdirectory(synap_cli)
    add_subdirectory(synap_cli_ic)
    add_subdirectory(synap_cli_od)
    add_subdirectory(synap_cli_ip)
    add_subdirectory(synap_init)

    if("${SYNAP_BUILD_TYPE}" STREQUAL "SyNAP")
        add_subdirectory(synap_cli_ic2)
        add_subdirectory(synap_cli_sr)
        add_subdirectory(synap_cli_rnn)
    endif()

endif()

