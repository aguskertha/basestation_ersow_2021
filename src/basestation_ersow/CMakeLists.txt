##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(basestation_ersow)
add_compile_options(-std=c++11)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS 
	qt_build 
	roscpp 
	roslaunch 
	geometry_msgs
	message_generation
    std_msgs 
	)

include_directories(${catkin_INCLUDE_DIRS})
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 

add_message_files (
    FILES 
    	data_agent0.msg
	strategyMsg.msg
	ifaceMsg.msg
	refereeMsg.msg
	dataAgentMsg.msg
	data_gui.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
    CATKIN_DEPENDS message_runtime
    INCLUDE_DIRS include
  	LIBRARIES ersow_comm
  	CATKIN_DEPENDS roscpp std_msgs
	CATKIN_DEPENDS message_runtime std_msgs
 	DEPENDS system_lib
)

include_directories(${catkin_INCLUDE_DIRS})

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
# SET (TARGET_SRC ${TARGET_CPP} ${TARGET_H})
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/basestation_ersow/*.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(basestation_ersow ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(basestation_ersow ${QT_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS basestation_ersow RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


