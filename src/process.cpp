#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"



using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long total_time, start_time;
    std::tie(total_time,start_time) = LinuxParser::ActiveJiffies(pid_); 
    long hertz = sysconf(_SC_CLK_TCK);
    long uptime = LinuxParser::UpTime();
    
    long seconds =  uptime - (start_time/ hertz);
    if(seconds == 0){
        seconds = 1; 
    }
    
    return ((float) total_time / (float) hertz )/  (float) seconds; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    uid_ =  LinuxParser::Uid(pid_); 
    return LinuxParser::User(uid_);
    
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {

    if(a.pid_<this->pid_){
        return true;
    } 
    else {
        return false;
    }
 }
