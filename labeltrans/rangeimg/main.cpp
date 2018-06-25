#include "readkitti.h"
#include "GetRangeimgFromCloud.h"

using namespace std;
using namespace cv;

int main()
{
  string file="/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/0000000000.bin";
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = readkitti(file);
  //for(int i=0; i<cloud->points.size(); i++)
    //printf("%.2f %.2f %.2f\n",cloud->points[i].x,cloud->points[i].y,cloud->points[i].z);
  Mat rangeimg = GetRangeIMG(cloud);
  imwrite("test.png",rangeimg);
  return 0;
}