#include <iostream>
#include <pcl/point_types.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>

int main (int argc, char** argv)
{
	if (argc != 2)  //ȷ������Ĳ��� 
	{
		std::cerr << "please specify command line arg '-r' or '-c'" << std::endl;
		exit(0);
	}

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);  //������

	cloud->width  = 5;
	cloud->height = 1;
	cloud->points.resize (cloud->width * cloud->height);  for (size_t i = 0; i < cloud->points.size (); ++i)
	{
		cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
		cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
		cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
	}
	if (strcmp(argv[1], "-r") == 0) {
		pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;  //�����˲���    
		outrem.setInputCloud(cloud);    //�����������
		outrem.setRadiusSearch(0.8);     //���ð뾶Ϊ0.8�ķ�Χ�����ٽ���
		outrem.setMinNeighborsInRadius (2); //���ò�ѯ�������㼯��С��2��ɾ��    // apply filter
		outrem.filter (*cloud_filtered);   //ִ�������˲�   �ڰ뾶Ϊ0.8 �ڴ˰뾶�ڱ���Ҫ�������ھӵ㣬�˵�Żᱣ��
	} else if (strcmp(argv[1], "-c") == 0) {    
		//���������޶����µ��˲���
		pcl::ConditionAnd<pcl::PointXYZ>::Ptr range_cond (new pcl::ConditionAnd<pcl::PointXYZ> ());   //���������������     

		//Ϊ�������������ӱȽ�����
		//�����Z�ֶ��ϴ���0�ıȽ�����
		range_cond->addComparison (pcl::FieldComparison<pcl::PointXYZ>::ConstPtr (new pcl::FieldComparison<pcl::PointXYZ> ("z", pcl::ComparisonOps::GT, 0.0)));
		//�����Z�ֶ���С��0.8�ıȽ�����
		range_cond->addComparison (pcl::FieldComparison<pcl::PointXYZ>::ConstPtr (new pcl::FieldComparison<pcl::PointXYZ> ("z", pcl::ComparisonOps::LT, 0.8)));
	
		// �����˲�������������������ʼ��
		pcl::ConditionalRemoval<pcl::PointXYZ> condrem;
		condrem.setCondition (range_cond);              
		condrem.setInputCloud (cloud);     //�������
		condrem.setKeepOrganized(true);    //���ñ��ֵ��ƵĽṹ
		// ִ���˲�
		condrem.filter (*cloud_filtered);  //����0.0С��0.8�������������ڽ����˲���
	}else{
		std::cerr << "please specify command line arg '-r' or '-c'" << std::endl;
		exit(0);
	}

	std::cerr << "Cloud before filtering: " << std::endl;  for (size_t i = 0; i < cloud->points.size (); ++i)
	std::cerr << "    " << cloud->points[i].x << " "
					   << cloud->points[i].y << " "
					   << cloud->points[i].z << std::endl;  

	std::cerr << "Cloud after filtering: " << std::endl;  for (size_t i = 0; i < cloud_filtered->points.size (); ++i)
	std::cerr << "    " << cloud_filtered->points[i].x << " "
					   << cloud_filtered->points[i].y << " "
					   << cloud_filtered->points[i].z << std::endl;
	
	return (0);
}


