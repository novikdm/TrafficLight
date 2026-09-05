#include "CrossroadsConfigMapper.h"
#include "CrossroadsConfig.h"
#include <iostream>
#include <map>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>


CrossroadsConfigMapper::CrossroadsConfigMapper() {
    read_config_from_base_config_file(base_config_file_path);
}

CrossroadsConfigMapper::CrossroadsConfigMapper(const string& crossroads_config_file_path) {
    config_file_path = crossroads_config_file_path;
}

CrossroadsConfigMapper::~CrossroadsConfigMapper() {

}

void CrossroadsConfigMapper::read_config_from_base_config_file(const string& config_file_path) {
    try {
        pt::ptree tree;
        pt::read_ini(config_file_path, tree);
        this->config_file_path = tree.get<string>(crossroads_config_name);
    } catch (const std::exception& e) {
        std::cerr << "\nError reading base config file: " << e.what() << std::endl;
    }
}

void CrossroadsConfigMapper::save_instance(const CrossroadsConfig& config, const string &instance_name) {

}

void CrossroadsConfigMapper::save_all(const map<string, CrossroadsConfig>& cr_cfg_map) {
    
    try {
         pt::write_json(config_file_path, cr_config_to_json(cr_cfg_map));
    } catch (const std::exception& e) {
        std::cerr << "\nError saving config to file " << config_file_path << ": \n"
            << e.what() << std::endl;
    }
}

pt::ptree CrossroadsConfigMapper::cr_config_to_json(const map<string, CrossroadsConfig>& cr_cfg_map) {
    pt::ptree tree;
    for (const auto& pair : cr_cfg_map) {
        tree.add_child(pair.first, cr_config_instance_to_json(pair.second));
    }
    return tree;
}

pt::ptree CrossroadsConfigMapper::cr_config_instance_to_json(const CrossroadsConfig& cfg) {
    pt::ptree tree;

    tree.put("id", cfg.get_crossroads_id());
    tree.put("name", cfg.get_crossroads_name());
    tree.put("address", cfg.get_crossroads_address());
    tree.put("type", cfg.get_crossroads_type());
    tree.put("traffic_lights", cfg.get_traffic_lights());
    
    return tree;
}

CrossroadsConfig CrossroadsConfigMapper::map_to_object(const string &instance_name) {
    CrossroadsConfig result;
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

map<string, CrossroadsConfig> CrossroadsConfigMapper::map_all_from_file() {
    std::map<std::string, CrossroadsConfig> result;
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

CrossroadsConfig CrossroadsConfigMapper::map_from_ptree_to_object(const pt::ptree& instanse) {
    CrossroadsConfig config;
    config.set_crossroads_id(instanse.get<int>("id"));
    config.set_crossroads_name(instanse.get<std::string>("name"));
    config.set_crossroads_address(instanse.get<std::string>("address"));
    config.set_crossroads_type(instanse.get<std::string>("type"));
    config.set_traffic_lights(instanse.get<std::vector<std::string>>("traffic_lights"));
    return config;
}
