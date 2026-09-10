#ifndef _CROSSROADSCONFIGMAPPER_H_
#define _CROSSROADSCONFIGMAPPER_H_
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include "CrossroadsConfig.h"

using namespace std;
namespace pt = boost::property_tree;

class CrossroadsConfigMapper {
    private:
        string base_config_file_path {"./config.ini"};
        string crossroads_config_name {"crossroads_config_file"};
        string config_file_path {""};

        void read_config_from_base_config_file(const string& config_file_path);
        CrossroadsConfig map_from_ptree_to_object(const pt::ptree& instanse);
        pt::ptree cr_config_to_json(const map<string, CrossroadsConfig>& cr_cfg_map);
        pt::ptree cr_config_instance_to_json(const CrossroadsConfig& cfg);
    public:
        CrossroadsConfigMapper();
        CrossroadsConfigMapper(const string& base_config_file_path);
        ~CrossroadsConfigMapper();
        map<string, CrossroadsConfig> map_all_from_file();
        CrossroadsConfig map_to_object(const string &instance_name);
        void save_instance(const CrossroadsConfig& config, const string &instance_name);
        void save_all(const map<string, CrossroadsConfig>& cr_cfg_map);
};



#endif // _CROSSROADSCONFIGMAPPER_H_
