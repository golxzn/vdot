# SPDX-License-Identifier: Unlicense

include(GitVersionInfo)

generate_git_version_file(
  TEMPLATE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
  OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/include/gen
)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.*pp)
list(APPEND sources ${CMAKE_BINARY_DIR}/src/include/gen/version.hpp)

target_sources(${PROJECT_NAME} PRIVATE ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src/include
)

