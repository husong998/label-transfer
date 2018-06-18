#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "projection.h"

using namespace std;
using namespace cv;

typedef struct{
  char label[20];
  Point tl,tr,bl,br,ctr;
} obj2d;

typedef struct{
  char label[20];
  Point tl1,tr1,bl1,br1,ctr;
  Point tl2,tr2,bl2,br2;
} obj3d;

int main()
{
  String name("/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/image_00/data/0000000000.png");
  Mat img;
  img=imread(name,IMREAD_COLOR);

  FILE* fi=fopen("../yoloresult/result","r");
  int i=0,left,right,top,bot;
  vector<obj2d> yolo;
  obj2d s;
  while(fgets(s.label,20,fi)){
    fscanf(fi,"%d%d%d%d\n",&left,&right,&top,&bot);
    s.tl=Point(left,top);
    s.tr=Point(right,top);
    s.bl=Point(left,bot);
    s.br=Point(right,bot);
    s.ctr=Point((left+right+1)/2,(top+bot+1)/2);
    line(img, s.tl, s.tr, Scalar(0,0,100), 2, FILLED);
    line(img, s.tl, s.bl, Scalar(0,0,100), 2, FILLED);
    line(img, s.tr, s.br, Scalar(0,0,100), 2, FILLED);
    line(img, s.bl, s.br, Scalar(0,0,100), 2, FILLED);
    yolo.push_back(s);
  } 
  fclose(fi);

  obj3d sf;
  fi=fopen("../outfile","r");
  long double ctr[3],dim[3],corners[3];
  while(3==fscanf(fi,"%Lf%Lf%Lf",&ctr[0],&ctr[1],&ctr[2])){
    fscanf(fi,"%Lf%Lf%Lf",&dim[0],&dim[1],&dim[2]);
    if(ctr[0]<5) continue;
    long double *pctr=proj(ctr);
    sf.ctr=Point(0.5+pctr[0],0.5+pctr[1]);
    circle(img, sf.ctr, 2, Scalar(0,255,0), FILLED, FILLED);

    corners[0]=ctr[0]-dim[0]/2;corners[1]=ctr[1]-dim[1]/2;corners[2]=ctr[2]-dim[2]/2;
    pctr=proj(corners);
    sf.br1=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]-dim[0]/2;corners[1]=ctr[1]-dim[1]/2;corners[2]=ctr[2]+dim[2]/2;
    pctr=proj(corners);
    sf.tr1=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]-dim[0]/2;corners[1]=ctr[1]+dim[1]/2;corners[2]=ctr[2]+dim[2]/2;
    pctr=proj(corners);
    sf.tl1=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]-dim[0]/2;corners[1]=ctr[1]+dim[1]/2;corners[2]=ctr[2]-dim[2]/2;
    pctr=proj(corners);
    sf.bl1=Point(pctr[0]+0.5,pctr[1]+0.5);

    corners[0]=ctr[0]+dim[0]/2;corners[1]=ctr[1]-dim[1]/2;corners[2]=ctr[2]-dim[2]/2;
    pctr=proj(corners);
    sf.br2=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]+dim[0]/2;corners[1]=ctr[1]-dim[1]/2;corners[2]=ctr[2]+dim[2]/2;
    pctr=proj(corners);
    sf.tr2=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]+dim[0]/2;corners[1]=ctr[1]+dim[1]/2;corners[2]=ctr[2]+dim[2]/2;
    pctr=proj(corners);
    sf.tl2=Point(pctr[0]+0.5,pctr[1]+0.5);
    corners[0]=ctr[0]+dim[0]/2;corners[1]=ctr[1]+dim[1]/2;corners[2]=ctr[2]-dim[2]/2;
    pctr=proj(corners);
    sf.bl2=Point(pctr[0]+0.5,pctr[1]+0.5);

    line(img, sf.tl1, sf.tr1, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tl1, sf.bl1, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tr1, sf.br1, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.bl1, sf.br1, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tl2, sf.tr2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tl2, sf.bl2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tr2, sf.br2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.bl2, sf.br2, Scalar(0,100,0), 1, LINE_8);

    line(img, sf.tl1, sf.tl2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.tr1, sf.tr2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.br1, sf.br2, Scalar(0,100,0), 1, LINE_8);
    line(img, sf.bl1, sf.bl2, Scalar(0,100,0), 1, LINE_8);
  }
  imshow("000",img);
  namedWindow( "000", WINDOW_AUTOSIZE );
  waitKey(0);
  return 0;
}