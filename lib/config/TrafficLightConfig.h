#ifndef _TRAFFICLIGHTCONFIG_H_
#define _TRAFFICLIGHTCONFIG_H_
#include <string>
using namespace std;

class TrafficLightConfig {
    private:
        int traffic_light_id; // unique identifier for the traffic light configuration
        string traffic_light_name; // unique name for the traffic light configuration instance
        unsigned int *red_pin {nullptr}; // GPIO pin number for the red light
        unsigned int *yellow_pin {nullptr}; // GPIO pin number for the yellow light
        unsigned int *green_pin {nullptr}; // GPIO pin number for the green light
        int red_time;
        int yellow_time;
        int green_time;
        int green_time_blinking;
        int yellow_blinking_period;
    public:

        TrafficLightConfig();
        TrafficLightConfig(string config_file_path);
        TrafficLightConfig(const TrafficLightConfig &source);
        ~TrafficLightConfig();

        void initialization(string config_file_path);

        int get_traffic_light_id() const;
        string get_traffic_light_name() const;
        unsigned int *get_red_pin() const;
        unsigned int *get_yellow_pin() const;
        unsigned int *get_green_pin() const;
        int get_red_time() const;
        int get_yellow_time() const;
        int get_green_time() const;
        int get_green_time_blinking() const;
        int get_yellow_blinking_period() const;

        void set_traffic_light_id(int id);
        void set_traffic_light_name(string instance_name);
        void set_red_pin(const int red);
        void set_yellow_pin(const int yellow);
        void set_green_pin(const int green);
        void set_red_time(int time);
        void set_yellow_time(int time);
        void set_green_time(int time);
        void set_green_time_blinking(int time);
        void set_yellow_blinking_period(int time);
        

};

#endif // _TRAFFICLIGHTCONFIG_H_