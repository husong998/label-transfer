#include "readkitti.h"
#include "GetRangeimgFromCloud.h"

using namespace std;
using namespace cv;

int main()
{
  //Change the content of this string below to the path of KITTI Velodyne data on your disk
  string file="/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/0000000000.bin";

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = readkitti(file);
  
  Mat rangeimg = GetRangeIMG(cloud);
  
  imwrite("test.png",rangeimg);
  
  return 0;
}
