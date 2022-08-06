# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "basestation_ersow: 6 messages, 0 services")

set(MSG_I_FLAGS "-Ibasestation_ersow:/home/agus/basestation_ersow/src/basestation_ersow/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(basestation_ersow_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" ""
)

get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" ""
)

get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" ""
)

get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" ""
)

get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" ""
)

get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_custom_target(_basestation_ersow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basestation_ersow" "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_cpp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
)

### Generating Services

### Generating Module File
_generate_module_cpp(basestation_ersow
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(basestation_ersow_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(basestation_ersow_generate_messages basestation_ersow_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_cpp _basestation_ersow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basestation_ersow_gencpp)
add_dependencies(basestation_ersow_gencpp basestation_ersow_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basestation_ersow_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)
_generate_msg_eus(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
)

### Generating Services

### Generating Module File
_generate_module_eus(basestation_ersow
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(basestation_ersow_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(basestation_ersow_generate_messages basestation_ersow_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_eus _basestation_ersow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basestation_ersow_geneus)
add_dependencies(basestation_ersow_geneus basestation_ersow_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basestation_ersow_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)
_generate_msg_lisp(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
)

### Generating Services

### Generating Module File
_generate_module_lisp(basestation_ersow
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(basestation_ersow_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(basestation_ersow_generate_messages basestation_ersow_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_lisp _basestation_ersow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basestation_ersow_genlisp)
add_dependencies(basestation_ersow_genlisp basestation_ersow_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basestation_ersow_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)
_generate_msg_nodejs(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
)

### Generating Services

### Generating Module File
_generate_module_nodejs(basestation_ersow
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(basestation_ersow_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(basestation_ersow_generate_messages basestation_ersow_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_nodejs _basestation_ersow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basestation_ersow_gennodejs)
add_dependencies(basestation_ersow_gennodejs basestation_ersow_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basestation_ersow_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)
_generate_msg_py(basestation_ersow
  "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
)

### Generating Services

### Generating Module File
_generate_module_py(basestation_ersow
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(basestation_ersow_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(basestation_ersow_generate_messages basestation_ersow_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/refereeMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/ifaceMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/dataAgentMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_agent0.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/strategyMsg.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/agus/basestation_ersow/src/basestation_ersow/msg/data_gui.msg" NAME_WE)
add_dependencies(basestation_ersow_generate_messages_py _basestation_ersow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basestation_ersow_genpy)
add_dependencies(basestation_ersow_genpy basestation_ersow_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basestation_ersow_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basestation_ersow
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(basestation_ersow_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basestation_ersow
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(basestation_ersow_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basestation_ersow
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(basestation_ersow_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basestation_ersow
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(basestation_ersow_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basestation_ersow
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(basestation_ersow_generate_messages_py std_msgs_generate_messages_py)
endif()
