#include "TrafficLightConfigMapper.h"
#include "TrafficLightConfig.h"
#include <iostream>
#include <map>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;


TrafficLightConfigMapper::TrafficLightConfigMapper() {
    read_config_from_base_config_file(base_config_file_path);
}

TrafficLightConfigMapper::TrafficLightConfigMapper(const string& traffic_light_config_file_path) {
    config_file_path = traffic_light_config_file_path;
}

TrafficLightConfigMapper::~TrafficLightConfigMapper() {

}

void TrafficLightConfigMapper::read_config_from_base_config_file(const string& config_file_path) {
    try {
        pt::ptree tree;
        pt::read_ini(config_file_path, tree);
        this->config_file_path = tree.get<string>(traffic_light_config_name);
    } catch (const std::exception& e) {
        std::cerr << "\nError reading base config file: " << e.what() << std::endl;
    }
}

void TrafficLightConfigMapper::save_instance(const TrafficLightConfig& config, const string &instance_name) {

}

void TrafficLightConfigMapper::save_all(const map<string, TrafficLightConfig>& tl_cfg_map) {
    
    try {
         pt::write_json(config_file_path, tl_config_to_json(tl_cfg_map));
    } catch (const std::exception& e) {
        std::cerr << "\nError saving config to file " << config_file_path << ": \n"
            << e.what() << std::endl;
    }
}

pt::ptree TrafficLightConfigMapper::tl_config_to_json(const map<string, TrafficLightConfig>& tl_cfg_map) {
    pt::ptree tree;
    for (const auto& pair : tl_cfg_map) {
        tree.add_child(pair.first, tl_config_instance_to_json(pair.second));
    }
    return tree;
}

pt::ptree TrafficLightConfigMapper::tl_config_instance_to_json(const TrafficLightConfig& cfg) {
    pt::ptree tree;

    tree.put("id", cfg.get_traffic_light_id());
    tree.put("name", cfg.get_traffic_light_name());
    tree.put("address", cfg.get_traffic_light_address());
    tree.put("tlon_endpoint", cfg.get_tlon_endpoint());
    tree.put("tloff_endpoint", cfg.get_tloff_endpoint());
    tree.put("tlt_endpoint", cfg.get_tlt_endpoint());
    tree.put("tlyb_endpoint", cfg.get_tlyb_endpoint());
    tree.put("red_pin", cfg.get_red_pin());
    tree.put("yellow_pin", cfg.get_yellow_pin());
    tree.put("green_pin", cfg.get_green_pin());
    tree.put("start_delay", cfg.get_start_delay());
    tree.put("red_time", cfg.get_red_time());
    tree.put("yellow_time", cfg.get_yellow_time());
    tree.put("green_time", cfg.get_green_time());
    tree.put("green_time_blinking", cfg.get_green_time_blinking());
    tree.put("yellow_blinking_period", cfg.get_yellow_blinking_period());
    
    return tree;
}

TrafficLightConfig TrafficLightConfigMapper::map_to_object(const string &instance_name) {
    TrafficLightConfig result;
    try {
        pt::ptree tree;
        pt::read_json(config_file_path, tree);
        for(const auto& item : tree) {
            const std::string& key = item.first;
            if(key == instance_name) {
                const pt::ptree& instanse = item.second;
                return map_from_ptree_to_object(instanse);
            }

        }
    } catch (const std::exception& e) {
        std::cerr << "\nError reading config from file " << config_file_path << ": \n"
            << e.what() << std::endl;
    }
    std::cerr << "\nError reading config from file: Instanse '" << instance_name << "' not found!"<< std::endl;
    return result;
}

map<string, TrafficLightConfig> TrafficLightConfigMapper::map_all_from_file() {
    std::map<std::string, TrafficLightConfig> result;
    try {
        pt::ptree tree;
        pt::read_json(config_file_path, tree);
        for(const auto& item : tree) {
            const std::string& key = item.first;
            const pt::ptree& instanse = item.second;

            result[key] = map_from_ptree_to_object(instanse);
        }
    } catch (const std::exception& e) {
        std::cerr << "\nError reading config from file " << config_file_path << ": \n"
            << e.what() << std::endl;
    }
    return result;
}

TrafficLightConfig TrafficLightConfigMapper::map_from_ptree_to_object(const pt::ptree& instanse) {
    TrafficLightConfig config;
    config.set_traffic_light_id(instanse.get<int>("id"));
    config.set_traffic_light_name(instanse.get<std::string>("name"));
    config.set_traffic_light_address(instanse.get<std::string>("address"));
    config.set_tlon_endpoint(instanse.get<std::string>("tlon_endpoint"));
    config.set_tloff_endpoint(instanse.get<std::string>("tloff_endpoint"));
    config.set_tlt_endpoint(instanse.get<std::string>("tlt_endpoint"));
    config.set_tlyb_endpoint(instanse.get<std::string>("tlyb_endpoint"));
    config.set_red_pin(instanse.get<int>("red_pin"));
    config.set_green_pin(instanse.get<int>("green_pin"));
    config.set_yellow_pin(instanse.get<int>("yellow_pin"));
    config.set_start_delay(instanse.get<int>("start_delay"));
    config.set_red_time(instanse.get<int>("red_time"));
    config.set_yellow_time(instanse.get<int>("yellow_time"));
    config.set_green_time(instanse.get<int>("green_time"));
    config.set_green_time_blinking(instanse.get<int>("green_time_blinking"));
    config.set_yellow_blinking_period(instanse.get<int>("yellow_blinking_period"));
    return config;
}
