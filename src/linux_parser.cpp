#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

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
  string os, version, kernel;
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
  string line;
  string key, value;
  float MemTotal, MemFree;
  string unit;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal") {
          MemTotal = std::stof(value);
        }
        else if(key == "MemFree"){
          MemFree = std::stof(value);
        }
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime, idle_time;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_time;
  }

  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> res;
  string cpu_label, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    res.push_back(cpu_label);
    res.push_back(user);
    res.push_back(nice);
    res.push_back(system);
    res.push_back(idle);
    res.push_back(iowait);
    res.push_back(irq);
    res.push_back(softirq);
    res.push_back(steal);
    res.push_back(guest);
    res.push_back(guest_nice);

  }
  
  return res; 

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int value = 0;
  string line;
  string key;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }

  return value; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int value = 0;
  string line;
  string key;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }

  return value; 
}

// return the cpu utilization for a single process
float LinuxParser::ProcessCpuUtilization(int pid) {
  float cpu_usage = 0.0;
  float Hertz = sysconf(_SC_CLK_TCK);
  string _pid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, \
         majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, \
         starttime, vsize, rss, rsslim, startcode, endcode, startstack, kstkesp, kstkeip, signal, \
         blocked, sigignore, sigcatch, wchain, nswap, cnswap, exit_signal, processor, rt_priority, \
         policy, delayacct_blkio_ticks, guest_time, cguest_time, start_data, end_data, start_brk, \
         arg_start, arg_end, env_start, env_end, exit_code;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatFilename  );

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> _pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> \
               majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> \
               starttime >> vsize >> rss >> rsslim >> startcode >> endcode >> startstack >> kstkesp >> kstkeip >> signal >> \
               blocked >> sigignore >> sigcatch >> wchain >> nswap >> cnswap >> exit_signal >> processor >> rt_priority >> \
               policy >> delayacct_blkio_ticks >> guest_time >> cguest_time >> start_data >> end_data >> start_brk >> \
               arg_start >> arg_end >> env_start >> env_end >> exit_code;

    float total_time = std::stof(utime) + std::stof(stime) + std::stof(cutime) + std::stof(cstime);
    float seconds = UpTime() - (std::stof(starttime) / Hertz);
    cpu_usage = ( total_time / Hertz) / seconds ;
  }
  
  return cpu_usage; 
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kCmdlineFilename );

  if (stream.is_open()) {
    std::getline(stream, line);
    command = line; 
  }

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  int res = 0;
  string line;
  string key;
  float value, VmSize;
  string unit;

  std::ifstream filestream(kProcDirectory + to_string(pid) + "/"  + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "VmSize") {
          VmSize = value;
        }
      }
    }
  }
  res = VmSize / 1000;
  return to_string(res); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value1, value2, value3, value4;
  string res;

  std::ifstream filestream(kProcDirectory + to_string(pid) + "/"  + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value1 >> value2 >> value3 >> value4) {
        if (key == "Uid") {
          res = value1;
        }
      }
    }
  }
  return res; 
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string target_uid = Uid(pid);
  string line;
  string name, permission, uid;
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> permission >> uid) {
        if (uid == target_uid) {
          return name;
        }
      }
    }
  }

  return name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) {
  long int res;
  string token;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/" + kStatFilename  );

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int counter = 1;
    while (linestream >> token){
      if (counter == 22){ //starttime
        res =  UpTime() - ( std::stol(token) / sysconf(_SC_CLK_TCK) );
        break;
      }
      counter ++;
    }
  }
    
  return res;
}