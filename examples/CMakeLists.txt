# Examples

# Common libraries to link for examples.
set(EXAMPLE_LIBS
    csoap
    Boost::filesystem
    Boost::system
    Boost::date_time
    "${CMAKE_THREAD_LIBS_INIT}")

if(WEBCC_ENABLE_SSL)
    set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${OPENSSL_LIBRARIES})

    if(WIN32)
        set(EXAMPLE_LIBS ${EXAMPLE_LIBS} crypt32)
    endif()
endif()

if(WEBCC_ENABLE_GZIP)
    if(WIN32)
        set(EXAMPLE_LIBS ${EXAMPLE_LIBS} zlibstatic)
    else()
        set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${ZLIB_LIBRARIES})
    endif()
endif()

if(UNIX)
    # Add `-ldl` for Linux to avoid "undefined reference to `dlopen'".
    set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${CMAKE_DL_LIBS})
endif()

add_executable(calc_client_parasoft calc_client_parasoft.cc)
target_link_libraries(calc_client_parasoft ${EXAMPLE_LIBS})
