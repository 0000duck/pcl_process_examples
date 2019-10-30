#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/segmentation/region_growing.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	//���Ƶ�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	//�򿪵���
	if (pcl::io::loadPCDFile <pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) == -1)
	{
		std::cout << "Cloud reading failed." << std::endl;
		return -1;
	}

	//���������ķ�ʽ����˵�ǽṹ
	pcl::search::Search<pcl::PointXYZ>::Ptr tree = boost::shared_ptr<pcl::search::Search<pcl::PointXYZ> >(new pcl::search::KdTree<pcl::PointXYZ>);
	
	//����
	pcl::PointCloud <pcl::Normal>::Ptr normals (new pcl::PointCloud <pcl::Normal>);

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
	normal_estimator.setSearchMethod(tree);
	normal_estimator.setInputCloud(cloud);
	normal_estimator.setKSearch(50);
	normal_estimator.compute(*normals);

	//ֱͨ�˲���Z���0��1��֮��
	pcl::IndicesPtr indices(new std::vector <int>);
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 1.0);
	pass.filter(*indices);  //�������<�㣬����>

	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
	reg.setMinClusterSize(50);  //��С�ľ���ĵ���
	reg.setMaxClusterSize(1000000);  //����
	reg.setSearchMethod(tree);    //������ʽ
	reg.setNumberOfNeighbours(30);    //���������������ĸ���
	reg.setInputCloud(cloud);         //�����  

	// reg.setIndices (indices);
	reg.setInputNormals (normals);     //����ķ���
	reg.setSmoothnessThreshold(3.0 / 180.0 * M_PI);  //����ƽ����
	reg.setCurvatureThreshold(1.0);     //�������ʵķ�ֵ
	std::vector <pcl::PointIndices> clusters;
	reg.extract(clusters); std::cout << "Number of clusters is equal to " << clusters.size() << std::endl;
	std::cout << "First cluster has " << clusters[0].indices.size() << " points." << endl;
	std::cout << "These are the indices of the points of the initial" <<
		std::endl << "cloud that belong to the first cluster:" << std::endl;

	/*
	int counter = 0;
	while (counter < clusters[0].indices.size()) {
		std::cout << clusters[0].indices[counter] << ", ";
		counter++;
		if (counter % 10 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
	*/

	//���ӻ�����Ľ��
	pcl::PointCloud <pcl::PointXYZRGB>::Ptr colored_cloud = reg.getColoredCloud();
	pcl::visualization::CloudViewer viewer("Cluster viewer");
	viewer.showCloud(colored_cloud);

	while (!viewer.wasStopped())
	{
	}
	
	return (0);
}

