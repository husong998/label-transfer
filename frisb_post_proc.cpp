#include<cstdio>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
using namespace std;

int main()
{
  ifstream fi;
  FILE *fo=fopen("outfile","w");
  fi.open("output");
  vector<string> s;
  string line;
  if(!fi.is_open()) return 0;
  getline(fi,line);
  s.push_back(line);
  for(int i=0;i<line.length();i++)
    if(line[i]==',')line[i]='.';
  cout<<line<<endl;
  //float a,b,c;
  //stringstrea ss(line);
  //ss>>a>>b>>c;
  //fprintf(fo,"%.4f %.4f %.4f\n",
      //a,b,c);

  while(getline(fi,line)){
    if(0==s[0].compare(line))
        break;
    if(line[0]==0)continue;
    for(int i=0;i<line.length();i++)
      if(line[i]==',')line[i]='.';
    //stringstream ss(line);
    //ss>>a>>b>>c;
    //fprintf(fo,"%.4f %.4f %.4f\n",
        //a,b,c);
    cout<<line<<endl;
    s.push_back(line);
  }
  fi.close();
  fclose(fo);
  return 0;
}
