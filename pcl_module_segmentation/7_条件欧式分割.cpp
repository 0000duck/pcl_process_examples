#include <pcl/io/pcd_io.h>
#include <pcl/segmentation/conditional_euclidean_clustering.h>
#include <pcl/visualization/cloud_viewer.h>  //���ƿ��ӻ���ض���
#include <iostream>

//����˺�������true������Ӻ�ѡ�㵽���ӵ�Ĵ����С�
bool customCondition(const pcl::PointXYZ& seedPoint, const pcl::PointXYZ& candidatePoint, float squaredDistance)
{    // �����������������Զ��������
	if (candidatePoint.y < seedPoint.y)
		return false;
	return true;
}

int main(int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) != 0)
	{
		std::cout << "pcd file load error!\n";
		return -1;
	}

	// ����һ����������Ķ���
	pcl::ConditionalEuclideanClustering<pcl::PointXYZ> clustering;
	clustering.setClusterTolerance(0.02);
	clustering.setMinClusterSize(100);
	clustering.setMaxClusterSize(25000);
	clustering.setInputCloud(cloud);    // ����Ҫ���ÿ�Ե�ĺ�����
	clustering.setConditionFunction(&customCondition);
	std::vector<pcl::PointIndices> clusters;
	clustering.segment(clusters);

	// ����ÿһ��������
	int currentClusterNum = 1;
	std::vector<pcl::PointIndices>::const_iterator i = clusters.begin();
	for ( ; i != clusters.end(); ++i)
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZ>);
		std::vector<int>::const_iterator point = i->indices.begin();
		for ( ; point != i->indices.end(); point++)
			cluster->points.push_back(cloud->points[*point]);

		cluster->width = cluster->points.size();
		cluster->height = 1;
		cluster->is_dense = true;

		if (cluster->points.size() <= 0)
			break;

		std::cout << "Cluster " << currentClusterNum << " has " << cluster->points.size() << " points." << std::endl;
		//std::string fileName = "cluster" + boost::to_string(currentClusterNum) + ".pcd";
		//pcl::io::savePCDFileASCII(fileName, *cluster);
		currentClusterNum++;
	}

}