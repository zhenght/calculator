//this small project prepares a static vector_partition.html.cpp file from the
//vector_partition.html file.
//You do not need to use this program. You can directly modify the vector_partition.html.cpp file.
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//This is needed since it is practically impossible to read
//a static file on the file system in a system independent fashion.
//I have neither the time nor the inclination to write
//code for opening a file in every possible system and circumstance.
//After all, I want to keep my file i/o routines to less than 10k lines of code
//(does it actually suffice to write 10k lines of code to open a file?)

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
    if (path[i]=='/')
    { output.resize(i+1);
      return;
    }
}

void htmlTocpp(std::string& input, std::string& output)
{ output=input;
}

int main(int argc, char **argv)
{ std::string tempS,tempS2;
  getPathTemp(argv[0],tempS);
  tempS2=tempS;
  tempS.append("../vector_partition.html");
  tempS2.append("../vector_partition.html");
  std::stringstream BufferIO;
  std::fstream fileHeaderHtml;
  std::fstream fileout;
  fileHeaderHtml.open(tempS.c_str(),std::fstream::in|std::fstream::out);
  fileout.open(tempS2.c_str(), std::fstream::out| std::fstream::trunc);
  fileHeaderHtml.clear(std::ios::goodbit);
  fileHeaderHtml.seekp(0,std::ios_base::end);
  fileHeaderHtml.seekg(0);
  char buffer[2000];
  while (!fileHeaderHtml.eof())
  { fileHeaderHtml.read(buffer,1024);
		BufferIO.write(buffer,fileHeaderHtml.gcount());
  }
  fileHeaderHtml.close();
  int bufferSize=BufferIO.tellp();
  BufferIO.seekg(0);;

  while (BufferIO.tellg()<bufferSize)
  { BufferIO >> tempS;
    htmlTocpp(tempS, tempS2);
    fileout << tempS2;
  }
  fileout.flush();
  fileout.close();
  return 0;
}
