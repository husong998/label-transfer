cmake_minimum_required (VERSION 3.1)
project (labeltrans)

find_package(OpenCV REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(showRGB SHARED
	projection.cpp
	get_3d_bbox.cpp
	)
target_link_libraries(showRGB ${OpenCV_LIBS})

add_executable(main main.cpp)
target_link_libraries(main 
	showRGB 
	${OpenCV_LIBS}
	)

set(CMAKE_BUILD_TYPE Debug)
