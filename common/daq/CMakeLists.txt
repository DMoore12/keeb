MACRO(MAKE_DAQ_LIB lib_name lib_link hal_name)
    set(TARGET_NAME ${lib_name})
    add_library(${TARGET_NAME})

    # Find all .c sources in project
    #file(GLOB_RECURSE glob_sources "*.c")
    target_sources(${TARGET_NAME} PRIVATE "daq_base.c")

    # Find directories for '#include'
    # For libraries, these directories are all referenced to the top level firmware directory, CMAKE_SOURCE_DIR
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

    target_link_libraries(${TARGET_NAME} "QUEUE")
    target_link_libraries(${TARGET_NAME} ${lib_link})
    target_link_libraries(${TARGET_NAME} ${hal_name})

ENDMACRO(MAKE_DAQ_LIB)

MAKE_DAQ_LIB(DAQ CMSIS_L432 PHAL_L432)
MAKE_DAQ_LIB(DAQ_L496 CMSIS_L496 PHAL_L496)
MAKE_DAQ_LIB(DAQ_F407 CMSIS_F407 PHAL_F407)
MAKE_DAQ_LIB(DAQ_F732 CMSIS_F732 PHAL_F732)

MACRO(MAKE_CAN_PARSE_LIB lib_name lib_link hal_name sched_name)
    set(TARGET_NAME ${lib_name})
    add_library(${TARGET_NAME})

    # Find all .c sources in project
    #file(GLOB_RECURSE glob_sources "*.c")
    target_sources(${TARGET_NAME} PRIVATE "can_parse_base.c")

    # Find directories for '#include'
    # For libraries, these directories are all referenced to the top level firmware directory, CMAKE_SOURCE_DIR
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

    target_link_libraries(${TARGET_NAME} "QUEUE")
    target_link_libraries(${TARGET_NAME} ${lib_link})
    target_link_libraries(${TARGET_NAME} ${hal_name})
    target_link_libraries(${TARGET_NAME} ${sched_name})

ENDMACRO(MAKE_CAN_PARSE_LIB)

MAKE_CAN_PARSE_LIB(CAN_PARSE CMSIS_L432 PHAL_L432 PSCHED)
MAKE_CAN_PARSE_LIB(CAN_PARSE_L496 CMSIS_L496 PHAL_L496 PSCHED_L496)
MAKE_CAN_PARSE_LIB(CAN_PARSE_F407 CMSIS_F407 PHAL_F407 PSCHED_F407)
MAKE_CAN_PARSE_LIB(CAN_PARSE_F732 CMSIS_F732 PHAL_F732 PSCHED_F732)
