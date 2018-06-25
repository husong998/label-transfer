#ifndef READKITTI_H
#define READKITTI_H

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

#include <opencv2/opencv.hpp>

#include <string>

pcl::PointCloud<pcl::PointXYZ>::Ptr readkitti(std::string);

#endif
