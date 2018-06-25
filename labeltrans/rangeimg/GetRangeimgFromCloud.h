#ifndef RANGEIMG_H
#define RANGEIMG_H

#include <vector>

int find_nearest(std::vector<float>&, float);

cv::Mat GetRangeIMG(pcl::PointCloud<pcl::PointXYZ>::Ptr);

constexpr long double operator"" _deg ( long double deg )
{
      return deg*3.141592/180;
}

#endif
