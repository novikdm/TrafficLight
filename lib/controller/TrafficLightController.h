#ifndef _TRAFFICLIGHTCONTROLLER_H_
#define _TRAFFICLIGHTCONTROLLER_H_
#include <gpiod.h>
#include <thread>
#include <atomic>
#include "../config/TrafficLightConfig.h"

class TrafficLightController {
    private:
        TrafficLightConfig config;
        gpiod_chip* chip {nullptr};
        gpiod_line_request* request_red {nullptr};
        gpiod_line_request* request_yellow {nullptr};
        gpiod_line_request* request_green {nullptr};
        std::thread working_thread {nullptr};
        std::atomic<bool> is_thread_running {false};
        std::atomic<bool> stop_thread {false};

        void init_gpio_requests(gpiod_chip* chip);


    public:
    
        TrafficLightController(TrafficLightConfig config, gpiod_chip* chip);
        void trafic_light_on();
        void trafic_light_off();
        void trafic_light_test();
        void trafic_light_yellow_blink();
        void release_gpiod_line_requests();
};

#endif // _TRAFFICLIGHTCONTROLLER_H_