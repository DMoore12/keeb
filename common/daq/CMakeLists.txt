set(TARGET_NAME DAQ)
add_library(${TARGET_NAME})

# Find all .c sources in project
#file(GLOB_RECURSE glob_sources "*.c")
target_sources(${TARGET_NAME} PRIVATE "daq_base.c")

# Find directories for '#include'
# For libraries, these directories are all referenced to the top level firmware directory, CMAKE_SOURCE_DIR
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} "CMSIS_L432;PHAL_L432;QUEUE")

set(TARGET_NAME DAQ_L496)
add_library(${TARGET_NAME})

# Find all .c sources in project
#file(GLOB_RECURSE glob_sources "*.c")
target_sources(${TARGET_NAME} PRIVATE "daq_base.c")

# Find directories for '#include'
# For libraries, these directories are all referenced to the top level firmware directory, CMAKE_SOURCE_DIR
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} "CMSIS_L496;PHAL_L496;QUEUE")