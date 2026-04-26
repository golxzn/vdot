cmake_minimum_required(VERSION 3.25)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
  message(FATAL_ERROR
    "You cannot build in the source directory. "
    "Please use a build subdirectory."
  )
endif()

# Add paths to modules
list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build universal lib on macOS
# Note that CMAKE_OSX_ARCHITECTURES must be set before project().
if (APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
endif()

# Install the build plugin files to the project directory.
# TODO: CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT SHOULD be true
# for "/usr/local", but for some reason it is not on my machine.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE
    "${CMAKE_CURRENT_SOURCE_DIR}/project/addons"
  )
endif()

# Main project information
project(vdot
  VERSION 0.2.0
  DESCRIPTION ""
  LANGUAGES CXX
)

# ========================== godot-cpp ========================== #

# godot-cpp
# From here: https://github.com/godotengine/godot-cpp
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/godot-cpp")
  message(FATAL_ERROR
    "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. "
    "Please update submodules: git submodule update --init --recursive."
  )
endif()

add_subdirectory(extern/godot-cpp SYSTEM)
set_target_properties(godot-cpp PROPERTIES CXX_VISIBILITY_PRESET hidden)

list(APPEND CMAKE_MODULE_PATH ${godot-cpp_SOURCE_DIR}/cmake)
include(GodotCPPModule)

get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

# ========================= cubism-sdk ========================= #
# From here: https://github.com/Live2D/CubismNativeFramework
# Set directory paths.

include(cmake/CubismSDK.cmake)

# ========================= main target ========================= #

# Create our library
add_library(${PROJECT_NAME} SHARED)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

# BUILD_OUTPUT_DIR is where we put the resulting library (in the build directory)
set(BUILD_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/out/${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN true

  PREFIX ""
  OUTPUT_NAME "${PROJECT_NAME}${GODOTCPP_SUFFIX}"
  RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/bin/${GODOTCPP_PLATFORM}"
  LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/bin/${GODOTCPP_PLATFORM}"
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    godot-cpp
    Framework # CubismSDK
)

# Copy over additional files from the support_files directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/support_files" ${BUILD_OUTPUT_DIR}
)

# Custom target to make CLion targets easier for debugging.
add_custom_target(install_${PROJECT_NAME}
  "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
  DEPENDS ${PROJECT_NAME}
  COMMENT "Installing ${PROJECT_NAME}"
)


# Warnings
include( CompilerWarnings )

add_subdirectory( src )

# Install library, extension file, and support files in ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/" )

message( STATUS "Install directory: ${INSTALL_DIR}")

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${INSTALL_DIR}/${GODOTCPP_PLATFORM}
  RUNTIME DESTINATION ${INSTALL_DIR}/${GODOTCPP_PLATFORM}
)

# Copy over support files
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/support_files/"
  DESTINATION ${INSTALL_DIR} PATTERN ".*" EXCLUDE
)

add_subdirectory(templates)

include(ccache)
include(ClangFormat)


if (PROJECT_IS_TOP_LEVEL AND UNIX)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
endif()
