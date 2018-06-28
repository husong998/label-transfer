#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "projection.h"
#include "get_3d_bbox.h"

using namespace std;
using namespace cv;

float IOU (obj3d& fribs_bbox, obj2d& yolo_bbox)
{
  int top, bot, left, right;
  top = min( min( min( fribs_bbox.tl1.y, fribs_bbox.tr1.y) ,fribs_bbox.tl2.y) ,fribs_bbox.tr2.y);
  bot = max( max( max( fribs_bbox.bl1.y, fribs_bbox.br1.y) ,fribs_bbox.bl2.y) ,fribs_bbox.br2.y);
  left = min( min( min( fribs_bbox.tl1.x, fribs_bbox.bl1.x) ,fribs_bbox.tl2.x) ,fribs_bbox.bl2.x);
  right = max( max( max( fribs_bbox.br1.x, fribs_bbox.tr1.x) ,fribs_bbox.br2.x) ,fribs_bbox.tr2.x);
  Rect fribsRect = Rect(Point(left,top), Point(right,bot));

  Rect Intersection;
  Intersection = fribsRect & yolo_bbox.bbox;

  return 1.0*Intersection.area()/
  (fribsRect.area()+yolo_bbox.bbox.area()-Intersection.area());
}

std::vector<obj3d> get_3d_bbox(string input, Mat& img)
{
  const float thresh = 0.5;

  String name("/home/husong/kitti/2011_09_26/2011_09_26_drive_0001_sync/image_00/data/"
  +input+".png");
  img=imread(name,IMREAD_COLOR);

  string path="../../yoloresult/"+input;
  FILE* fi=fopen(path.c_str(),"r");
  int i=0,left,right,top,bot;
  vector<obj2d> yolo;
  vector<obj3d> fribs;

  obj2d s;
  Point tl,br;
  while(fgets(s.label,20,fi)){
    fscanf(fi,"%d%d%d%d\n",&left,&right,&top,&bot);
    if(s.label[strlen(s.label)-1]=='\n')
      s.label[strlen(s.label)-1]=0;
    tl=Point(left,top);
    br=Point(right,bot);
    s.bbox=Rect(tl,br);
    rectangle(img,s.bbox,Scalar(0,0,255));
    yolo.push_back(s);
  } 
  fclose(fi);

  obj3d sf;
  path="../../fribs_output/"+input;
  fi=fopen(path.c_str(),"r");
  long double ctr[3],dim[3],corners[3];
  while(3==fscanf(fi,"%Lf%Lf%Lf",&ctr[0],&ctr[1],&ctr[2])){
    fscanf(fi,"%Lf%Lf%Lf",&dim[0],&dim[1],&dim[2]);
    if(ctr[0]<5) continue;
    long double *pctr=proj(ctr);
    sf.ctr=Point(0.5+pctr[0],0.5+pctr[1]);
    circle(img, sf.ctr, 2, Scalar(0,255,0), FILLED, FILLED);

    sf.x_min=ctr[0]-dim[0]/2;sf.x_max=ctr[0]+dim[0]/2;
    sf.y_min=ctr[1]-dim[1]/2;sf.y_max=ctr[1]+dim[1]/2;
    sf.z_min=ctr[2]-dim[2]/2;sf.z_max=ctr[2]+dim[2]/2;

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

    fribs.push_back(sf);

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

  vector<obj3d> valid_transfer;
  for(vector<obj2d>::iterator ityolo=yolo.begin();ityolo!=yolo.end();ityolo++){
    for(vector<obj3d>::iterator itfribs=fribs.begin();
        itfribs!=fribs.end();itfribs++){
      if(itfribs->ctr.inside(ityolo->bbox)){
        if ( IOU(*itfribs, *ityolo) < thresh ) continue;
        putText(img,ityolo->label,itfribs->ctr,FONT_HERSHEY_PLAIN,1,
            Scalar(0,255,0));
        strcpy(itfribs->label,ityolo->label);
        valid_transfer.push_back(*itfribs);
      }
    }
  }
  return valid_transfer;
}
