# SPDX-License-Identifier: Unlicense

set(ENTRY_SYMBOL "${PROJECT_NAME}_extension_init")
set(GD_EXTENSION_FILE lib${PROJECT_NAME}.gdextension)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/template.gdextension.in
  ${CMAKE_BINARY_DIR}/bin/${GD_EXTENSION_FILE}
  @ONLY
)

add_custom_target(templates
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/bin/${GD_EXTENSION_FILE}
    ${BUILD_OUTPUT_DIR}/${GD_EXTENSION_FILE}
  SOURCES ${CMAKE_BINARY_DIR}/bin/${GD_EXTENSION_FILE}
)

add_dependencies(${PROJECT_NAME} templates)

install(FILES ${BUILD_OUTPUT_DIR}/${GD_EXTENSION_FILE} DESTINATION ${INSTALL_DIR})

unset(GD_EXTENSION_FILE)
unset(ENTRY_SYMBOL)
