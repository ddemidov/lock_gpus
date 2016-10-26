cmake_minimum_required(VERSION 2.8)
project(lock_gpus)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#----------------------------------------------------------------------------
# Find Boost
#----------------------------------------------------------------------------
find_package(Boost COMPONENTS program_options system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )

#----------------------------------------------------------------------------
# Find VexCL
#----------------------------------------------------------------------------
set(VEXCL_ROOT $ENV{VEXCL_ROOT} CACHE STRING "VexCL root")
set(VEXCL_BACKEND "OpenCL" CACHE STRING "Select VexCL backend (OpenCL/CUDA/Compute)")
set_property(CACHE VEXCL_BACKEND PROPERTY STRINGS "OpenCL" "CUDA" "Compute")

include_directories( ${VEXCL_ROOT} )

#----------------------------------------------------------------------------
# Find VexCL backend
#----------------------------------------------------------------------------
if ("${VEXCL_BACKEND}" STREQUAL "OpenCL")
    find_package(OpenCL REQUIRED)
    include_directories( ${OPENCL_INCLUDE_DIRS} )
    set(VEXCL_BACKEND_LIBRARIES ${OPENCL_LIBRARIES})
    add_definitions(-DVEXCL_BACKEND_OPENCL)
elseif ("${VEXCL_BACKEND}" STREQUAL "Compute")
    find_package(OpenCL REQUIRED)
    include_directories( ${OPENCL_INCLUDE_DIRS} )
    set(VEXCL_BACKEND_LIBRARIES ${OPENCL_LIBRARIES})
    add_definitions(-DVEXCL_BACKEND_COMPUTE)
elseif ("${VEXCL_BACKEND}" STREQUAL "CUDA")
    find_package(CUDA REQUIRED)
    include_directories( ${CUDA_INCLUDE_DIRS} )
    set(VEXCL_BACKEND_LIBRARIES ${CUDA_CUDA_LIBRARY})
    add_definitions(-DVEXCL_BACKEND_CUDA)
endif()

#----------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-strict-aliasing -Wno-deprecated-declarations -Wno-ignored-attributes")


#----------------------------------------------------------------------------
add_executable(lock_gpus lock_gpus.cpp)
target_link_libraries(lock_gpus ${Boost_LIBRARIES} ${VEXCL_BACKEND_LIBRARIES})
