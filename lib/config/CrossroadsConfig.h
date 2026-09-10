#ifndef _CROSSROADSCONFIG_H_
#define _CROSSROADSCONFIG_H_
#include <string>
#include <vector>
using namespace std;

class CrossroadsConfig {
    private:
        int crossroads_id; // unique identifier for the crossroads configuration
        string crossroads_name; // unique name for the crossroads configuration instance
        string crossroads_address; // address for the crossroads configuration instance
        string crossroads_type; // type of the crossroads (e.g., right_angle, intersection)
        std::vector<std::string> traffic_lights; // list of traffic lights associated with this crossroads
    public:
        CrossroadsConfig();
        CrossroadsConfig(string config_file_path);
        CrossroadsConfig(const CrossroadsConfig &source);
        ~CrossroadsConfig();

        void initialization(string config_file_path);

        int get_crossroads_id() const;
        string get_crossroads_name() const;
        string get_crossroads_address() const;
        string get_crossroads_type() const;
        const std::vector<std::string>& get_traffic_lights() const;

        void set_crossroads_id(int id);
        void set_crossroads_name(string instance_name);
        void set_crossroads_address(string address);
        void set_crossroads_type(string type);
        void set_traffic_lights(const std::vector<std::string>& light_ids);
        void add_traffic_light(const std::string& light_id);
};

#endif // _CROSSROADSCONFIG_H_