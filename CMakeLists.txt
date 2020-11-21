cmake_minimum_required(VERSION 3.8)

if(DEFINED PROJECT_NAME)
  set(REPR_SUBPROJECT ON)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
  project(repr VERSION 1.0.0 LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/p-ranav/repr"
    DESCRIPTION "repr for Modern C++: Return a string containing a printable representation of a value")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL "3.9")
  project(repr VERSION 1.0.0 LANGUAGES CXX
    DESCRIPTION "repr for Modern C++: Return a string containing a printable representation of a value")
else()
  project(repr VERSION 1.0.0 LANGUAGES CXX)
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup()
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(repr INTERFACE)
add_library(repr::repr ALIAS repr)

target_compile_features(repr INTERFACE cxx_std_17)
target_include_directories(repr INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(NOT REPR_SUBPROJECT)
  configure_package_config_file(reprConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/reprConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/repr)
  write_basic_package_version_file(reprConfigVersion.cmake
                                   COMPATIBILITY AnyNewerVersion)

  configure_file(repr.pc.in repr.pc @ONLY)

  install(TARGETS repr EXPORT reprTargets)
  install(EXPORT reprTargets
          FILE reprTargets.cmake
          NAMESPACE repr::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/repr)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/reprConfig.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/reprConfigVersion.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/repr)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/repr.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
  install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/repr
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          USE_SOURCE_PERMISSIONS
          PATTERN "*.hpp" PATTERN "*.h")
  install(FILES LICENSE
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/repr)

  if(EXISTS "${PROJECT_SOURCE_DIR}/.gitignore")
    # Simple glob to regex conversion (.gitignore => CPACK_SOURCE_IGNORE_FILES)
    file(READ ".gitignore" DOT_GITIGNORE)
    string(REPLACE ";" "RANDOMSEQUENCE" DOT_GITIGNORE "${DOT_GITIGNORE}")
    string(REPLACE "\n" ";" DOT_GITIGNORE "${DOT_GITIGNORE}")
    string(REPLACE "RANDOMSEQUENCE" "\\;" DOT_GITIGNORE "${DOT_GITIGNORE}")
    foreach(IGNORE_LINE ${DOT_GITIGNORE})
      if(NOT IGNORE_LINE OR IGNORE_LINE MATCHES "^#")
        continue()
      endif()
      string(REPLACE "\\" "\\\\" IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "." "\\\\." IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "*" ".*" IGNORE_LINE "${IGNORE_LINE}")
      string(REPLACE "+" "\\\\+" IGNORE_LINE "${IGNORE_LINE}")
      list(APPEND CPACK_SOURCE_IGNORE_FILES "${IGNORE_LINE}")
    endforeach()
  endif()

  # extra ignored files
  list(APPEND CPACK_SOURCE_IGNORE_FILES
    .editorconfig
    .git
    .gitignore
    .travis.yml
    .appveyor.yml
  )
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
  set(CPACK_GENERATOR "TGZ;TXZ")
  set(CPACK_SOURCE_GENERATOR "TGZ;TXZ")
  include(CPack)
endif()