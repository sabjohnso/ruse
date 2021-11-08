cmake_minimum_required(VERSION 3.12)

project(ruse VERSION 0.1.0 LANGUAGES C CXX)

option(ruse_BUILD_TESTING "Build the ruse tests" ON)

include(GNUInstallDirs)
set(ruse_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(ruse_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(ruse_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
set(ruse_INSTALL_CONFDIR ${ruse_INSTALL_LIBDIR}/cmake/ruse-${PROJECT_VERSION})

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(ruse_deps)

add_subdirectory(ruse)

if(ruse_BUILD_TESTING)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(ruse_testing)
  endif()
endif()

install(EXPORT ruse_EXPORTS
  NAMESPACE ruse::
  FILE ruse-exports.cmake
  DESTINATION ${ruse_INSTALL_CONFDIR})

configure_file(ruse-config.cmake.in ruse-config.cmake)
install(FILES ${PROJECT_BINARY_DIR}/ruse-config.cmake
  DESTINATION ${ruse_INSTALL_CONFDIR})








