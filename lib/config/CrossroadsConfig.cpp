#include "CrossroadsConfig.h"
#include <string>
using namespace std;


CrossroadsConfig::CrossroadsConfig() 
    : crossroads_id {0}, 
    crossroads_name {"Default Crossroads"},
    crossroads_address {"Default Address"},
    crossroads_type {"Default Type"} {
}

CrossroadsConfig::CrossroadsConfig(const CrossroadsConfig &source)
    : crossroads_id {source.crossroads_id},
      crossroads_name {source.crossroads_name},
      crossroads_address {source.crossroads_address},
      crossroads_type {source.crossroads_type} {
}

CrossroadsConfig::CrossroadsConfig(string config_file_path) {
    initialization(config_file_path);
}

CrossroadsConfig::~CrossroadsConfig() {
    // Destructor implementation
}

void CrossroadsConfig::initialization(string config_file_path) {
    // Implementation for initializing from config file
}
        

// Getters
int CrossroadsConfig::get_crossroads_id() const { return crossroads_id; }
string CrossroadsConfig::get_crossroads_name() const { return crossroads_name; }
string CrossroadsConfig::get_crossroads_address() const { return crossroads_address; }
string CrossroadsConfig::get_crossroads_type() const { return crossroads_type; }
const std::vector<std::string>& CrossroadsConfig::get_traffic_lights() const { return traffic_lights; }

// Setters
void CrossroadsConfig::set_crossroads_id(int id){crossroads_id = id;}
void CrossroadsConfig::set_crossroads_name(string instance_name){crossroads_name = instance_name;}
void CrossroadsConfig::set_crossroads_address(string address){crossroads_address = address;}
void CrossroadsConfig::set_crossroads_type(string type){crossroads_type = type;}
void CrossroadsConfig::set_traffic_lights(const std::vector<std::string>& light_ids) { traffic_lights = light_ids; }
void CrossroadsConfig::add_traffic_light(const std::string& light_id){traffic_lights.push_back(light_id);}