#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h> //ͳ����Ⱥɾ��
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <iostream>

int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

	//�����ȡ����
	pcl::PCDReader reader;  //��ȡ�����ļ�
	reader.read<pcl::PointXYZ> ("table_scene_lms400.pcd", *cloud);
	std::cerr << "cloud before filtering: " << cloud->points.size() << std::endl;
	// std::cerr << *cloud << std::endl;

	// �����˲�������ÿ����������ٽ���ĸ�������Ϊ50 ��������׼��ı�������Ϊ1  ����ζ�����һ����ľ��볬����ƽ������һ����׼�����ϣ���õ㱻���Ϊ��Ⱥ�㣬�������Ƴ����洢����
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;  //�����˲�������
	sor.setInputCloud(cloud);                           //���ô��˲��ĵ���
	sor.setMeanK(50);									//�����ڽ���ͳ��ʱ���ǲ�ѯ���ٽ�����
	sor.setStddevMulThresh(1.0);						//�����ж��Ƿ�Ϊ��Ⱥ��ķ�ֵ
	sor.filter(*cloud_filtered);						//�洢
	std::cerr << "cloud after filtering: " << cloud_filtered->points.size() << std::endl;
	// std::cerr << *cloud_filtered << std::endl;
	
	pcl::PCDWriter writer;
	writer.write<pcl::PointXYZ>("table_scene_lms400_inliers.pcd", *cloud_filtered, false);
	sor.setNegative(true);
	sor.filter(*cloud_filtered);

	writer.write<pcl::PointXYZ>("table_scene_lms400_outliers.pcd", *cloud_filtered, false);
	
	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	//viewer.showCloud(cloud);
	viewer.showCloud(cloud_filtered);

	system("pause");
	return (0);
}


