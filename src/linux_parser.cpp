#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string extract_after_key(std::ifstream & stream, string keyword){
  string word;
  string target;
  // string keyword = "procs_running";
  while(stream >> word){
    if(word == keyword){
      stream >> target;
      break;
    }
  }
  return target; 
}



// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memtotal, memtotalsize, kb, memfree, memfreesize;
 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  stream >> memtotal >> memtotalsize >> kb >> memfree >> memfreesize;
  stream.clear();

  return (float) (std::stoi(memtotalsize) - std::stoi(memfreesize))/ std::stoi(memtotalsize);
}

// TODO: Read and return the system uptime

long LinuxParser::UpTime() { 
  
  string line;
  long n;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    linestream >> n;
    
  }
  // std::cout << n << "\n";
  return n;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
std::pair<long, long> LinuxParser::ActiveJiffies(int pid) { 

  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  int nth_element = 14; 
  string word, utime, stime, cutime, cstime, s_time;
  long total_time, starttime;
  if(stream.good()){

    for(int i = 1; i<nth_element; i++){
      stream >> word;
    }
    stream >> utime >> stime >> cutime >> cstime >> word >> word >> word >> word >> s_time;
    total_time = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
    starttime = stol(s_time);
  }
  else {
    total_time = 0;
    starttime = 0;
  } 

  
  return std::make_pair(total_time, starttime);

}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    std::ifstream stream(kProcDirectory + kStatFilename);
  string word;
  string processes;
  
  string keyword = "processes";
  // string keyword = "procs_running";
  while(stream >> word){
    if(word == keyword){
      stream >> processes;
      break;
    }
  }
  return std::stoi(processes);
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

  std::ifstream stream(kProcDirectory + kStatFilename);
  string word;
  string processes;
  
  string keyword = "procs_running";
  // string keyword = "procs_running";
  while(stream >> word){
    if(word == keyword){
      stream >> processes;
      break;
    }
  }
  return std::stoi(processes);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid)+ kCmdlineFilename);
  string word;
  stream >> word;
  return word; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatusFilename);
  return extract_after_key(stream, "VmSize:");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatusFilename); 
  return extract_after_key(stream, "Uid:"); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(std::string uid) { 
  std::ifstream stream(kPasswordPath);
  string word;
  string target;
  

  // string keyword = "procs_running";
  while(stream >> word){
    std::size_t found = word.find(uid);
    if (found!=std::string::npos){
      target.erase();
      for(unsigned int i = 0; i<word.size();i++){
        if(word[i]!=':'){
          target.push_back(word[i]);
        }
        else{
          break;
        }
      }
      break;
    }
  }
  return target; 

  return string(); 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatFilename);
  int nth_element = 22; 
  string word;
  long ticks;
  if(stream.good()){

    for(int i = 1; i<nth_element; i++){
      stream >> word;
    }
    stream >> word;
    ticks = stol(word);
  }
  else {
    return 0;
  } 
  long seconds =LinuxParser::UpTime()-  ticks / sysconf(_SC_CLK_TCK);
  return seconds; 
  

  }