cmake_minimum_required(VERSION 3.5)

project(pointcloud_segmentation_demos)

set( CMAKE_CXX_FLAGS "-std=c++11" )
set( CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}" )

set(CMAKE_CONFIGRATION_TYPES "Release" CACHE STRING "" FORCE)

# find_package(PCL 1.9 REQUIRED COMPONENTS common io)
find_package(PCL 1.9 REQUIRED)

# include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})

# 库目录
# link_directories(${PCL_LIBRARY_DIRS})
link_directories(${PCL_ROOT}/3rdParty/OpenNI2/Lib)
link_directories(${PCL_ROOT}/D:/PCL 1.9.1/3rdParty/VTK/lib)
link_directories(${PCL_ROOT}/D:/PCL 1.9.1/3rdParty/Qhull/lib)
link_directories(${PCL_ROOT}/D:/PCL 1.9.1/3rdParty/FLANN/lib)
link_directories(${PCL_ROOT}/D:/PCL 1.9.1/3rdParty/Boost/lib)
link_directories(${PCL_ROOT}/D:/PCL 1.9.1/lib)

# 包含目录
include_directories(${PCL_ROOT}/3rdParty/OpenNI2/Include)
include_directories(${PCL_ROOT}/3rdParty/VTK/include/vtk-8.1)
include_directories(${PCL_ROOT}/3rdParty/Qhull/include)
include_directories(${PCL_ROOT}/3rdParty/FLANN/include)
include_directories(${PCL_ROOT}/3rdParty/Eigen/eigen3)
include_directories(${PCL_ROOT}/3rdParty/Boost/include/boost-1_68)
include_directories(${PCL_ROOT}/include/pcl-1.9)

# opencv库查询
set(OpenCV_DIR $ENV{OPENCV_DIR})
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(${OpenCV_DIR}/install/include)
include_directories(${OpenCV_DIR}/install/include/opencv)
include_directories(${OpenCV_DIR}/install/include/opencv2)

# 查找源文件
# aux_source_directory(. DIR_SRCS)

set(EXE_NAME_1 1_planner_segmentation)
add_executable( ${EXE_NAME_1} 1_简单平面点云分割.cpp )
target_link_libraries( ${EXE_NAME_1} ${OpenCV_LIBS} ${PCL_LIBRARIES})



# set(EXE_NAME 01_DisplayCloudAsHeight)
# add_executable(${EXE_NAME} ${DIR_SRCS})

# target_link_libraries(${EXE_NAME} ${PCL_LIBRARIES})
# target_link_libraries(pcd_write_test ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES})