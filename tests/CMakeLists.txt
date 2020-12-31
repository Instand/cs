set(TEST_NAME cstests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test_program)
add_subdirectory(test_benchmark)

file(GLOB SRCS *.cpp)

if (MSVC)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
    add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1 /MTd /MP") # default CMAKE_CXX_FLAGS_DEBUG but /MDd -> /MTd
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG /MT /MP") # default CMAKE_CXX_FLAGS_RELEASE but /MD -> /MT
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /Ob1 /DNDEBUG /MT /MP") # default CMAKE_CXX_FLAGS_RELWITHVEBINFO but /MD -> /MT + /MP
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /MP") # default CMAKE_CXX_FLAGS_MINSIZEREL but /MD -> /MT

  set(CMAKE_C_FLAGS_RELMONITOR ${CMAKE_C_FLAGS_RELEASE})
  set(CMAKE_C_FLAGS_RELWALLET ${CMAKE_C_FLAGS_RELEASE})

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO /LTCG")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /INCREMENTAL:NO /LTCG")

  message(STATUS "CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
  message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
  message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO = ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4075")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4075")
  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4075")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  if(CMAKE_BUILD_TYPE EQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
  endif()

  message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
  message(STATUS "CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}")
  message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

if (MSVC)
    MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_RELMONITOR
        CMAKE_C_FLAGS_RELMONITOR
        CMAKE_EXE_LINKER_FLAGS_RELMONITOR
        CMAKE_CXX_FLAGS_RELWALLET
        CMAKE_C_FLAGS_RELWALLET
        CMAKE_EXE_LINKER_FLAGS_RELWALLET
        )

    option(BUILD_COMPILER "" OFF)
endif(MSVC)

macro(configure_msvc_runtime)
    if (MSVC)
        # Default to statically-linked runtime.
        if ("${MSVC_RUNTIME}" STREQUAL "")
            set(MSVC_RUNTIME "static")
        endif()

        # Set compiler options.
        set(variables
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_RELMONITOR
            CMAKE_C_FLAGS_RELWALLET
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELMONITOR
            CMAKE_CXX_FLAGS_RELWALLET
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            )

        if (${MSVC_RUNTIME} STREQUAL "static")
            message(STATUS "MSVC -> forcing use of statically-linked runtime.")

            foreach(variable ${variables})
                if (${variable} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
                endif()
            endforeach()
        else()
            message(STATUS "MSVC -> forcing use of dynamically-linked runtime.")

            foreach(variable ${variables})
                if (${variable} MATCHES "/MT")
                    string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
                endif()
            endforeach()
        endif()
    endif()
endmacro()

add_executable(${TEST_NAME} ${SRCS})

target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if (NOT MSVC AND NOT APPLE)
    # some way to resolve cyclic dependencies
    set(LINKER_START_GROUP "-Wl,--start-group")
    set(LINKER_END_GROUP "-Wl,--end-group")
endif()

target_link_libraries(${TEST_NAME} ${LINKER_START_GROUP} gtest gmock cs ${LINKER_END_GROUP})

if (Boost_FOUND)
    target_link_libraries(${TEST_NAME} Boost::filesystem)
endif()

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

configure_msvc_runtime()
