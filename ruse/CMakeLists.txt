configure_file(config.hpp.in config.hpp)

add_library(ruse_header INTERFACE)
set_target_properties(ruse_header PROPERTIES EXPORT_NAME header)
target_include_directories(ruse_header
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${ruse_INSTALL_INCLUDEDIR}>)

add_library(ruse::header ALIAS ruse_header)
  
install(TARGETS ruse_header
  EXPORT ruse_EXPORTS
  RUNTIME DESTINATION ${ruse_INSTALL_BINDIR}
  LIBRARY DESTINATION ${ruse_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${ruse_INSTALL_LIBDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
  DESTINATION ${ruse_INSTALL_INCLUDEDIR}/ruse)
