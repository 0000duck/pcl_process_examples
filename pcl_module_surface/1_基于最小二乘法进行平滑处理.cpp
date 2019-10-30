#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>  //kd-tree����������ඨ���ͷ�ļ�
#include <pcl/surface/mls.h> //��С���˷�ƽ�������ඨ��ͷ�ļ�
#include <pcl/visualization/cloud_viewer.h>

int main (int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ> ());
	pcl::io::loadPCDFile ("rabbit.pcd", *cloud);  // ���� KD-Tree

	//��ӡ����������
	std::cout << "Saved " << cloud->points.size() << " data points to input:" << std::endl;

	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);  // Output has the PointNormal type in order to store the normals calculated by MLS
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_mls(new pcl::PointCloud<pcl::PointNormal>);  // ������С����ʵ�ֵĶ���mls

	pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal> mls; //
	mls.setComputeNormals (true);  //��������С���˼�������Ҫ���з��߹���  
	// Set parameters  
	mls.setInputCloud (cloud);
	mls.setPolynomialFit (true);
	mls.setSearchMethod (tree);
	//mls.setSearchRadius (0.03);  
	mls.setSearchRadius (0.11);  

	// Reconstruct  
	mls.process (*cloud_mls);  // Save output
	pcl::io::savePCDFile ("mls.pcd", *cloud_mls);

	std::cout << "Saved " << cloud_mls->points.size() << " data points to output:" << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ> ());
	pcl::io::loadPCDFile ("mls.pcd", *cloud_out);  // ���� KD-Tree

	/*ͼ����ʾģ��*/
	pcl::visualization::CloudViewer viewer("Cloud Viewer"); //����viewer����
	viewer.showCloud(cloud_out);

	while (!viewer.wasStopped())
	{
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return 0;
}