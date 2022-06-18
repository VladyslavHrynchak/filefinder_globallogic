#include "file_finder.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

using namespace std;
using namespace filesystem;

bool search_file(const string& path,const string& name_of_file)
{
   for (const auto& entry : recursive_directory_iterator(path,filesystem::directory_options::skip_permission_denied))
   {
        if (entry.path().filename().string() == name_of_file)
        {
            cout << entry.path().string()<< endl;
            return true;
        }
    }
    return false;
}

void find_file(const std::string& name)
{
    const string rootPath = "/";
    const uint maxThreads = 8;

    atomic_bool stop = false;
    vector<thread> threads;

    queue<string> subdirs;
    mutex m;

    for (const auto& entry : directory_iterator(rootPath))
    {
        if(entry.is_directory())
        {
            subdirs.push(entry.path().string());
        }
    }

    threads.reserve(maxThreads);
    for (int i = 0; i < maxThreads; ++i)
    {
       threads.push_back(thread([&]()
       {
          while(true)
          {
             if(stop)
               return;

             string path;
             m.lock();
             if(!subdirs.empty())
             {
                path = subdirs.front();
                subdirs.pop();
             }
             else
             {
                m.unlock();
                return;
             }
             m.unlock();

             if(search_file(path,name))
                stop = true;
          }
        }));
    }

    for (auto &t : threads)
        t.join();
}
