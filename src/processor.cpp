#include <unistd.h>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> value = LinuxParser::CpuUtilization(); 

    float user          = std::stof(value[0]); 
    float nice          = std::stof(value[1]);
    float system        = std::stof(value[2]); 
    float idle          = std::stof(value[3]); 
    float iowait        = std::stof(value[4]); 
    float irq           = std::stof(value[5]); 
    float softirq       = std::stof(value[6]); 
    float steal         = std::stof(value[7]); 
    float guest         = std::stof(value[8]); 
    float guest_nice    = std::stof(value[9]); 

    float Total_CPU_time_since_boot = user + nice + system + idle + iowait + irq + softirq + steal;
    //std::cout << Total_CPU_time_since_boot << std::endl;
    float Total_CPU_Idle_time_since_boot = idle + iowait; 
    //std::cout << Total_CPU_Idle_time_since_boot << std::endl;
    float Total_CPU_usage_time_since_boot = Total_CPU_time_since_boot - Total_CPU_Idle_time_since_boot;
    //std::cout << Total_CPU_usage_time_since_boot << std::endl;
    cpu_percentage = Total_CPU_usage_time_since_boot/Total_CPU_time_since_boot;

return cpu_percentage; 
}