cmake_minimum_required(VERSION 3.20) # change this to your needs
project(xclip VERSION 1.0 LANGUAGES CXX C)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
# find_package(PythonLibs)
find_package(Python3 COMPONENTS Interpreter Development)
# FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_SWIG_FLAGS "")
set(CMAKE_SWIG_OUTDIR ${PROJECT_SOURCE_DIR}/xclip)
set(SWIG_OUTFILE_DIR ${PROJECT_SOURCE_DIR}/xclip)
set_source_files_properties(xclip.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(xclip.i PROPERTIES USE_SWIG_DEPENDENCIES TRUE)
set_property(SOURCE xclip.i PROPERTY SWIG_MODULE_NAME xclip)
set_property(SOURCE xclip.i PROPERTY CPLUSPLUS ON)
swig_add_library(xclip 
LANGUAGE python 
SOURCES xclip.i
)
# target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})
target_link_libraries(xclip PRIVATE Python3::Module)

# include_directories(../src/xclip)
file(GLOB xclip_SRCS
        ../src/xclip/*.cpp
        )
target_sources( xclip PRIVATE 
    ${xclip_SRCS} 
)

# CMAKE_BUILD_TYPE is empty... we need to hardcode the output dir...
add_custom_command(TARGET xclip POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/Release/_xclip.pyd
                                    ${SWIG_OUTFILE_DIR}/_xclip.pyd
)
    