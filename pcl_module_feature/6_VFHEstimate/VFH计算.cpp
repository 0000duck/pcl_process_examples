// �����ֲ�ͼ(VFH)����
#include <pcl/point_types.h>
#include <pcl/features/vfh.h>
//#include <pcl/features/vfh_omp.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/voxel_grid.h>  //�����˲����
#include <pcl/visualization/histogram_visualizer.h> //ֱ��ͼ�Ŀ��ӻ�ģ��
#include <pcl/visualization/pcl_plotter.h>
#include <iostream>

int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>());

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("room_scan1.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before vfh estimate." << endl;

	// ���㷨����
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.03);
	ne.compute(*normals);

	pcl::VFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::VFHSignature308> vfh;
	vfh.setInputCloud(cloud);
	vfh.setInputNormals(normals);
	// alternatively, if cloud is of type PointNormal, do vfh.setInputNormals (cloud);

	vfh.setSearchMethod(tree);

	pcl::PointCloud<pcl::VFHSignature308>::Ptr vfhs(new pcl::PointCloud<pcl::VFHSignature308>());

	vfh.compute(*vfhs);

	// vfhs->points.size () should be of size 1
	cout << "there are " << vfhs->points.size() << " points after vfh estimate." << endl;

	pcl::visualization::PCLPlotter plotter;
	plotter.addFeatureHistogram(*vfhs, 300); //���õĺ����곤�ȣ���ֵԽ������ʾ��Խϸ��
	plotter.setBackgroundColor(10, 10, 10);
	plotter.plot();

	return 0;
}


