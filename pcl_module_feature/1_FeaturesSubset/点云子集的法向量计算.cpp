// ������Ƶ�ĳ�Ӽ��ķ�����
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/extract_indices.h>
#include <iostream>

int main()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before estimate." << endl;

	// ����������
	std::vector<int> indices(std::floor(cloud->points.size() / 10));
	for (std::size_t i = 0; i < indices.size(); ++i)
		indices[i] = i;

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);

	boost::shared_ptr<std::vector<int> > indicesptr(new std::vector<int>(indices));
	ne.setIndices(indicesptr);

	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	ne.setSearchMethod(tree);

	pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);

	ne.setRadiusSearch(0.03);

	ne.compute(*cloud_normals);

	cout << "there are " << cloud_normals->points.size() << " points after estimate." << endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_extract(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::ExtractIndices<pcl::PointXYZ> extract;
	extract.setInputCloud(cloud);
	extract.setIndices(indicesptr);
	extract.setNegative(false);//�����Ϊtrue,������ȡָ��index֮��ĵ���
	extract.filter(*cloud_extract);

	/*ͼ����ʾģ��*/
	//��ʾ����
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));

	//���ñ���ɫ
	viewer->setBackgroundColor(0, 0, 0.7);

	//���õ�����ɫ���ô�Ϊ��һ��ɫ����
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0);

	//�����Ҫ��ʾ�ĵ�������
	viewer->addPointCloud<pcl::PointXYZ>(cloud_extract, single_color, "sample cloud");

	//���õ���ʾ��С
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");

	//�����Ҫ��ʾ�ĵ��Ʒ���cloudΪԭʼ����ģ�ͣ�normalΪ������Ϣ��10��ʾ��Ҫ��ʾ����ĵ��Ƽ������ÿ10������ʾһ�η��򣬣���ʾ���򳤶ȡ�
	//viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud_extract, cloud_normals, 10, 5, "normals");
	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud_extract, cloud_normals);

	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return 0;
}



