// #include <cstdio>

//#include <pcl/visualization/cloud_viewer.h>

#include "readkitti.h"

using namespace std;

pcl::PointCloud<pcl::PointXYZ>::Ptr readkitti(string file){
  // allocate 4 MB buffer (only ~130*4*4 KB are needed)
  int32_t num = 1000000;
  float *data = (float*)malloc(num*sizeof(float));

  // pointers
  float *px = data+0;
  float *py = data+1;
  float *pz = data+2;
  float *pr = data+3;

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  // load point cloud
  FILE *stream;
  stream = fopen (file.c_str(),"rb");
  num = fread(data,sizeof(float),num,stream)/4;
  cloud->height=1;
  for (int32_t i=0; i<num; i++) {
    cloud->points.push_back(pcl::PointXYZ(*px,*py,*pz));
    px+=4; py+=4; pz+=4; pr+=4;
    cloud->width++;
  }
  fclose(stream);
  return cloud;
}
