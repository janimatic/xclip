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

# set_target_properties(pyxclip PROPERTIES
                      # RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/pyxclip
# )
# setting RUNTIME_OUTPUT_DIRECTORY_RELEASE we should get rid of this but it doesn't work....
if(WIN32)
    # CMAKE_BUILD_TYPE is empty... we need to hardcode the output dir /Release under windows...
    add_custom_command(TARGET pyxclip POST_BUILD
        COMMAND ${CMAKE_COMMAND}  -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/Release/_pyxclip.pyd
                                        ${SWIG_OUTFILE_DIR}/_pyxclip.pyd
    )
else()
    add_custom_command(TARGET pyxclip POST_BUILD
        COMMAND ${CMAKE_COMMAND}  -E copy_if_different ${RUNTIME_OUTPUT_DIRECTORY}/_pyxclip.pyd
                                        ${SWIG_OUTFILE_DIR}/_pyxclip.pyd
    )
endif()
    