# CMakeList.txt : CMake project for 3dConverter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project (3dConverter VERSION 1.0 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Add source to this project's executable.
add_executable (3dConverter "3dConverter.cpp" "myIoStream.cpp")

find_package(assimp CONFIG REQUIRED)
#find_package(Magnum CONFIG REQUIRED)
    # Note: 1 target(s) were omitted.
#    target_link_libraries(main PRIVATE Magnum::Magnum Magnum::GLContext Magnum::Application Magnum::${_component})
find_package(clipp CONFIG REQUIRED)
find_package(IFCPP CONFIG REQUIRED)
    
#include_directories(${ASSIMP_INCLUDE_DIRS})
target_include_directories(3dConverter 
	PUBLIC	
		$<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>			
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(3dConverter PRIVATE cxx_std_17)

#find_package(BOOST 1.58.0 REQUIRED)
message(STATUS "${IFCPP_LIBRARIES} loaded" )

target_link_libraries(3dConverter    
	PRIVATE 
		assimp::assimp
		clipp::clipp
		IFCPP::IfcPlusPlus
)


# TODO: Add tests and install targets if needed.
