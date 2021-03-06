#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
    return this->Pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    this->CpuUtilization_ = LinuxParser::ProcessCpuUtilization(this->Pid_);
    return this->CpuUtilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(this->Pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(this->Pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(this->Pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(this->Pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->CpuUtilization_ < a.CpuUtilization_ ;
}
