# CMakeList.txt : CMake project for 3dConverter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
#   set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
# endif()

set(VCPKG_TARGET_TRIPLET "x64-linux")
set(VCPKG_DEFAULT_TRIPLET "x64-linux")

message (STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

project (3dConverter VERSION 1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/magnum-plugins/src/MagnumPlugins)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "/home/ducthangho/vcpkg/installed/x64-linux/share/clipp/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/magnum-plugins/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/OpenSceneGraph/CMakeModules/" ${CMAKE_MODULE_PATH})
set(clipp_DIR "/home/ducthangho/vcpkg/installed/x64-linux/share/clipp")
set(Flatbuffers_DIR "/home/ducthangho/vcpkg/installed/x64-linux/share/flatbuffers")
message (STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# Add source to this project's executable.
add_executable (3dConverter "3dConverter.cpp" "myIoStream.cpp" "assimpConverter.cpp" "Convert2AssImp.cpp" "IfcAssImpImporter.cpp")


# message("Your CMake version supports these C++ features:")
# get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
# foreach(i ${known_features})
#   message("${i}")
# endforeach()

target_compile_features(3dConverter PRIVATE cxx_std_17)
find_package(assimp CONFIG REQUIRED)
message(STATUS "VCPKG_ROOT = ${CMAKE_TOOLCHAIN_FILE}" )
message(STATUS "assimp = ${ASSIMP_LIBRARIES}" )
#find_package(Magnum CONFIG REQUIRED)
    # Note: 1 target(s) were omitted.
#    target_link_libraries(main PRIVATE Magnum::Magnum Magnum::GLContext Magnum::Application Magnum::${_component})
find_package(clipp CONFIG REQUIRED)
find_package(IFCPP CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Flatbuffers CONFIG REQUIRED)
find_package(OSG COMPONENTS osgGA osgUtil osgDB osgText osgWidget osgTerrain osgFX osgViewer osgVolume osgManipulator osgAnimation osgParticle osgShadow osgPresentation osgSim OpenThreads)
# find_package(Magnum REQUIRED Trade)
# find_package(MagnumPlugins CONFIG REQUIRED AssimpImporter TinyGltfImporter)
# find_package(MagnumPlugins CONFIG REQUIRED AssimpImporter tinygltfimporter)
message ("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# Global plugin include dir
# find_path(MAGNUMPLUGINS_INCLUDE_DIR MagnumPlugins
# 	HINTS ${MAGNUM_INCLUDE_DIR})

# 	IF (MINIZIP_INCLUDE_DIR)
# 	# Already in cache, be silent
# 	SET(MINIZIP_FIND_QUIETLY TRUE)
#   ENDIF (MINIZIP_INCLUDE_DIR)
  
#   FIND_PATH(MINIZIP_INCLUDE_DIR minizip/zip.h)
  
#   SET(MINIZIP_NAMES minizip )
#   FIND_LIBRARY(MINIZIP_LIBRARY NAMES ${MINIZIP_NAMES} )
#   MARK_AS_ADVANCED( MINIZIP_LIBRARY MINIZIP_INCLUDE_DIR )
  
  # Per-recommendation
  SET(MINIZIP_INCLUDE_DIRS "${MINIZIP_INCLUDE_DIR}")
  SET(MINIZIP_LIBRARIES    "${MINIZIP_LIBRARY}")
  
  # handle the QUIETLY and REQUIRED arguments and set MINIZIP_FOUND to TRUE if 
  # all listed variables are TRUE
#   INCLUDE(FindPackageHandleStandardArgs)
#   FIND_PACKAGE_HANDLE_STANDARD_ARGS(MINIZIP DEFAULT_MSG MINIZIP_LIBRARIES MINIZIP_INCLUDE_DIRS)

# message(STATUS ${MAGNUMPLUGINS_INCLUDE_DIR})
# mark_as_advanced(MAGNUMPLUGINS_INCLUDE_DIR)
    
#include_directories(${ASSIMP_INCLUDE_DIRS})
message(STATUS "${ASSIMP_INCLUDE_DIRS}")
target_include_directories(3dConverter 
	PUBLIC	
		$<INSTALL_INTERFACE:include>    
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>			
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assimp>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assimp/code>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		# ${ASSIMP_INCLUDE_DIRS}		
		# ${MINIZIP_INCLUDE_DIRS}
		${OSG_INCLUDE_DIR}
)

#find_package(BOOST 1.58.0 REQUIRED)
message(STATUS "${IFCPP_LIBRARIES} loaded" )

target_link_libraries(3dConverter    
	PRIVATE 
		# assimp::assimp
		${ASSIMP_LIBRARIES}
		clipp::clipp
		IFCPP::IfcPlusPlus
		ZLIB::ZLIB
		flatbuffers::flatbuffers		
		${OSG_LIBRARIES}
		${OSG_LIBRARY}
		${OSGUTIL_LIBRARY}
		${OSGTEXT_LIBRARY}
		${OSGVIEWER_LIBRARY}
		# Magnum::Trade
		# MagnumPlugins::TinyGltfImporter
		# MagnumPlugins::AssimpImporter		
)


# TODO: Add tests and install targets if needed.
