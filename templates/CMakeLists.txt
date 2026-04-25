# SPDX-License-Identifier: Unlicense

add_custom_target(templates SOURCES template.gdextension.in)

add_dependencies( ${PROJECT_NAME} templates )

set(ENTRY_SYMBOL "${PROJECT_NAME}_extension_init")
set(GD_EXTENSION_FILE ${PROJECT_NAME}.gdextension)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/template.gdextension.in
  ${BUILD_OUTPUT_DIR}/lib${GD_EXTENSION_FILE}
  @ONLY
)

install(FILES ${BUILD_OUTPUT_DIR}/${GD_EXTENSION_FILE} DESTINATION ${INSTALL_DIR})



unset(GD_EXTENSION_FILE)
unset(ENTRY_SYMBOL)
