#include <iostream>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/segmentation/region_growing_rgb.h>

int main(int argc, char** argv)
{
	pcl::search::Search <pcl::PointXYZRGB>::Ptr tree = boost::shared_ptr<pcl::search::Search<pcl::PointXYZRGB> >(new pcl::search::KdTree<pcl::PointXYZRGB>);

	pcl::PointCloud <pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud <pcl::PointXYZRGB>);

	if (pcl::io::loadPCDFile <pcl::PointXYZRGB>("table_scene_lms400.pcd", *cloud) == -1)
	{
		std::cout << "Cloud reading failed." << std::endl;
		return (-1);
	}

	//�洢���Ƶ�����
	pcl::IndicesPtr indices(new std::vector <int>);  //�˲�
	pcl::PassThrough<pcl::PointXYZRGB> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 1.0);
	pass.filter(*indices);   //������ɫ���������ɵĶ���
	pcl::RegionGrowingRGB<pcl::PointXYZRGB> reg;
	reg.setInputCloud(cloud);
	reg.setIndices(indices);

	//���Ƶ����� 
	reg.setSearchMethod(tree);
	reg.setDistanceThreshold(10);  //����ķ�ֵ
	reg.setPointColorThreshold(6);  //�����֮����ɫ�ݲ�
	reg.setRegionColorThreshold(5);  //����֮���ݲ�
	reg.setMinClusterSize(600);       //���þ���Ĵ�С
	std::vector <pcl::PointIndices> clusters;
	reg.extract(clusters);

	pcl::PointCloud <pcl::PointXYZRGB>::Ptr colored_cloud = reg.getColoredCloud();

	pcl::visualization::CloudViewer viewer("Cluster viewer");
	viewer.showCloud(colored_cloud);  while (!viewer.wasStopped())
	{
		boost::this_thread::sleep(boost::posix_time::microseconds(100));
	}

	return 0;
}

