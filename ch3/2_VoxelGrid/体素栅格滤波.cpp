#include <pcl/io/pcd_io.h>  //�ļ��������
#include <pcl/point_types.h>  //��������ض���
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <pcl/filters/voxel_grid.h>  //�����˲����
#include <pcl/common/common.h>  

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	//1.��ȡ����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("object_template_0.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before filtering." << endl;

	//2����դ���˲�
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud(cloud);
	//sor.setLeafSize(0.3f, 0.3f, 0.3f);//���ش�С����Ϊ30*30*30cm
	sor.setLeafSize(0.01f, 0.01f, 0.01f);//���ش�С����Ϊ1*1*1cm
	sor.filter(*cloud_filter);
	
	//3.�˲��������
	pcl::io::savePCDFile<pcl::PointXYZ>("demo_filter.pcd", *cloud_filter);
	cout << "there are " << cloud_filter->points.size() << " points after filtering." << endl;

	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	viewer.showCloud(cloud);

	system("pause");
	return 0;
}