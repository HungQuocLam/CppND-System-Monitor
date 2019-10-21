#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  return (version + " " + kernel);
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
  string line;
  string key;
  string value;
  float mem_total, mem_free;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = std::stof(value);
          //std::cout <<  "MemTotal:"<< value << std::endl;  
        }
        else if (key == "MemFree:") {
          mem_free = std::stof(value);
          //std::cout <<  "MemFree:" << value << std::endl;   
        }
      }
    }
  }
  
  return ((mem_total - mem_free)/mem_total); 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string up_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }  
  return std::stol(up_time); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string cpu_time;
  vector<string> value;
  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key == "cpu") {
        for (int i=0; i < 10; i++){
          linestream >> cpu_time;
          value.push_back(cpu_time);
          //std::cout << cpu_time << std::endl;
        }
      }
    }  
  }
  return value; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int total_processes;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = std::stof(value);
          //std::cout <<  "processes:"<< total_processes << std::endl;  
        }
      }
    }
  }
 
  return total_processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int running_processes;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = std::stof(value);
          //std::cout <<  "processes:"<< running_processes << std::endl;  
        }
      }
    }
  }
 
  return running_processes; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }  
  return line; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::State(int pid) {
  std::string line;
  std::string key;
  std::string value;
  int VmSize;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "State:") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid){
  std::string line;
  std::string key;
  std::string value;
  int VmSize;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          VmSize = std::stoi(value);
          VmSize /= 1024; // 1MB = 1024KB
        }
      }
    }
  }
  return std::to_string(VmSize);
}
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line;
  std::string key;
  std::string value;
  std::string uid;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string line;
  std::string name,uid,x;
  std::string user;
  std::string uid_st = LinuxParser::Uid(pid);
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if ((x == "x") && (uid == uid_st)) {
          user = name;
          //std::cout << name << "1"<< std::endl;
        }
      }
    }
  }
  return user;  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  string pid_para;
  vector<string> value;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (pid == std::stoi(key)) {
        for (int i=0; i < 21; i++){
          linestream >> pid_para;
          value.push_back(pid_para);
          //std::cout << cpu_time << std::endl;
        }
      }
    
  }
  long uptime = std::stol(value[kstarttime_])/sysconf(_SC_CLK_TCK);
  //std::cout << uptime << std::endl;
  return uptime; // (22) starttime
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::ProcCpuPercent(int pid) {
  string line;
  string key;
  string pid_para;
  vector<string> value;
  std::string pid_st = std::to_string(pid) ;
  std::ifstream filestream(kProcDirectory+pid_st+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (pid == std::stoi(key)) {
        for (int i=0; i < 21; i++){
          linestream >> pid_para;
          value.push_back(pid_para);
          //std::cout << cpu_time << std::endl;
        }
      }
    
  }
  const float freq = sysconf(_SC_CLK_TCK);
  const float total_s = std::stol(value[kutime_]) + (std::stol(value[kstime_]) +  std::stol(value[kcutime_]) + std::stol(value[kcstime_])) / freq;
  const float result = total_s / std::stol(value[kstarttime_]);
  return result; // (22) starttime
}