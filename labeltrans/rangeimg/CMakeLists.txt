cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)

project(rangeimg )

find_package(OpenCV REQUIRED)
find_package(PCL 1.3 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

add_library(projection SHARED
  GetRangeimgFromCloud.cpp
  readkitti.cpp
  )

target_link_libraries(projection
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES} 
  )

add_executable(rangeimg main.cpp)
target_link_libraries(rangeimg 
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES} 
  projection
  )

set(CMAKE_BUILD_TYPE Debug)
