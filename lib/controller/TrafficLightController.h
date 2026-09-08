#ifndef _TRAFFICLIGHTCONTROLLER_H_
#define _TRAFFICLIGHTCONTROLLER_H_
#include <gpiod.h>
#include <thread>
#include <atomic>
#include "../config/TrafficLightConfig.h"

class TrafficLightController {
    private:
        bool debug_mode {false};
        bool is_local {true};
        TrafficLightConfig *config;
        gpiod_chip* chip {nullptr};
        gpiod_line_request* request_red {nullptr};
        gpiod_line_request* request_yellow {nullptr};
        gpiod_line_request* request_green {nullptr};
        std::atomic<bool> *is_thread_running {nullptr};
        std::atomic<bool> *stop_thread {nullptr};

        void init_gpio_requests(gpiod_chip* chip);
        void release_gpiod_line_requests();

        void tl_on_local();
        void tl_off_local();
        void tl_test_local();
        void tl_yellow_blink_local();
        
        void tl_on_external();
        void tl_off_external();
        void tl_test_external();
        void tl_yellow_blink_external();

        // static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
        void call_api(std::string address);



    public:
        TrafficLightController(TrafficLightConfig *config, bool debug_mode, gpiod_chip* chip, std::atomic<bool> *is_thread_running, std::atomic<bool> *stop_thread);
        ~TrafficLightController();
        void trafic_light_on();
        void trafic_light_off();
        void trafic_light_test();
        void trafic_light_yellow_blink();
};

#endif // _TRAFFICLIGHTCONTROLLER_H_
