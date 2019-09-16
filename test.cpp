#include <stdio.h>
#include <string>
#include <memory.h>
#include <set>
#include <iostream>
#include <sstream>
#include <regex>

using namespace std;

set<string> files;

void filter_file(vector<string> lsof_ret)
{
  //stringstream ss;
  string tmp;
  set<string> res;
  for (auto elem: lsof_ret)
  {
    vector<string> tokens;
    stringstream ss(elem);
    //cout << "elem: " << ss.str() << endl;
    while (ss >> tmp)
    {
      tokens.push_back(tmp);
    }
    if (tokens.size() != 9)
    {
      cout << "parts less than 9" << endl;
      continue;
    }
    else if (tokens[4] == "REG" && 
          regex_match(tokens[3], regex("\\d+(u|w)")))
    {
      files.insert(tokens[8]);
    }
  }
}

void get_open_file(int pid)
{
  FILE *fstream = NULL;
  char buff[1024];
  vector<string> lsof_ret;
  string command = "lsof -p ";
  command += to_string(pid);
  if ((fstream = popen(command.c_str(), "r")) == NULL)
  {
    fprintf(stderr, "lsof command failed: %s\n", strerror(errno));
    return;
  }
  while (fgets(buff, sizeof(buff), fstream) != NULL)
  {
    //cout << buff;
    string s = buff;
    lsof_ret.push_back(s);
    memset(buff, 0, sizeof(buff));
  }
  filter_file(lsof_ret);
}

int main(int argc, char* argv[])
{
  FILE *fstream = NULL;
  char buff[1204];
  set<int> pid_set;
  if ((fstream = popen("pgrep load_file", "r")) == NULL)
  {
    fprintf(stderr, "command failed: %s\n", strerror(errno));
  }
  while ( fgets(buff, sizeof(buff), fstream) != NULL)
  {
    pid_set.insert(atoi(buff));
    memset(buff, 0, sizeof(buff));
  }

  for (auto elem: pid_set)
  {
    cout << elem << endl;
    get_open_file(elem);
  }

  for(auto file: files)
  {
    cout << file << endl;
  }


  pclose(fstream);
  return 0;
}
