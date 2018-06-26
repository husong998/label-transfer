# Range image generation using KITTI Velodyne data
The code in this folder reads Velodyne data in KITTI dataset and generate a range image.
## Dependencies
- Point Cloud Library
- OpenCV
## Usage
- Clone the repository
`$ git clone git@github.com:husong998/label-transfer.git`
- `$ cd label-transfer/labeltrans/rangeimg`
- Open main.cpp in your favorite editor and change the content of string file to the path of the KITTI Velodyne data file.
- `$ cd build`
- `$ cmake ..`
- `$ make -j`
- `$ ./rangeimg`
- The sample range image is named "test.png" under build/ folder.
## File description
### readkitti.cpp
This file contains the function that can read the Velodyne binary data provided by the KITTI dataset into a PCL PointCloud.
### GetRangeimgFromCloud.cpp
The function GetRangeIMG takes a PCL PointCloud and spits out a range image as an OpenCV matrix. See the source code comment for more detail regarding the algorithm.
### main.cpp
This is file that you can play around to make it suite your needs. It should be easy to understand and ammend.
## Credits
Special thanks to Igor Bogoslavskyi for his expertise in range image processing algorithm. All I did was simply extracted the particular piece of code from his depth_clustering project (https://github.com/PRBonn/depth_clustering).
