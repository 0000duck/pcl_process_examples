#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <iostream>

int main(int argc, char** argv)
{
	//����pcd���ݺ󣬶����ݽ����²���
	pcl::PCLPointCloud2::Ptr cloud_blob(new pcl::PCLPointCloud2)
		, cloud_filtered_blob(new pcl::PCLPointCloud2); //�����˲�ǰ��ĵ���
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>)
		, cloud(new pcl::PointCloud<pcl::PointXYZ>)
		, cloud_p(new pcl::PointCloud<pcl::PointXYZ>)
		, cloud_f(new pcl::PointCloud<pcl::PointXYZ>);

	//��ȡpcd�ļ�  
	pcl::PCDReader reader;
	reader.read("table_scene_lms400.pcd", *cloud_blob); //ͳ���˲�ǰ�ĵ��Ƹ���
	//reader.read("table_scene_lms400_plane_1.pcd", *cloud_blob); //ͳ���˲�ǰ�ĵ��Ƹ���

	std::cerr << "PointCloud before filtering: " << cloud_blob->width * cloud_blob->height << " data points." << std::endl;
	
	// ��������դ���²���: �²����Ĵ�СΪ1cm
	pcl::VoxelGrid<pcl::PCLPointCloud2> sor; //����դ���²�������
	sor.setInputCloud(cloud_blob);           //ԭʼ����
	sor.setLeafSize(0.01f, 0.01f, 0.01f);    //���ò������ش�С
	sor.filter(*cloud_filtered_blob);        //����
	
	//ת��Ϊģ�����
	pcl::fromPCLPointCloud2(*cloud_filtered_blob, *cloud_filtered);
	pcl::fromPCLPointCloud2(*cloud_blob, *cloud);

	std::cerr << "PointCloud after filtering: " << cloud_filtered->points.size() << " data points." << std::endl;  // �����²�����ĵ���

	//pcl::PCDWriter writer;
	//writer.write<pcl::PointXYZ>("table_scene_lms400_downsampled.pcd", *cloud_filtered, false);

	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
	pcl::SACSegmentation<pcl::PointXYZ> seg;		//�����ָ����
	seg.setOptimizeCoefficients(true);				//���öԹ���ģ�Ͳ��������Ż�����
	seg.setModelType(pcl::SACMODEL_PLANE);          //���÷ָ�ģ�����
	seg.setMethodType(pcl::SAC_RANSAC);				//�������ĸ�����������Ʒ���
	seg.setMaxIterations(1000);                     //��������������
	seg.setDistanceThreshold(0.01);					//�ж��Ƿ�Ϊģ���ڵ�ľ��뷧ֵ  

	// ����ExtractIndices��ʵ�ʲ���
	pcl::ExtractIndices<pcl::PointXYZ> extract;		//����������ȡ����
	int nr_points = (int)cloud_filtered->points.size();  // While 30% of the original cloud is still there

	while (cloud_filtered->points.size() > 0.3 * nr_points)
	{
		// Ϊ�˴�����ư����Ķ��ģ�ͣ���һ��ѭ����ִ�иù��̲���ÿ��ģ�ͱ���ȡ�󣬱���ʣ��ĵ���е���
		seg.setInputCloud(cloud_filtered);
		seg.segment(*inliers, *coefficients);
		if (inliers->indices.size() == 0)
		{
			std::cerr << "Could not estimate a planar model for the given dataset." << std::endl;      break;
		}

		// Extract the inliers  
		extract.setInputCloud(cloud_filtered);
		extract.setIndices(inliers);
		extract.setNegative(false);
		extract.filter(*cloud_p);
		std::cerr << "PointCloud representing the planar component: " << cloud_p->width * cloud_p->height << " data points." << std::endl;
		std::stringstream ss;
		// ss << "table_scene_lms400_plane_" << i << ".pcd";
		// writer.write<pcl::PointXYZ>(ss.str(), *cloud_p, false);

		// Create the filtering object
		extract.setNegative(true);
		extract.filter(*cloud_f);
		cloud_filtered.swap(cloud_f);
	}

	//pcl::visualization::CloudViewer viewer("cloud"); //����viewer����
	//viewer.showCloud(cloud);
	pcl::visualization::CloudViewer viewer2("cloud_filtered"); //����viewer����
	viewer2.showCloud(cloud_filtered);

	//viewer.showCloud(cloud_f);

	system("pause");
	return (0);
}
