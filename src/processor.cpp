#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float CPU_Percentage = 0.0;

    vector<string> v_CPU_attrs = LinuxParser::CpuUtilization();

    this->idle = std::stoi(v_CPU_attrs[4]);
    this->iowait = std::stoi(v_CPU_attrs[5]);
    float Idle = this->idle + this->iowait;

    this->user = std::stoi(v_CPU_attrs[1]);
    this->nice = std::stoi(v_CPU_attrs[2]);
    this->system = std::stoi(v_CPU_attrs[3]);
    this->irq = std::stoi(v_CPU_attrs[6]);
    this->softirq = std::stoi(v_CPU_attrs[7]);
    this->steal = std::stoi(v_CPU_attrs[8]);
    float Nonidle = this->user + this->nice + this->system + this->irq + this->softirq + this->steal;

    float Total = Idle + Nonidle;
    CPU_Percentage = (Total - Idle) / Total;

    return CPU_Percentage; 
}