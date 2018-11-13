cmake_minimum_required(VERSION 3.5)

project(proyecto3 VERSION 0.0.0 LANGUAGES CXX)

## Ensure that all binaries are stored in the same directory (usually build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake") 

## Options
option(ANPI_ENABLE_OpenMP "Force the use of OpenMP" on)
option(BUILD_DOC "Use Doxygen to create the HTML based API documentation" OFF)

## All compiler options
include(CompilerFlags)
include(ExternalLibs)

## Create libraries and executables
add_subdirectory (src)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/AnpiConfig.hpp.in ${CMAKE_SOURCE_DIR}/include/AnpiConfig.hpp)

## Also build unit tests ...
enable_testing ()

if(BUILD_DOC)
  FIND_PACKAGE(Doxygen)
	if(DOXYGEN_FOUND)
	
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc ALL
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
		COMMENT "Generating API documentation with Doxygen"
		)
		
	else (DOXYGEN_FOUND)
		message("Doxygen need to be installed to generate the documentation \nInstall with > sudo apt install doxygen")
		
	endif(DOXYGEN_FOUND)
	
endif(BUILD_DOC)