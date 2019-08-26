file(GLOB SRCS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(csoap STATIC ${SRCS})

target_link_libraries(csoap webcc pugixml)
