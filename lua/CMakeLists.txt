cmake_minimum_required(VERSION 3.20) # change this to your needs
project(luaxclip VERSION 1.0 LANGUAGES CXX C)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
# find_package(PythonLibs)
# find_package(Python3 COMPONENTS Interpreter Development)
# https://stackoverflow.com/questions/46919807/cmake-how-to-include-and-link-lua-5-1-lib
find_package(Lua51 REQUIRED)
include_directories("P:/workflow/dev/lua/lua-5.1.5_Win64_dll17_lib/include" ${LUA_INCLUDE_DIR})

# FIND_PACKAGE(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)
# include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_SWIG_FLAGS "")
set(CMAKE_SWIG_OUTDIR ${PROJECT_SOURCE_DIR}/luaxclip)
set(SWIG_OUTFILE_DIR ${PROJECT_SOURCE_DIR}/luaxclip)
set_source_files_properties(xclip.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(xclip.i PROPERTIES USE_SWIG_DEPENDENCIES TRUE)
set_property(SOURCE xclip.i PROPERTY SWIG_MODULE_NAME luaxclip)
set_property(SOURCE xclip.i PROPERTY CPLUSPLUS ON)
swig_add_library(luaxclip 
LANGUAGE lua 
SOURCES xclip.i
)

# target_link_libraries(xcl PRIVATE Python3::Module)
target_link_libraries(luaxclip ${LUA_LIBRARIES})

file(GLOB xclip_SRCS
        ../src/xclip/*.cpp
        )
target_sources( luaxclip PRIVATE 
    ${xclip_SRCS} 
)

# CMAKE_BUILD_TYPE is empty... we need to hardcode the output dir...
add_custom_command(TARGET luaxclip POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/Release/luaxclip.dll
                                    ${SWIG_OUTFILE_DIR}/luaxclip.dll
)
