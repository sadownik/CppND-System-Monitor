#include "processor.h"
#include "linux_parser.h"

#include <fstream>
#include <regex>
#include <string>
#include <iostream>

using namespace std;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  string word;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  
  string keyword = "cpu";
  while(stream >> word){
    if(word == keyword){
      stream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
      break;
    }
  }
  
  long Idle = stoi(idle) + stoi(iowait);
  

  long NonIdle = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);

  long Total = Idle + NonIdle;

  // CPU_Percentage
  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;

  float CPU_Percentage = (float)(totald - idled)/totald;

  PrevIdle = Idle;
  PrevNonIdle = NonIdle;
  PrevTotal = Total;
  
  return CPU_Percentage;



}