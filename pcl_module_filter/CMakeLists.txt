cmake_minimum_required(VERSION 3.5)

# 工程信息
project(pcl_module_filter)

include_directories(${CMAKE_SOURCE_DIR}/..)

add_subdirectory(1_PassThrough)
add_subdirectory(2_VoxelGrid)
add_subdirectory(3_StatisticalOutlierRemoval)
add_subdirectory(4_ProjectInliers)
# add_subdirectory(5_BilateralFilter)
add_subdirectory(6_ExtractIndices)
add_subdirectory(7_OutlinerRemoval2)
add_subdirectory(8_RadiusOutlierRemoval)
add_subdirectory(9_NormalRefinement)
add_subdirectory(10_MedianFilter)

