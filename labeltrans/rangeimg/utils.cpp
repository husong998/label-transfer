#include <string>
#include <cstdio>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/projection_matrix.h>
#include <pcl/range_image/range_image.h>
#include <pcl/common/common_headers.h>
#include <pcl/visualization/range_image_visualizer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>

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

int main(){
  //loading pcd
  string file="/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/0000000000.bin";
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud=readkitti(file);

  //visualizing pcd
  pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
  viewer.showCloud (cloud);
  while (!viewer.wasStopped ())
  {
  }

  //create rangeimg
  float angres=(float)(1.0f*(M_PI/180.f));
  float maxwidth=(float) (360.0f * (M_PI/180.0f));
  float maxheight=(float) (180.0f * (M_PI/180.0f));
  Eigen::Affine3f sensorPose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f);
  pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::LASER_FRAME;
  float noiseLevel=0.00;
  float minRange = 0.0f;
  int borderSize = 1;
  pcl::RangeImage::Ptr rangeimg_ptr(new pcl::RangeImage);
  rangeimg_ptr->createFromPointCloud(*cloud,angres,maxwidth,maxheight,sensorPose,
      coordinate_frame,noiseLevel,minRange);

  //visualizing rangeimg
  range_image_widget.showRangeImage (*rangeimg_ptr);
  while(!viewer1.wasStopped()){}
  return 0;
}
