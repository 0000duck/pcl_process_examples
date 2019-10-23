#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>  //��������ض���
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <pcl/filters/passthrough.h>  //ֱͨ�˲����
#include <pcl/common/common.h>

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	/****************************************************************************************
	*****************************************************************************************
	** (һ) ��ȡ��������
	*****************************************************************************************
	****************************************************************************************/

	//1.��ȡ����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("rabbit.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size()<<" points before filtering." << endl;

	//2.ȡ�õ������꼫ֵ
	pcl::PointXYZ minPt, maxPt;
	pcl::getMinMax3D(*cloud, minPt, maxPt);

	//3.ֱͨ�˲�
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PassThrough<pcl::PointXYZ> pass;		//�����˲�������
	pass.setInputCloud(cloud);					//���ô��˲��ĵ���
	pass.setFilterFieldName("z");			    //������Z�᷽���Ͻ����˲�
	//pass.setFilterLimits(0, maxPt.z - 1);		//�����˲���Χ
	pass.setFilterLimits(minPt.z, maxPt.z - 5); //�����˲���Χ
	pass.setFilterLimitsNegative(false);		//����
	pass.filter(*cloud_filter);					//�˲����洢

	//4.�˲��������
	pcl::io::savePCDFile<pcl::PointXYZ>("filter.pcd", *cloud_filter);
	cout << "there are " << cloud_filter->points.size() << " points after filtering." << endl;

	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	// viewer.showCloud(cloud);
	viewer.showCloud(cloud_filter);


	/****************************************************************************************
	*****************************************************************************************
	** (��) �Զ����������
	*****************************************************************************************
	****************************************************************************************/
	/*
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

	cloud->width  = 5;
	cloud->height = 1;
	cloud->points.resize (cloud->width * cloud->height);
	for (size_t i = 0; i < cloud->points.size (); ++i)   //�������
	{
		cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
		cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
		cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
	}
	std::cerr << "cloud before filtering: " << std::endl;   //��ӡ
	
	for (size_t i = 0; i < cloud->points.size (); ++i)
		std::cerr << "    " << cloud->points[i].x << " "
							<< cloud->points[i].y << " "
							<< cloud->points[i].z << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
	// �����˲�������
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud (cloud);          //�����������
	pass.setFilterFieldName ("z");       //���ù���ʱ����Ҫ�������͵�Z�ֶ�
	pass.setFilterLimits (0.0, 500.0);   //�����ڹ����ֶεķ�Χ
	pass.setFilterLimitsNegative (true); //���ñ�����Χ�ڻ��ǹ��˵���Χ�� false: ���� true������
	pass.filter (*cloud_filtered);       //ִ���˲�
	std::cerr << "cloud after filtering: " << std::endl;   //��ӡ
	for (size_t i = 0; i < cloud_filtered->points.size (); ++i)
	std::cerr << "    " << cloud_filtered->points[i].x << " "
					   << cloud_filtered->points[i].y << " "
					   << cloud_filtered->points[i].z << std::endl;
	*/

	system("pause");
	return 0;
}