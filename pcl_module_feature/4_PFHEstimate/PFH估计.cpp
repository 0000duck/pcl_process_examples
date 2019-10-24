// �������ֲ�ͼ(PFH)����
#include <pcl/point_types.h>
#include <pcl/features/pfh.h>
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
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal> ());

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("rabbit.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before pfh estimate." << endl;

	// ���㷨����
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.03);
	ne.compute(*normals);

	// ����PFH���ƶ���pfh����������������ݼ�cloud�ͷ���normals���ݸ���
	pcl::PointCloud<pcl::PFHSignature125>::Ptr pfhs (new pcl::PointCloud<pcl::PFHSignature125> ());
	pcl::PFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::PFHSignature125> pfh;
	pfh.setInputCloud (cloud);
	pfh.setInputNormals (normals);
	// �������������ΪPointNormal,��ִ��pfh.setInputNormals (cloud);

	pfh.setSearchMethod (tree);

	// ����pfh����ֵ,�뾶��Ϊ5cm���˰뾶Ҫ���ڱ��淨�߹�����ʹ�õĽ��������뾶��
	pfh.setRadiusSearch (0.05);

	pfh.compute (*pfhs);

	// pfhs->points.size()Ӧ����input cloud->points.size()����ͬ�Ĵ�С����ÿ���㶼��һ��pfh��������
	cout << "there are " << pfhs->points.size() << " points after pfh estimate." << endl;

	/*
	pcl::visualization::PCLHistogramVisualizer view; //ֱ��ͼ��ʾ
	view.setBackgroundColor(255, 0, 0);
	view.addFeatureHistogram<pcl::PFHSignature125>(*pfhs, "fpfh", 1000);   //���±�Ϊ1000��Ԫ�ؿ��ӻ�
	*/

	pcl::visualization::PCLPlotter plotter;
	plotter.addFeatureHistogram(*pfhs, 300); //���õĺ����곤�ȣ���ֵԽ������ʾ��Խϸ��
	plotter.plot();

	return 0;
}

