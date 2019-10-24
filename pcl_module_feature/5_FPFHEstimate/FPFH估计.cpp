// �������ֲ�ͼ(PFH)����
#include <pcl/point_types.h>
#include <pcl/features/fpfh.h>
#include <pcl/features/fpfh_omp.h>
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

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("room_scan1.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before fpfh estimate." << endl;

	// ���㷨����
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.03);
	ne.compute(*normals);

	pcl::FPFHEstimationOMP<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh;
	fpfh.setInputCloud (cloud);
	fpfh.setInputNormals (normals);
	// �������������ΪPointNormal,��ִ��pfh.setInputNormals (cloud);

	fpfh.setSearchMethod (tree);

	pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs (new pcl::PointCloud<pcl::FPFHSignature33> ());

	// ����pfh����ֵ,�뾶��Ϊ5cm���˰뾶Ҫ���ڱ��淨�߹�����ʹ�õĽ��������뾶��
	fpfh.setRadiusSearch (0.05);

	fpfh.compute (*fpfhs);

	// fpfhs->points.size () should have the same size as the input cloud->points.size ()*
	cout << "there are " << fpfhs->points.size() << " points after fpfh estimate." << endl;

	/*
	pcl::visualization::PCLHistogramVisualizer view; //ֱ��ͼ��ʾ
	view.setBackgroundColor(255, 0, 0);
	view.addFeatureHistogram<pcl::FPFHSignature33>(*fpfhs, "fpfh", 1000);   //���±�Ϊ1000��Ԫ�ؿ��ӻ�
	//view.addFeatureHistogram<pcl::FPFHSignature33>(*fpfhs, 33, "fpfh", 600, 200);   //���±�Ϊ1000��Ԫ�ؿ��ӻ�
	*/

	pcl::visualization::PCLPlotter plotter;
	plotter.addFeatureHistogram(*fpfhs, 300); //���õĺ����곤�ȣ���ֵԽ������ʾ��Խϸ��
	plotter.plot();

	/*
	//�����ͼ��
	pcl::visualization::PCLPlotter plotter;
	//��������
	plotter.setShowLegend(true);
	//��ʾ
	plotter.addFeatureHistogram<pcl::FPFHSignature33>(*fpfhs, "fpfh", 0, "fpfh");
	plotter.setWindowSize(800, 600);
	plotter.spinOnce(30000);
	plotter.clearPlots();
	*/

	return 0;
}