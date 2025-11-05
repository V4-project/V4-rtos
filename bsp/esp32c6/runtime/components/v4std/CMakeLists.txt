# V4-std ESP-IDF Component
# Links to V4-std library in V4-project/V4-std

set(V4STD_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../../V4-std")

# V4-std source files
set(COMPONENT_SRCS
    "${V4STD_DIR}/src/ddt.cpp"
    "${V4STD_DIR}/src/sys_handlers.cpp"
    "${V4STD_DIR}/src/sys_led.cpp"
)

# Include directories
set(COMPONENT_ADD_INCLUDEDIRS
    "${V4STD_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"  # For generated sys_ids.h
)

set(COMPONENT_REQUIRES)

register_component()

# Generate sys_ids.h from v4sys_ids.def
set(V4SYS_IDS_DEF "${V4STD_DIR}/include/v4std/v4sys_ids.def")
set(V4SYS_IDS_H "${CMAKE_CURRENT_BINARY_DIR}/generated/v4std/sys_ids.h")

add_custom_command(
  OUTPUT "${V4SYS_IDS_H}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/generated/v4std"
  COMMAND ${CMAKE_COMMAND}
          -DINPUT_FILE=${V4SYS_IDS_DEF}
          -DOUTPUT_FILE=${V4SYS_IDS_H}
          -P ${V4STD_DIR}/cmake/generate_sys_ids.cmake
  DEPENDS "${V4SYS_IDS_DEF}" "${V4STD_DIR}/cmake/generate_sys_ids.cmake"
  COMMENT "Generating sys_ids.h from v4sys_ids.def"
  VERBATIM
)

add_custom_target(generate_sys_ids_esp DEPENDS "${V4SYS_IDS_H}")
add_dependencies(${COMPONENT_LIB} generate_sys_ids_esp)

# Add generated directory to include path
target_include_directories(${COMPONENT_LIB} PUBLIC
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
