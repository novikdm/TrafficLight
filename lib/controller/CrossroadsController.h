#ifndef _CROSSROADSCONTROLLER_H_
#define _CROSSROADSCONTROLLER_H_
#include <gpiod.h>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include "../config/CrossroadsConfig.h"
#include "../config/TrafficLightConfig.h"
#include "../controller/TrafficLightController.h"

class CrossroadsController {
    private:
        bool debug_mode {false};
        CrossroadsConfig *config;
        vector<TrafficLightConfig*> traffic_light_configs;
        map<string, TrafficLightController*> tl_controller_map;
        gpiod_chip* chip {nullptr};
        std::atomic<bool> *is_thread_running {nullptr};
        std::atomic<bool> *stop_thread {nullptr};

        void init();
        void init_local();
        void init_external();


    public:
        CrossroadsController(CrossroadsConfig *config, vector<TrafficLightConfig*> traffic_light_configs, bool debug_mode, gpiod_chip *chip, std::atomic<bool> *is_thread_running, std::atomic<bool> *stop_thread);
        ~CrossroadsController();
        void trafic_light_on();
        void trafic_light_off();
        void trafic_light_test();
        void trafic_light_yellow_blink();
};

#endif // _CROSSROADSCONTROLLER_H_