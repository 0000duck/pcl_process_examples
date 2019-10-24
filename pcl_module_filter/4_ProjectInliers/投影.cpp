#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h> //ģ��ϵ��ͷ�ļ�
#include <pcl/filters/project_inliers.h> //ͶӰ�˲���ͷ�ļ�
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���

int main()
{
	/*
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_projected(new pcl::PointCloud<pcl::PointXYZ>);  //�������Ʋ���ӡ����

	cloud->width = 5;
	cloud->height = 1;
	cloud->points.resize(cloud->width * cloud->height);  for (size_t i = 0; i < cloud->points.size(); ++i)
	{
		cloud->points[i].x = 1024 * rand() / (RAND_MAX + 1.0f);
		cloud->points[i].y = 1024 * rand() / (RAND_MAX + 1.0f);
		cloud->points[i].z = 1024 * rand() / (RAND_MAX + 1.0f);
	}

	std::cerr << "cloud before projection: " << std::endl;
	for (size_t i = 0; i < cloud->points.size(); ++i)
		std::cerr << "    " << cloud->points[i].x << " "
		<< cloud->points[i].y << " "
		<< cloud->points[i].z << std::endl;

	// ���ModelCoefficients��ֵ,ʹ��ax+by+cz+d=0ƽ��ģ�ͣ����� a=b=d=0,c=1 Ҳ����X����Yƽ��
	//����ģ��ϵ�����󣬲�����Ӧ������
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);
	coefficients->values[0] = coefficients->values[1] = 0;
	coefficients->values[2] = 1.0;
	coefficients->values[3] = 0;

	// ����ProjectInliers����ʹ��ModelCoefficients��ΪͶӰ�����ģ�Ͳ���
	pcl::ProjectInliers<pcl::PointXYZ> proj;     //����ͶӰ�˲�����
	proj.setModelType(pcl::SACMODEL_PLANE);      //���ö����Ӧ��ͶӰģ��
	proj.setInputCloud(cloud);                   //�����������
	proj.setModelCoefficients(coefficients);     //����ģ�Ͷ�Ӧ��ϵ��
	proj.filter(*cloud_projected);               //ͶӰ����洢

	std::cerr << "cloud after projection: " << std::endl;
	for (size_t i = 0; i < cloud_projected->points.size(); ++i)
		std::cerr << "    " << cloud_projected->points[i].x << " "
		<< cloud_projected->points[i].y << " "
		<< cloud_projected->points[i].z << std::endl;
	*/

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_projected(new pcl::PointCloud<pcl::PointXYZ>);  //�������Ʋ���ӡ����

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("rabbit.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	std::cout << "there are " << cloud->points.size() << " points before filtering." << std::endl;

	// ���ModelCoefficients��ֵ,ʹ��ax+by+cz+d=0ƽ��ģ�ͣ����� a=b=d=0,c=1 Ҳ����X����Yƽ��
	//����ģ��ϵ�����󣬲�����Ӧ������
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);
	coefficients->values[0] = coefficients->values[1] = 0;
	coefficients->values[2] = 1.0;
	coefficients->values[3] = 0;

	// ����ProjectInliers����ʹ��ModelCoefficients��ΪͶӰ�����ģ�Ͳ���
	pcl::ProjectInliers<pcl::PointXYZ> proj;  //����ͶӰ�˲�����
	proj.setModelType(pcl::SACMODEL_PLANE);   //���ö����Ӧ��ͶӰģ��
	proj.setInputCloud(cloud);                //�����������
	proj.setModelCoefficients(coefficients);  //����ģ�Ͷ�Ӧ��ϵ��
	proj.filter(*cloud_projected);            //ͶӰ����洢

	std::cout << "there are " << cloud_projected->points.size() << " points after filtering." << std::endl;

	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	//viewer.showCloud(cloud);
	viewer.showCloud(cloud_projected);
	
	system("pause");
	return (0);
}

