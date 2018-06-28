#include <opencv2/opencv.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
	while (1)
	{
		int Key=waitKeyEx();
		cout<<Key<<endl;
	}
	return 0;
}