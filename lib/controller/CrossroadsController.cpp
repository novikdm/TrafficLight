#include "CrossroadsController.h"
#include <gpiod.h>
#include <thread>
#include <atomic>
#include "../config/CrossroadsConfig.h"



CrossroadsController::CrossroadsController(CrossroadsConfig *config, vector<TrafficLightConfig*> traffic_light_configs, bool debug_mode, gpiod_chip *chip, std::atomic<bool> *is_thread_running, std::atomic<bool> *stop_thread) 
    : config(config), traffic_light_configs(traffic_light_configs), chip(chip), is_thread_running(is_thread_running), stop_thread(stop_thread), debug_mode(debug_mode) {
    init();
}

CrossroadsController::~CrossroadsController(){}

void CrossroadsController::init() {
    init_local();
    init_external();
}

void CrossroadsController::init_local() {

}



        void trafic_light_on();
        void trafic_light_off();
        void trafic_light_test();
        void trafic_light_yellow_blink();
