#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>  //��������ض���
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <pcl/filters/median_filter.h>  //ֱͨ�˲����
#include <pcl/common/common.h>

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	//1.��ȡ����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size()<<" points before filtering." << endl;

	//2.ȡ�õ������꼫ֵ
	pcl::PointXYZ minPt, maxPt;
	pcl::getMinMax3D(*cloud, minPt, maxPt);

	//3.ֱͨ�˲�
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::MedianFilter<pcl::PointXYZ> median;
	median.setInputCloud(cloud);		//���ô��˲��ĵ���
	median.setMaxAllowedMovement(15.5);
	median.setWindowSize(3); 
	median.filter(*cloud_filtered);

	//4.�˲��������
	pcl::io::savePCDFile<pcl::PointXYZ>("filter.pcd", *cloud_filtered);
	cout << "there are " << cloud_filtered->points.size() << " points after filtering." << endl;

	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	// viewer.showCloud(cloud);
	viewer.showCloud(cloud_filtered);

	system("pause");
	return 0;
}