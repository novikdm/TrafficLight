#ifndef _TRAFFICLIGHTCONTROLLER_H_
#define _TRAFFICLIGHTCONTROLLER_H_
#include <gpiod.h>
#include <thread>
#include <atomic>
#include "../config/TrafficLightConfig.h"

class TrafficLightController {
    private:
        bool debug_mode {false};
        TrafficLightConfig *config;
        gpiod_chip* chip {nullptr};
        gpiod_line_request* request_red {nullptr};
        gpiod_line_request* request_yellow {nullptr};
        gpiod_line_request* request_green {nullptr};
        std::atomic<bool> *is_thread_running {nullptr};
        std::atomic<bool> *stop_thread {nullptr};

        void init_gpio_requests(gpiod_chip* chip);


    public:
        TrafficLightController(TrafficLightConfig *config, bool debug_mode, gpiod_chip* chip, std::atomic<bool> *is_thread_running, std::atomic<bool> *stop_thread);
        ~TrafficLightController();
        void trafic_light_on();
        void trafic_light_off();
        void trafic_light_test();
        void trafic_light_yellow_blink();
        void release_gpiod_line_requests();
};

#endif // _TRAFFICLIGHTCONTROLLER_H_