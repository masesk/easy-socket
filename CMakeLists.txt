cmake_minimum_required (VERSION 3.5)

project(easy-socket VERSION 0.2.0)

option(EASY_SOCKET_BUILD_TESTS "Turn on to build tests." ON)

set(PROJECT_COMPATIBILITY AnyNewerVersion)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

set(target_dependencies "")
if(WIN32)
    list(APPEND target_dependencies wsock32 ws2_32)
endif()

target_link_libraries(${PROJECT_NAME} INTERFACE ${target_dependencies})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ${PROJECT_COMPATIBILITY}
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}-${PROJECT_VERSION} COMPONENT Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}-${PROJECT_VERSION} COMPONENT Development
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}-${PROJECT_VERSION} COMPONENT Runtime
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}-${PROJECT_VERSION} COMPONENT Runtime
    PUBLIC_HEADER DESTINATION ${INCLUDE_DESTINATION} COMPONENT Development
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION lib/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

install(
    FILES
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION
      lib/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include
    DESTINATION include-${PROJECT_VERSION}
)

  set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION} CACHE INTERNAL "")

if(EASY_SOCKET_BUILD_TESTS)
    # Add sub directories
    add_subdirectory(test/test-server)
    add_subdirectory(test/test-client)
endif()