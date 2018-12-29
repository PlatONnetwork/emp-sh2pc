cmake_minimum_required (VERSION 3.0)
if(POLICY CMP0054)
	 cmake_policy(SET CMP0054 NEW)
endif()

project (emp-sh2pc)
set(NAME "emp-sh2pc")
set (CMAKE_CXX_STANDARD 11) 


SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(EMP_TOOL_CMAKE ${CURRENT_DIR}/../emp-tool/cmake)

# will set on the upper cmakelist
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

#find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${EMP_TOOL_CMAKE}/common.cmake)
include(${EMP_TOOL_CMAKE}/source_of_randomness.cmake)
include(${EMP_TOOL_CMAKE}/threading.cmake)

# setup directory where we should look for cmake files
list(APPEND CMAKE_MODULE_PATH "${EMP_TOOL_CMAKE}")
list(APPEND CMAKE_MODULE_PATH ${CURRENT_DIR}/cmake)
message(STATUS "cmake path: ${CMAKE_MODULE_PATH}")
find_package(OpenSSL)


if(WIN32)
    link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    # for windows socket
    link_libraries(Ws2_32)
else()
    find_package(GMP)

    link_directories(${CMAKE_BINARY_DIR}/lib)
	include_directories(${GMP_INCLUDE_DIR})
	link_libraries(${GMP_LIBRARIES})
endif()

# protobuf dependencies
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	if (WIN32)
		SET(LIB_PROTOBUF libprotobufd)
	else()
		SET(LIB_PROTOBUF protobufd)
	endif()
else()
	if (WIN32)
		SET(LIB_PROTOBUF libprotobuf)
	else()
		SET(LIB_PROTOBUF protobuf)
	endif()
endif()

if(OT_NP_USE_MIRACL)
	message(STATUS "Use MIRACL base libraries !")
	ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
	include_directories(../miracl/include)
	link_libraries(miracl)
else()
	message(STATUS "Use RELIC base libraries !")
	find_package(relic) # compile relic should refer to emp-readme github repository (https://github.com/emp-toolkit/emp-readme)
	#set(RELIC_LIBRARIES relic_s)
	include_directories(${RELIC_INCLUDE_DIR})
	message(STATUS "RELIC_INCLUDE_DIR: ${RELIC_INCLUDE_DIR}")
endif()

# emp-tool, emp-ot include
include_directories(${CURRENT_DIR}/../emp-tool)
include_directories(${CURRENT_DIR}/../emp-tool/include)
include_directories(${CURRENT_DIR}/../emp-ot)
include_directories(${CURRENT_DIR})

# Installation
install(FILES cmake/emp-sh2pc-config.cmake DESTINATION cmake/)
install(DIRECTORY emp-sh2pc DESTINATION include/)

# Test cases
macro (emp_sh2pc_test _name)
	message(STATUS "emp ot test: ${_name}")

	#aux_source_directory(./testutils EXE_SRC_LIST)
    add_executable(emp_sh2pc_${_name} ./test/${_name}.cpp)

	if(NOT OT_NP_USE_MIRACL)
		if(WIN32)
			message(FATAL "Not support relic on Windows !")
		endif()

		target_link_libraries(emp_sh2pc_${_name} ${OPENSSL_LIBRARIES} ${RELIC_LIBRARIES} ${GMP_LIBRARIES} ${LIB_PROTOBUF} emp-tool)
	else()
		if(NOT WIN32)
			target_link_libraries(emp_sh2pc_${_name} ${OPENSSL_LIBRARIES} ${GMP_LIBRARIES} ${LIB_PROTOBUF} emp-tool)
		else()	
			target_link_libraries(emp_sh2pc_${_name} ${OPENSSL_LIBRARIES} ${LIB_PROTOBUF} emp-tool_static)
		endif()
	endif()
	SET_TARGET_PROPERTIES(emp_sh2pc_${_name} PROPERTIES FOLDER "emp-tool-test")
endmacro()

# Tests
emp_sh2pc_test(bit)
emp_sh2pc_test(int)
emp_sh2pc_test(float)
emp_sh2pc_test(circuit_file)
emp_sh2pc_test(example)
