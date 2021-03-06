#include "ncurses_display.h"
#include "system.h"

//Test includes
#include "processor.h"
#include "format.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using std::vector;
using std::string;

int main() {
  System system;
  NCursesDisplay::Display(system);

/*
//Unit Test
//case 1
Processor p;
float CPU_percentage = p.Utilization();
std::cout << CPU_percentage << "\n";

case 2
System s;
long up_time = s.UpTime();
std::cout << up_time << "\n";

case 3

System s;
vector<Process> v_process = s.Processes();
for (auto p : v_process){
  if (p.Pid() == 6051){
    std::cout << "system uptime:" << s.UpTime() << "\n";
    std::cout << "process uptime:" << p.Pid() << " " << p.UpTime() << "\n";
    std::cout << "format:" << Format::ElapsedTime(p.UpTime()) << "\n";
  }
}
*/

}