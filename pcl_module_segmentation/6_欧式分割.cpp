#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h> //
#include <pcl/segmentation/extract_clusters.h> //������ȡ
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <iostream>

/* �򿪵������ݣ����Ե��ƽ����˲��ز���Ԥ����Ȼ�����ƽ��ָ�ģ�ͶԵ��ƽ��зָ��
��ȡ��������������ƽ���ϵĵ㼯�����������*/

int main(int argc, char** argv)
{
	// ��ȡ�ļ�
	pcl::PCDReader reader;
	pcl::PointCloud<pcl::PointXYZ>::Ptr add_cloud(new pcl::PointCloud<pcl::PointXYZ>); pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>), cloud_f(new pcl::PointCloud<pcl::PointXYZ>);
	reader.read("table_scene_lms400.pcd", *cloud);
	std::cout << "PointCloud before filtering has: " << cloud->points.size() << " data points." << std::endl;  // �²���������Ҷ�Ӵ�СΪ0.01

	pcl::VoxelGrid<pcl::PointXYZ> vg;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	vg.setInputCloud(cloud);
	vg.setLeafSize(0.01f, 0.01f, 0.01f);
	vg.filter(*cloud_filtered);
	std::cout << "PointCloud after filtering has: " << cloud_filtered->points.size() << " data points." << std::endl;

	//����ƽ��ģ�ͷָ�Ķ������ò���
	pcl::SACSegmentation<pcl::PointXYZ> seg;
	pcl::PointIndices::Ptr inliers(new pcl::PointIndices); //���þ�����ڵ�����
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
	//ƽ��ģ�͵�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ>());

	pcl::PCDWriter writer;
	seg.setOptimizeCoefficients(true);
	seg.setModelType(pcl::SACMODEL_PLANE);    //�ָ�ģ��
	seg.setMethodType(pcl::SAC_RANSAC);       //����������Ʒ���
	seg.setMaxIterations(100);                //���ĵ����Ĵ���
	seg.setDistanceThreshold(0.02);           //���÷�ֵ
	int i=0, nr_points = (int) cloud_filtered->points.size ();//ʣ����Ƶ�����

	while (cloud_filtered->points.size() > 0.3 * nr_points)
	{
		// ��ʣ��������ٷָ������ƽ����� ����Ϊ����Ҫ����ĵ��Ƶ�����������ƽ��Ĵ��ڵģ�
		seg.setInputCloud (cloud_filtered);
		seg.segment(*inliers, *coefficients);
		if (inliers->indices.size() == 0) //����ڵ�������Ѿ�����0����˵��û��   
		{
			std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
			break;
		}
		// ������ĵ�������ȡƽ��ģ�͵��ڵ�
		pcl::ExtractIndices<pcl::PointXYZ> extract;
		extract.setInputCloud(cloud_filtered);
		extract.setIndices(inliers);        //��ȡ�ڵ���������洢������
		extract.setNegative(false);    // �õ���ƽ�����������ĵ�������
		extract.filter(*cloud_plane);
		std::cout << "PointCloud representing the planar component: " << cloud_plane->points.size() << " data points." << std::endl;

		// ��ȥƽ����ڵ㣬��ȡʣ�����   
		extract.setNegative(true);
		extract.filter(*cloud_f);
		*cloud_filtered = *cloud_f;
	}

	// ����������ȡ����������kdtree������
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud(cloud_filtered);
	std::vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;   //ŷʽ�������
	ec.setClusterTolerance(0.02);              // ���ý��������������뾶Ϊ2cm
	ec.setMinClusterSize(100);                 //����һ��������Ҫ�����ٵĵ���ĿΪ100
	ec.setMaxClusterSize(25000);               //����һ��������Ҫ��������ĿΪ25000
	ec.setSearchMethod(tree);                    //���õ��Ƶ���������
	ec.setInputCloud (cloud_filtered);
	ec.extract(cluster_indices);

	//�ӵ�������ȡ���࣬������������������cluster_indices��  
	//�������ʵ�������cluster_indices,ֱ���ָ���о���
	int j = 0;
	std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin();
	for (; it != cluster_indices.end(); ++it)
	{
		//���������еĵ��Ƶ����������ҷֿ����������ĵ���
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
		for (std::vector<int>::const_iterator pit = it->indices.begin();pit != it->indices.end(); ++pit)
			//���ñ�����Ƶ���������
			cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
		cloud_cluster->width = cloud_cluster->points.size();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;
		std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size() << " data points." << std::endl;
		std::stringstream ss;
		ss << "cloud_cluster_" << j << ".pcd";
		writer.write<pcl::PointXYZ>(ss.str(), *cloud_cluster, false);
		//���Ͼ���ʵ�����еľ���Ĳ��裬���ұ����˵���
		j++;
		*add_cloud += *cloud_cluster;
		pcl::io::savePCDFileASCII("add_cloud.pcd", *add_cloud);
	}

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->initCameraParameters();
	int v1(0); //�ӿں�
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	int v2(0); //�ӿں�
	viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	viewer->setBackgroundColor(0, 0, 0, v1);
	viewer->setBackgroundColor(0, 0, 0, v2);
	viewer->addPointCloud(cloud, "cloud", v1);
	viewer->addPointCloud(add_cloud, "add_cloud", v2);
	viewer->addCoordinateSystem();
	viewer->spin();

	return (0);
}

