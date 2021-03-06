#include <opencv2/opencv.hpp>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

#include <iostream>
#include <cmath>

#include "GetRangeimgFromCloud.h"

using namespace std;

//Find the nearest row number and column number of a 3D Velodyne point
int find_nearest(vector<float>& angles, float angle)
{
  int found=0;
  if (angles.front() < angles.back())
    found=upper_bound(angles.begin(),angles.end(),angle)
      -angles.begin();
  else
    found=angles.rend()-upper_bound(angles.rbegin(),angles.rend(),angle);
  if (found==0)
    return found;
  if (found==angles.size())
    return found-1;
  float diff_next = fabs(angles[found]-angle),
        diff_prev = fabs(angle-angles[found-1]);
  return diff_next < diff_prev ? found : found-1;
}

cv::Mat GetRangeIMG(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
  //image dimension is 64x870
  cv::Mat rangeimg = cv::Mat::zeros(64,870, CV_32F);

  //initialize angle values for rows
  int i;
  vector<float> row_angles;
  float step=-0.33333_deg, start=2.0_deg;
  for (i = 0; i < 32; i++)
    row_angles.push_back(start+i*step);

  step=-0.5_deg, start=-8.83_deg;
  for (i = 0; i < 32; i++)
    row_angles.push_back(start+i*step);

  //initialize angle values for columns
  vector<float> col_angles;
  step=0.413793103_deg, start=-180.0_deg;
  for (i = 0; i < 870; i++)
    col_angles.push_back(start+i*step);

  //search thru all the 3d points
  for(i=0;i<cloud->points.size();i++)
  {
    float dist=cloud->points[i].x*cloud->points[i].x+
      cloud->points[i].y*cloud->points[i].y+
      cloud->points[i].z*cloud->points[i].z;
    dist=sqrt(dist);
    if (dist < 0.01) continue;
    //get the position of the pixel
    float row_angle = asin(cloud->points[i].z/dist),
          col_angle = atan2(cloud->points[i].y,cloud->points[i].x);
    int row=find_nearest(row_angles,row_angle),
        col=find_nearest(col_angles,col_angle);
    float& current_depth=rangeimg.at<float>(row,col);
    if (dist > current_depth)
      current_depth=dist;
  }
  return rangeimg;
}
