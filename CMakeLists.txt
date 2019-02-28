cmake_minimum_required (VERSION 3.0)
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project (emp-sh2pc)
set(NAME "emp-sh2pc")
set (CMAKE_CXX_STANDARD 11) 


SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(EMP_TOOL_CMAKE ${CURRENT_DIR}/../emp-tool/cmake)

# setup directory where we should look for cmake files
list(APPEND CMAKE_MODULE_PATH "${EMP_TOOL_CMAKE}")
list(APPEND CMAKE_MODULE_PATH ${CURRENT_DIR}/cmake)

#find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${EMP_TOOL_CMAKE}/common.cmake)
include(${EMP_TOOL_CMAKE}/source_of_randomness.cmake)
include(${EMP_TOOL_CMAKE}/threading.cmake)


###### includes here

# emp-tool, emp-ot include
include_directories(${CURRENT_DIR}/../emp-tool)
include_directories(${CURRENT_DIR}/../emp-tool/include)
include_directories(${CURRENT_DIR}/../emp-ot)
include_directories(${CURRENT_DIR})


###### libraries here
if(WIN32)
	link_libraries(Ws2_32)
endif()

link_relic_or_miracl() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_protobuf() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake

# Installation
install(FILES cmake/emp-sh2pc-config.cmake DESTINATION cmake/)
install(DIRECTORY emp-sh2pc DESTINATION include/)

# Test cases
macro (emp_sh2pc_test _name)
	message(STATUS "emp sh2pc test project name: ${_name}")

	#aux_source_directory(./testutils EXE_SRC_LIST)
    add_executable(emp_sh2pc_${_name} ./test/${_name}.cpp)
    target_link_libraries(emp_sh2pc_${_name} emp-tool)
    
    #IF(NOT WIN32 OR OT_NP_USE_RELIC_WIN)
    #    target_link_libraries(emp_sh2pc_${_name} ${GMP_LIBRARIES})
    #ENDIF()

	SET_TARGET_PROPERTIES(emp_sh2pc_${_name} PROPERTIES FOLDER "emp-toolkit-test")
endmacro()

# Tests
emp_sh2pc_test(bit)
emp_sh2pc_test(int)
emp_sh2pc_test(float)
emp_sh2pc_test(circuit_file)
emp_sh2pc_test(example)
