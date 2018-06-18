#include "projection.h"

long double* proj(long double *points){
  const long double transmat[3][4]={
    {609.6954091642638, -721.4215973249585, -001.2512585456598, -167.8990857473499},
    {180.3842015882277,  007.6447980192031, -719.6514740348144, -101.2330669741849},
    {000.9999453885620,  000.0001243653784,  000.0104513029957, -000.2721327964059}};
  static long double twod[3];
  twod[0]=0;
  twod[1]=0;
  twod[2]=0;
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++)
      twod[i]+=transmat[i][j]*points[j];
    twod[i]+=transmat[i][3];
  }
  for(int i=0;i<3;i++){
    twod[i]/=twod[2];
    twod[i]-=1;
  }
  return twod;
}
