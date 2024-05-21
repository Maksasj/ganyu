file(GLOB_RECURSE GANYU_SOURCE_FILES *.c)

include_directories("./")
include_directories("../3dparty/chttp/src")
include_directories("/usr/include/postgresql")

find_package(libssh REQUIRED)

add_executable(ganyu ${GANYU_SOURCE_FILES})
target_link_libraries(ganyu chttp)
target_link_libraries(ganyu glob)
target_link_libraries(ganyu pq)
