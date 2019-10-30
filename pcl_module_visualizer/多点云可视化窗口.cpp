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

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->initCameraParameters();
	int v1(0); //�ӿں�
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	int v2(0); //�ӿں�
	viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	viewer->setBackgroundColor(255, 0, 255, v1);
	viewer->setBackgroundColor(0, 255, 255, v2);
	viewer->addPointCloud(cloud, "cloud", v1);
	viewer->addPointCloud(cloud_filter, "cloud_filter", v2);
	viewer->addCoordinateSystem();
	viewer->spin();

	system("pause");
	return 0;
}