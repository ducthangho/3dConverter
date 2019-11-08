# CMakeList.txt : CMake project for 3dConverter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
option(WITH_ANYSCENEIMPORTER "Build AnySceneImporter plugin" ON)
option(WITH_TRADE "Build Build Trade library plugin" ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project (3dConverter VERSION 1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${PROJECT_SOURCE_DIR}/magnum-plugins/src/MagnumPlugins)
# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/magnum-plugins/modules/" ${CMAKE_MODULE_PATH})

# Add source to this project's executable.
add_executable (3dConverter "3dConverter.cpp" "myIoStream.cpp" "assimpConverter.cpp" "IfcAssImpImporter.cpp")
find_package(assimp CONFIG REQUIRED)
message(STATUS "assimp = ${assimp_FOUND}" )
#find_package(Magnum CONFIG REQUIRED)
    # Note: 1 target(s) were omitted.
#    target_link_libraries(main PRIVATE Magnum::Magnum Magnum::GLContext Magnum::Application Magnum::${_component})
find_package(clipp CONFIG REQUIRED)
find_package(IFCPP CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Magnum REQUIRED Trade)
find_package(MagnumPlugins CONFIG REQUIRED AssimpImporter TinyGltfImporter)
# find_package(MagnumPlugins CONFIG REQUIRED AssimpImporter tinygltfimporter)

# Global plugin include dir
find_path(MAGNUMPLUGINS_INCLUDE_DIR MagnumPlugins
	HINTS ${MAGNUM_INCLUDE_DIR})

	IF (MINIZIP_INCLUDE_DIR)
	# Already in cache, be silent
	SET(MINIZIP_FIND_QUIETLY TRUE)
  ENDIF (MINIZIP_INCLUDE_DIR)
  
  FIND_PATH(MINIZIP_INCLUDE_DIR minizip/zip.h)
  
  SET(MINIZIP_NAMES minizip )
  FIND_LIBRARY(MINIZIP_LIBRARY NAMES ${MINIZIP_NAMES} )
  MARK_AS_ADVANCED( MINIZIP_LIBRARY MINIZIP_INCLUDE_DIR )
  
  # Per-recommendation
  SET(MINIZIP_INCLUDE_DIRS "${MINIZIP_INCLUDE_DIR}")
  SET(MINIZIP_LIBRARIES    "${MINIZIP_LIBRARY}")
  
  # handle the QUIETLY and REQUIRED arguments and set MINIZIP_FOUND to TRUE if 
  # all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(MINIZIP DEFAULT_MSG MINIZIP_LIBRARIES MINIZIP_INCLUDE_DIRS)

message(STATUS ${MAGNUMPLUGINS_INCLUDE_DIR})
mark_as_advanced(MAGNUMPLUGINS_INCLUDE_DIR)
    
#include_directories(${ASSIMP_INCLUDE_DIRS})
target_include_directories(3dConverter 
	PUBLIC	
		$<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>			
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${MAGNUMPLUGINS_INCLUDE_DIR}
		${MINIZIP_INCLUDE_DIRS}
)
target_compile_features(3dConverter PRIVATE cxx_std_17)

#find_package(BOOST 1.58.0 REQUIRED)
message(STATUS "${IFCPP_LIBRARIES} loaded" )

target_link_libraries(3dConverter    
	PRIVATE 
		assimp::assimp
		clipp::clipp
		IFCPP::IfcPlusPlus
		ZLIB::ZLIB
		Magnum::Trade
		MagnumPlugins::TinyGltfImporter
		MagnumPlugins::AssimpImporter		
)


# TODO: Add tests and install targets if needed.
