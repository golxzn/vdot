# SPDX-License-Identifier: Unlicense

function(generate_git_version_file)
  cmake_parse_arguments(PARSE_ARGV 0 GGVF
    "" "TEMPLATE_FILE;OUTPUT_DIRECTORY" ""
  )

  find_program(GIT_PROGRAM git)

  if (NOT GIT_PROGRAM)
    message(WARNING "generate_git_version_file: Failed to find git program")
    return()
  endif()

  # get version information
  execute_process(
    COMMAND "${GIT_PROGRAM}" --version
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  include(GetGitRevisionDescription)

  get_git_head_revision(GIT_REFSPEC GIT_SHA1)
  git_describe(GIT_SHORT)

  string(TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME)

  get_filename_component(file_name ${GGVF_TEMPLATE_FILE} NAME)
  string(REPLACE ".in" "" file_name ${file_name})

  message(STATUS "Using git: ${GIT_PROGRAM} (${GIT_VERSION})")
  message(STATUS "Template file: ${GGVF_TEMPLATE_FILE}")
  message(STATUS "Output directory: ${GGVF_OUTPUT_DIRECTORY}")
  message(STATUS "Output file: ${file_name}")


  configure_file(
    ${GGVF_TEMPLATE_FILE}
    ${GGVF_OUTPUT_DIRECTORY}/${file_name}
    @ONLY
  )

  message(STATUS "${PROJECT_NAME} version: ${GIT_SHORT}")
  unset(GIT_VERSION)
endfunction()

