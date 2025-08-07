cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VERSION})

if (MSVC)
    set(BOOST_ROOT "D:/Boost/boost_1_88_0/") # Путь к библиотеке Boost
endif()

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# async lib
# add_library(async
#     lib/types.h
#     lib/async.h
#     lib/async.cpp
#     lib/commandparser.h
#     lib/commandparser.cpp
# )

# server executable
add_executable(${PROJECT_NAME}
    main.cpp
    # server/dispatcher.h
    # server/dispatcher.cpp
    server/joinserver.h
    server/joinserver.cpp
)

#
set_target_properties(${PROJECT_NAME} async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

#
target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_BINARY_DIR}
)

# server executable linking the lib
target_link_libraries(${PROJECT_NAME}
PRIVATE
    # async
    ${Boost_LIBRARIES}
)

#
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT vu-nrp@yandex.ru)
include(CPack)
