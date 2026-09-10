#ifndef _TRAFFICLIGHTCONFIGMAPPER_H_
#define _TRAFFICLIGHTCONFIGMAPPER_H_
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include "TrafficLightConfig.h"

namespace pt = boost::property_tree;

class TrafficLightConfigMapper {
    private:
        std::string base_config_file_path {"./config.ini"};
        std::string traffic_light_config_name {"traffic_light_config_file"};
        std::string config_file_path {""};

        void read_config_from_base_config_file(const std::string& config_file_path);
        TrafficLightConfig map_from_ptree_to_object(const pt::ptree& instanse);
        pt::ptree tl_config_to_json(const std::map<std::string, TrafficLightConfig>& tl_cfg_map);
        pt::ptree tl_config_instance_to_json(const TrafficLightConfig& cfg);
    public:
        TrafficLightConfigMapper();
        TrafficLightConfigMapper(const std::string& base_config_file_path);
        ~TrafficLightConfigMapper();
        std::map<std::string, TrafficLightConfig> map_all_from_file();
        TrafficLightConfig map_to_object(const std::string &instance_name);
        void save_instance(const TrafficLightConfig& config, const std::string &instance_name);
        void save_all(const std::map<std::string, TrafficLightConfig>& tl_cfg_map);
};



#endif // _TRAFFICLIGHTCONFIGMAPPER_H_
