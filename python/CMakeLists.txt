cmake_minimum_required(VERSION 3.20) # change this to your needs
project(pyxclip VERSION 1.0 LANGUAGES CXX C)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
# find_package(PythonLibs)
find_package(Python3 COMPONENTS Interpreter Development)
# FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_SWIG_FLAGS "")
set(CMAKE_SWIG_OUTDIR ${PROJECT_SOURCE_DIR}/pyxclip)
set(SWIG_OUTFILE_DIR ${PROJECT_SOURCE_DIR}/pyxclip)

set_source_files_properties(xclip.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(xclip.i PROPERTIES USE_SWIG_DEPENDENCIES TRUE)
set_property(SOURCE xclip.i PROPERTY SWIG_MODULE_NAME pyxclip)
set_property(SOURCE xclip.i PROPERTY CPLUSPLUS ON)
swig_add_library(pyxclip 
LANGUAGE python 
SOURCES xclip.i
)
# target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})
target_link_libraries(pyxclip PRIVATE Python3::Module)

# include_directories(../src/xclip)
file(GLOB xclip_SRCS
        ../src/xclip/*.cpp
        )
target_sources( pyxclip PRIVATE 
    ${xclip_SRCS} 
)


# setting RUNTIME_OUTPUT_DIRECTORY_RELEASE we should get rid of this but it doesn't work....
# https://jeremimucha.com/2021/02/cmake-variable-guidelines/
# https://stackoverflow.com/questions/46371176/why-arent-binaries-placed-in-cmake-current-binary-dir
# https://stackoverflow.com/questions/51353110/how-do-i-output-the-result-of-a-generator-expression-in-cmake
# message("RUNTIME_OUTPUT_DIRECTORY_RELEASE: ${RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
# message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
# message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
# message("RUNTIME_OUTPUT_DIRECTORY: ${RUNTIME_OUTPUT_DIRECTORY}")
add_custom_command(TARGET pyxclip POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo 
  "TARGET_FILE_DIR: $<TARGET_FILE_DIR:pyxclip>"
)
# add_custom_command(TARGET pyxclip POST_BUILD
    # COMMAND ${CMAKE_COMMAND}  -E copy_if_different $<TARGET_FILE_DIR:pyxclip>/_pyxclip.pyd
                                    # ${SWIG_OUTFILE_DIR}/_pyxclip.pyd
# )
# https://cmake.org/cmake/help/v3.0/manual/cmake-generator-expressions.7.html
add_custom_command(TARGET pyxclip POST_BUILD
    COMMAND ${CMAKE_COMMAND}  -E copy_if_different $<TARGET_FILE:pyxclip>
                                    ${SWIG_OUTFILE_DIR}/$<TARGET_FILE_NAME:pyxclip>
)
