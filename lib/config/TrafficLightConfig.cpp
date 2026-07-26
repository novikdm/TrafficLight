#include "TrafficLightConfig.h"
#include <string>
using namespace std;


TrafficLightConfig::TrafficLightConfig() 
    : traffic_light_id {0}, 
    traffic_light_name {"Default Traffic Light"},
    red_pin {new unsigned int(17)},
    yellow_pin {new unsigned int(27)},
    green_pin {new unsigned int(22)},
    red_time {7000},
    yellow_time {1000},
    green_time {13000},
    green_time_blinking {2000},
    yellow_blinking_period {2000} {
}

TrafficLightConfig::TrafficLightConfig(const TrafficLightConfig &source)
    : traffic_light_id {source.traffic_light_id},
      traffic_light_name {source.traffic_light_name},
      red_pin {source.red_pin},
      yellow_pin {source.yellow_pin},
      green_pin {source.green_pin},
      red_time {source.red_time},
      yellow_time {source.yellow_time},
      green_time {source.green_time},
      green_time_blinking {source.green_time_blinking},
      yellow_blinking_period {source.yellow_blinking_period} {
}

TrafficLightConfig::TrafficLightConfig(string config_file_path) {
    initialization(config_file_path);
}

TrafficLightConfig::~TrafficLightConfig() {
    // Destructor implementation
}

void TrafficLightConfig::initialization(string config_file_path) {
    // Implementation for initializing from config file
}
        

// Getterrs
int TrafficLightConfig::get_traffic_light_id() const { return traffic_light_id; }
string TrafficLightConfig::get_traffic_light_name() const { return traffic_light_name; }
unsigned int *TrafficLightConfig::get_red_pin() const { return red_pin; }
unsigned int *TrafficLightConfig::get_yellow_pin() const { return yellow_pin; }
unsigned int *TrafficLightConfig::get_green_pin() const { return green_pin; }
int TrafficLightConfig::get_red_time() const { return red_time; }
int TrafficLightConfig::get_yellow_time() const { return yellow_time; }
int TrafficLightConfig::get_green_time() const { return green_time; }
int TrafficLightConfig::get_green_time_blinking() const { return green_time_blinking; }
int TrafficLightConfig::get_yellow_blinking_period() const { return yellow_blinking_period; }

// Setters
void TrafficLightConfig::set_traffic_light_id(int id){traffic_light_id = id;}
void TrafficLightConfig::set_traffic_light_name(string instance_name){traffic_light_name = instance_name;}
void TrafficLightConfig::set_red_pin(const int red) { red_pin = new unsigned int(red); }
void TrafficLightConfig::set_yellow_pin(const int yellow) { yellow_pin = new unsigned int(yellow); }
void TrafficLightConfig::set_green_pin(const int green) { green_pin = new unsigned int(green); }
void TrafficLightConfig::set_red_time(int time) { red_time = time; }
void TrafficLightConfig::set_yellow_time(int time) { yellow_time = time; }
void TrafficLightConfig::set_green_time(int time) { green_time = time; }
void TrafficLightConfig::set_green_time_blinking(int time) { green_time_blinking = time; }
void TrafficLightConfig::set_yellow_blinking_period(int time) { yellow_blinking_period = time; }