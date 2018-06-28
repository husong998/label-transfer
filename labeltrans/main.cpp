#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <dirent.h>

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
  	int index=0, key=0;
    namedWindow( "tests", WINDOW_AUTOSIZE );
  	while(1)
  	{
      if (index >= fn.size()) index = 0;
  		vector<obj3d> bbox_3d = get_3d_bbox(fn[index],key);
  		if (key == 13) index++;
      else if (key == 8) index--;
      else if (key == 27) break;
      cout<<key<<" "<<index<<endl;
  	}
	return 0;
}