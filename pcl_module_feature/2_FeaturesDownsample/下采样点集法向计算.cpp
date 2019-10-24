// �����²������Ƶķ�����
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/voxel_grid.h>  //�����˲����
#include <iostream>


int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_downsampled (new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before estimate." << endl;

	// �������������˲�������
	pcl::VoxelGrid<pcl::PointXYZ> vgf;
	vgf.setInputCloud(cloud);
	vgf.setLeafSize(0.01f, 0.01f, 0.01f);// ��λ��m
	//sor.setLeafSize(0.3f, 0.3f, 0.3f);// ��λ��m
	vgf.filter(*cloud_downsampled);

	// �����²������Ƶķ�����
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud (cloud_downsampled);

	// ԭ����������Ϊ����������
	ne.setSearchSurface (cloud);

	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
	ne.setSearchMethod (tree);

	pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);

	ne.setRadiusSearch (0.03);

	ne.compute (*cloud_normals);

	// cloud_normals->points.size () should have the same size as the input cloud_downsampled->points.size ()
	cout << "there are " << cloud_normals->points.size() << " points after estimate." << endl;

	/*ͼ����ʾģ��*/
	//��ʾ����
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));

	//���ñ���ɫ
	viewer->setBackgroundColor(0, 0, 0.7);

	//���õ�����ɫ���ô�Ϊ��һ��ɫ����
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0);

	//�����Ҫ��ʾ�ĵ�������
	viewer->addPointCloud<pcl::PointXYZ>(cloud_downsampled, single_color, "sample cloud");

	//���õ���ʾ��С
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");

	//�����Ҫ��ʾ�ĵ��Ʒ���cloudΪԭʼ����ģ�ͣ�normalΪ������Ϣ��10��ʾ��Ҫ��ʾ����ĵ��Ƽ������ÿ10������ʾһ�η��򣬣���ʾ���򳤶ȡ�
	//viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud, cloud_normals, 10, 5, "normals");
	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud_downsampled, cloud_normals);

	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return 0;
}





