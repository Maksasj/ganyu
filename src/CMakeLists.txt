file(GLOB_RECURSE GANYU_SOURCE_FILES *.c)

include_directories("./")



add_executable(ganyu ${GANYU_SOURCE_FILES})