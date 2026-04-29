
add_library(vdot_extern INTERFACE)
add_library(vdot::extern ALIAS vdot_extern)

# ========================== godot-cpp ========================== #

# godot-cpp
# From here: https://github.com/godotengine/godot-cpp
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp")
  message(FATAL_ERROR
    "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. "
    "Please update submodules: git submodule update --init --recursive."
  )
endif()

set(GODOTCPP_USE_HOT_RELOAD OFF CACHE BOOL "" FORCE)
add_subdirectory(godot-cpp SYSTEM)
set_target_properties(godot-cpp PROPERTIES CXX_VISIBILITY_PRESET hidden)

list(APPEND CMAKE_MODULE_PATH ${godot-cpp_SOURCE_DIR}/cmake)
include(GodotCPPModule)

get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

set(GODOTCPP_SUFFIX ${GODOTCPP_SUFFIX} PARENT_SCOPE)
set(GODOTCPP_PLATFORM ${GODOTCPP_PLATFORM} PARENT_SCOPE)

target_link_libraries(vdot_extern INTERFACE godot-cpp)

# ========================== simdjson =========================== #

add_subdirectory(simdjson)
set_target_properties(simdjson PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(vdot_extern INTERFACE simdjson::simdjson)

