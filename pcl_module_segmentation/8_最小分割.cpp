#include <pcl/io/pcd_io.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d.h>
#include <pcl/segmentation/min_cut_segmentation.h> //��С�ָ�
#include <pcl/segmentation/region_growing_rgb.h>  //��������
#include <pcl/segmentation/region_growing.h> //��������
#include <iostream>

int main(int argc, char** argv)
{
	//�������Ƶ�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud) != 0)
	{
		std::cout << "pcd file load error!\n";
		return -1;
	}

	// ����һ��Min-cut�ľ������
	pcl::MinCutSegmentation<pcl::PointXYZ> clustering;
	clustering.setInputCloud(cloud); //��������      
	// ����һ�����ƣ��г���֪�����������ڶ���ĵ㣨ǰ���㣩���������þ����������ĵ�
	pcl::PointCloud<pcl::PointXYZ>::Ptr foregroundPoints(new pcl::PointCloud<pcl::PointXYZ>());
	pcl::PointXYZ point;
	point.x = 100.0;
	point.y = 100.0;
	point.z = 100.0;
	foregroundPoints->points.push_back(point);
	clustering.setForegroundPoints(foregroundPoints);  //���þ�������ǰ����
	//����sigma����Ӱ�����ƽ���ȵĳɱ�����������ȡ���ڵ���֮��ļ�����ֱ��ʣ�
	clustering.setSigma(0.02);    // ���þ������İ뾶.
	clustering.setRadius(0.01);
	//������Ҫ�������ٽ���ĸ������������Ҳ����Ҫ���ӱ߽紦ͼ�ĸ���
	clustering.setNumberOfNeighbours(20);
	//����ǰ�����Ȩ�أ�Ҳ�����ų��ھ�������еĵ㣬���ǵ���֮���ߵ�Ȩ�أ���

	clustering.setSourceWeight(0.6);
	std::vector <pcl::PointIndices> clusters;
	clustering.extract(clusters);
	std::cout << "Maximum flow is " << clustering.getMaxFlow() << "." << std::endl;

	int currentClusterNum = 1;
	std::vector<pcl::PointIndices>::const_iterator i = clusters.begin();
	for ( ; i != clusters.end(); ++i)
	{
		//���þ������Ƶ�����
		pcl::PointCloud<pcl::PointXYZ>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZ>);      
		for (std::vector<int>::const_iterator point = i->indices.begin(); point != i->indices.end(); point++)
			cluster->points.push_back(cloud->points[*point]);
		cluster->width = cluster->points.size();
		cluster->height = 1;
		cluster->is_dense = true;
		//�������Ľ��
		if (cluster->points.size() <= 0)
			break;
		std::cout << "Cluster " << currentClusterNum << " has " << cluster->points.size() << " points." << std::endl;
		// ��������ļ�
		// std::string fileName = "cluster" + boost::to_string(currentClusterNum) + ".pcd";
		// pcl::io::savePCDFileASCII(fileName, *cluster);
		currentClusterNum++;
	}

	return 0;
}

