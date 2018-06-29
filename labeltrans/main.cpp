#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <dirent.h>

#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/visualization/cloud_viewer.h>
// #include <pcl/filters/crop_box.h>

#include "rangeimg/readkitti.h"
#include "rangeimg/GetRangeimgFromCloud.h"
#include "get_3d_bbox.h"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	//read filenames into fn
 	DIR *dir;
  	struct dirent *ent;
	dir = opendir("/home/husong/kitti/fribs_output/");
	std::vector<string> fn;
  	while ((ent=readdir(dir)) != NULL)
  	{
    	if (ent->d_name[0] != '0') continue;
    	fn.push_back(strtok(ent->d_name,"."));
  	}
  	sort (fn.begin(), fn.end());

  	//start visualizing
  	int index=0;
    namedWindow( "tests", WINDOW_AUTOSIZE );
    namedWindow( "Range Image", WINDOW_AUTOSIZE );
    Mat img;

    pcl::visualization::PCLVisualizer viewer("Cloud viewer");
    viewer.setBackgroundColor (0, 0, 0);

  	while(1)
  	{
      int key=-1;
      if (index >= fn.size()) index = 0;
  		vector<obj3d> bbox_3d = get_3d_bbox(fn[index],img);

      pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = 
      readkitti("/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/velodyne_points/data/"
        +fn[index]+".bin");

      viewer.addPointCloud<pcl::PointXYZ> (cloud, "Cloud");

      pcl::PointCloud<pcl::PointXYZ>::Ptr cropped (new pcl::PointCloud<pcl::PointXYZ>);
      for (int i=0; i<bbox_3d.size(); i++)
      {
        viewer.addCube (bbox_3d[i].x_min,bbox_3d[i].x_max,bbox_3d[i].y_min,bbox_3d[i].y_max,bbox_3d[i].z_min,bbox_3d[i].z_max,
          1,0,0,"cube"+to_string(i));

        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
        pcl::PassThrough<pcl::PointXYZ> pass;
        pass.setInputCloud (cloud);
        pass.setFilterFieldName ("x");
        pass.setFilterLimits (bbox_3d[i].x_min,bbox_3d[i].x_max);
        pass.filter (*cloud_filtered);

        pass.setInputCloud (cloud_filtered);
        pass.setFilterFieldName ("y");
        pass.setFilterLimits (bbox_3d[i].y_min,bbox_3d[i].y_max);
        pass.filter (*cloud_filtered);

        pass.setInputCloud (cloud_filtered);
        pass.setFilterFieldName ("z");
        pass.setFilterLimits (bbox_3d[i].z_min,bbox_3d[i].z_max);
        pass.filter (*cloud_filtered);

        *cropped += *cloud_filtered;

        // pcl::visualization::CloudViewer viewer1 ("Simple Cloud Viewer");
        // viewer1.showCloud (cloud_filtered);
        // while (!viewer1.wasStopped ())
        // {
        // }
      }

      Mat rangeimg=GetRangeIMG(cloud);
      Mat rangeimg_obj=GetRangeIMG(cropped);

      double range_min, range_max;
      minMaxIdx(rangeimg, &range_min, &range_max);
      convertScaleAbs(rangeimg, rangeimg, 255/range_max);
      cvtColor(rangeimg, rangeimg, COLOR_GRAY2RGB);


      for (int i=0; i<rangeimg.rows; i++)
        for (int j=0; j<rangeimg.cols; j++)
          if (rangeimg_obj.at<float>(i,j)!=0)
            rangeimg.at<Vec3b>(i,j)[0]=255;

      imshow("Range Image", rangeimg);
      convertScaleAbs(rangeimg_obj, rangeimg_obj, 255/range_max);
      imshow("Obj Range Image", rangeimg_obj);

      viewer.setRepresentationToWireframeForAllActors();
      imshow("tests",img);
      while ( -1 == (key = waitKey(1)) )
      {
        viewer.spinOnce (100);
      }

      viewer.removePointCloud ("Cloud");
      viewer.removeAllShapes ();

      if (key == 13) index++;
      else if (key == 8) index--;
      else if (key == 27) break;
      // cout<<key<<" "<<index<<endl;
  	}
	return 0;
}