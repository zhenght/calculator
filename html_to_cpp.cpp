//this small project prepares a static vector_partition.html.cpp file from the
//vector_partition.html file.
//You do not need to use this program. You can directly modify the vector_partition.html.cpp file.
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//This is needed since to avoid reading a static file on the file system

#include <assert.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>

void getPathTemp(char* path, std::string& output)
{ int length=0;
  if (path==0) return;
  while (path[length]!=0)
    length++;
  output= path;
  for (int i=length-1;i>=0;i--)
#ifndef WIN32
    if (path[i]=='/')
    { output.resize(i+1);
      return;
    }
#else
    if (path[i]=='\\')
    { output.resize(i+1);
      return;
    }
#endif
}

void htmlTocpp(std::string& input, std::string& output)
{ std::stringstream out;
  out<< "output <<\" ";
  for (unsigned int i=0; i<input.length();i++)
  { if (input[i]=='\"')
      out <<"\\\"";
    else
    { if (input[i]=='\\')
        out <<"\\\\";
      else
      { out <<input[i];
      }
    }
  }
  out <<"\\n\";";
  output = out.str();
}

int main(int argc, char **argv)
{ std::string tempS, tempS2, tempS3;
  getPathTemp(argv[0], tempS);
  tempS2=tempS;
  tempS3=tempS;
  tempS.append("../vector_partition.html");
  tempS2.append("../vector_partition.html.cpp");
  tempS3.append("../polyhedra.cpp");
  std::stringstream BufferIO;
  std::fstream fileHeaderHtml;
  std::fstream fileout;
  fileHeaderHtml.open(tempS.c_str(),std::fstream::in|std::fstream::out);
  fileout.open(tempS2.c_str(), std::fstream::out| std::fstream::trunc);
  fileHeaderHtml.clear(std::ios::goodbit);
  fileHeaderHtml.seekp(0,std::ios_base::end);
  fileHeaderHtml.seekg(0);
  char buffer[10000];
  while (!fileHeaderHtml.eof())
  { fileHeaderHtml.read(buffer,5000);
		BufferIO.write(buffer, fileHeaderHtml.gcount());
  }
  fileHeaderHtml.close();
  BufferIO <<"\n<EndOfFile>";
  BufferIO.seekg(0);
  fileout <<"#include<sstream>\n";
  fileout <<"void static_html1(std::stringstream& output){\n";
  for (;;)
  { BufferIO.getline(buffer,5000);
    tempS=buffer;
    if (tempS=="<EndOfFile>")
      break;
    htmlTocpp(tempS, tempS2);
    fileout << tempS2 <<"\n";
  }
  fileout <<"}";
  fileout.flush();
  fileout.close();
  
  
  return 0;
}
