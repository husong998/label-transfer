#include <dirent.h>

#include "readkitti.h"
#include "GetRangeimgFromCloud.h"

using namespace std;
using namespace cv;

int main()
{
  DIR *dir;
  struct  dirent *ent;
  //Change the content of this string below to the path of KITTI Velodyne data on your disk
  dir = opendir("/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/");
  
  while ((ent = readdir(dir)) != NULL)
  {
    if (ent->d_name[0] != '0') continue;
  	string file = "/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/";
  	file += ent->d_name;
  	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = readkitti(file);
  
  	Mat rangeimg = GetRangeIMG(cloud);
  	file = "/home/husong/kitti/rangeimg/";
  	imwrite(file+strtok(ent->d_name,".")+".png",rangeimg);
  }

  
  return 0;
}
