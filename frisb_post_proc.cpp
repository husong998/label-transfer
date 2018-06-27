#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

#include<dirent.h>

using namespace std;

int main()
{
  DIR *dir;
  struct dirent *ent;
  char buff[256];
  char *input = buff, *output = buff;
  dir = opendir("frisb_output/raw");
  ifstream fi;
  while ((ent=readdir(dir)) != NULL)
  {
    if (ent->d_name[0] != '0') continue;

    //open raw input file
    strcpy(input, "frisb_output/raw/");
    strcat(input, ent->d_name);
    fi.open(input);

    //open text file to write info
    strcpy(output, "frisb_output/");
    strcat(output, ent->d_name);
    freopen(output,"w",stdout);

    vector<string> s;
    string line;
    getline(fi,line);
    s.push_back(line);
    for(int i=0;i<line.length();i++)
      if(line[i]==',')line[i]='.';
    printf("%s\n", line.c_str());
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
      printf("%s\n", line.c_str());
      s.push_back(line);
    }
    fi.close();
  }
  return 0;
}
