cmake_minimum_required(VERSION 3.10)
project(can)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SRC_FILES
	"${CMAKE_CURRENT_LIST_DIR}/src/*.cc"
)

file (GLOB_RECURSE INC_FILES
	"${CMAKE_CURRENT_LIST_DIR}/inc/*.h"
)

include_directories(
	"${CMAKE_CURRENT_LIST_DIR}/inc"
)

message("${INC_FILES}")

add_library(${PROJECT_NAME}_static STATIC ${SRC_FILES} ${INC_FILES})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "can")
set_target_properties(${PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${INC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "can")
set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
add_executable(StepMotorCtl test/test.cc)

target_link_libraries(StepMotorCtl ${PROJECT_NAME} pthread)

# out set
set (OUT_BIN ${CMAKE_BINARY_DIR}/target/lib)
set (OUT_LIB ${CMAKE_BINARY_DIR}/target/lib)
set (OUT_INC ${CMAKE_BINARY_DIR}/target/include)


set_target_properties(${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${OUT_LIB}"
  LIBRARY_OUTPUT_DIRECTORY "${OUT_LIB}"
  RUNTIME_OUTPUT_DIRECTORY "${OUT_BIN}"
)

add_custom_target(copy_files ALL  
	VERBATIM
	COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_INC}"
	COMMAND ${CMAKE_COMMAND} -E
                  copy_directory
                  "${CMAKE_CURRENT_LIST_DIR}/inc"
                  "${OUT_INC}"
)
