cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project (labeltrans)

find_package(OpenCV REQUIRED)
find_package(PCL 1.3 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})

add_library(CustomLib SHARED
	projection.cpp
	get_3d_bbox.cpp
	rangeimg/readkitti.cpp
	rangeimg/GetRangeimgFromCloud.cpp
	)
target_link_libraries(CustomLib 
	${OpenCV_LIBS}
	${PCL_LIBRARIES}
	)

add_executable(main main.cpp)
target_link_libraries(main 
	CustomLib 
	${OpenCV_LIBS}
	${PCL_LIBRARIES}
	)

set(CMAKE_BUILD_TYPE Debug)
