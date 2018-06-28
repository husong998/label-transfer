#ifndef GET_3D_BBOX_H
#define GET_3D_BBOX_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

typedef struct{
  char label[20];
  cv::Rect bbox;
} obj2d;

typedef struct
{
  char label[20];
  cv::Point tl1,tr1,bl1,br1,ctr;
  cv::Point tl2,tr2,bl2,br2;
  float x_min,x_max,y_min,y_max,z_min,z_max;
} obj3d;

std::vector<obj3d> get_3d_bbox(std::string input, int&);

float IOU (obj3d&, obj2d&);

#endif