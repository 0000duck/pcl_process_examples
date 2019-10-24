// �������ֲ�ͼ����
#include <pcl/point_types.h>
#include <pcl/features/pfh.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/voxel_grid.h>  //�����˲����
#include <iostream>

int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal> ());

	//... read, pass in or create a point cloud with normals ...
	//... (note: you can create a single PointCloud<PointNormal> if you want) ...
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("rabbit.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before pfh estimate." << endl;

	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.03);
	ne.compute(*normals);

	// Create the PFH estimation class, and pass the input dataset+normals to it
	// ����PFH���ƶ���pfh����������������ݼ�cloud�ͷ���normals���ݸ���
	pcl::PFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::PFHSignature125> pfh;
	pfh.setInputCloud (cloud);
	pfh.setInputNormals (normals); 

	// alternatively, if cloud is of tpe PointNormal, do pfh.setInputNormals (cloud);
	// �������������ΪPointNormal,��ִ��pfh.setInputNormals (cloud);

	// Create an empty kdtree representation, and pass it to the PFH estimation object.
	// ����һ���յ�kd����ʾ�������������ݸ�PFH���ƶ���
	// Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
	// �����Ѹ����������ݼ�������kdtree
	// pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
	// pcl::KdTreeFLANN<pcl::PointXYZ>::Ptr tree (new pcl::KdTreeFLANN<pcl::PointXYZ> ()); -- older call for PCL 1.5-
	pfh.setSearchMethod (tree);

	pcl::PointCloud<pcl::PFHSignature125>::Ptr pfhs (new pcl::PointCloud<pcl::PFHSignature125> ());

	// Use all neighbors in a sphere of radius 5cm
	// IMPORTANT: the radius used here has to be larger than the radius used to estimate the surface normals!!!
	// ����pfh����ֵ,�뾶��Ϊ5cm���˰뾶Ҫ���ڱ��淨�߹�����ʹ�õĽ��������뾶��
	pfh.setRadiusSearch (0.05);

	// Compute the features
	pfh.compute (*pfhs);

	// pfhs->points.size()Ӧ����input cloud->points.size()����ͬ�Ĵ�С����ÿ���㶼��һ��pfh��������
	cout << "there are " << pfhs->points.size() << " points after pfh estimate." << endl;
}

