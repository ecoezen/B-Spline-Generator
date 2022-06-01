cmake_minimum_required( VERSION 3.4.3 )

# Project name and programming language
project( linalg CXX )

# Gather files and store paths in CMake variables
file( GLOB LINALG_LIBRARY_SOURCE_FILES src/*.cpp )
file( GLOB LINALG_TEST_SOURCE_FILES test/*.cpp )
file( GLOB LINALG_HEADER_FILES inc/*.h* )

# Specify the folders with header files, otherwise our includes are not found
include_directories( external/catch )
include_directories( inc )

# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}" )

# Enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# When compiling with gcc we need to compile libraries with fPIC (stands for position independent code).
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pedantic -Wall -Werror -std=c++11" )
endif( CMAKE_COMPILER_IS_GNUCXX )

# Add compilation target for our library
add_library( linalg SHARED ${LINALG_LIBRARY_SOURCE_FILES} ${LINALG_HEADER_FILES} )

# specify the relative path the shared library object shall be installed to

if( WIN32 )
    install( TARGETS linalg RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
    install( TARGETS linalg LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

# Add test runner
add_executable( linalg_testrunner ${LINALG_TEST_SOURCE_FILES} )
target_link_libraries( linalg_testrunner linalg )

install( TARGETS linalg_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
