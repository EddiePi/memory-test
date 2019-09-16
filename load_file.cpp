#include <iostream>
#include <fstream>
#include <memory.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
using namespace std;

#define N 16
ifstream infiles[N];
void _read_(int index)
{
  string file_path = "./1G_";
  file_path += to_string(index);
  file_path += ".file";
  char buf[1048578];
  while(1)
  {
    infiles[index].open(file_path.c_str(), ios::in|ios::out);
    if (infiles[index].is_open())
    {
      while (!infiles[index].eof())
      {
        infiles[index].getline(buf, 1048578);
      }
      usleep(100);
    }
    else
    {
      cout << "fail to open file: " << file_path << endl;
    }
    infiles[index].close();
    usleep(10);
  }
  return;
}

int main()
{
  vector<thread> threads;
  for (int i = 0; i < N; i++)
  {
    threads.push_back(thread(&_read_, i));
  }

  /*
  read("./1G_0.file", 1);
  read("./1G_1.file", 1);
  read("./1G_2.file", 2);
  read("./1G_3.file", 3);
  read("./1G_4.file", 4);
  read("./1G_5.file", 5);
  read("./1G_6.file", 6);
  read("./1G_7.file", 7);
  read("./1G_8.file", 8);
  read("./1G_9.file", 9);
  */

  for (auto& th: threads)
  {
    th.join();
  }

  /*
  char buf[1048578];
  ifstream infile;
  infile.open("./1G_1.file", ios::in|ios::out);
  //infile.open("./load_file.cpp", ios::in|ios::out);
  if (infile.is_open())
  {
    while (!infile.eof())
    {
      infile.getline(buf, 1048578);
    }
  }
  else
  {
    cout << "fail to open file" << endl;
  }
  */
  cout << "press enter to end" << endl;
  getchar();
  return 0;
}
