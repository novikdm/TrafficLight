#include "TrafficLightController.h"
#include <gpiod.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include "../logger/Logger.cpp"

TrafficLightController::TrafficLightController(TrafficLightConfig *config, bool debug_mode, gpiod_chip *chip, std::atomic<bool> *is_thread_running, std::atomic<bool> *stop_thread) : config(config), chip(chip), is_thread_running(is_thread_running), stop_thread(stop_thread), debug_mode(debug_mode) {
    init_gpio_requests(chip);
}

void TrafficLightController::init_gpio_requests(gpiod_chip *chip) {
    // Request RED line
    gpiod_line_settings *settings_red = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_red, GPIOD_LINE_DIRECTION_OUTPUT);

    gpiod_line_config *cfg_red = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_red, (*config).get_red_pin(), 1, settings_red);

    gpiod_request_config *req_red = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_red, "gpio-timer");

    request_red = gpiod_chip_request_lines(chip, req_red, cfg_red);

    // Request YELLOW line
    gpiod_line_settings *settings_yellow = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_yellow, GPIOD_LINE_DIRECTION_OUTPUT);

    gpiod_line_config *cfg_yellow = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_yellow, (*config).get_yellow_pin(), 1, settings_yellow);

    gpiod_request_config *req_yellow = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_yellow, "gpio-timer");

    request_yellow = gpiod_chip_request_lines(chip, req_yellow, cfg_yellow);

    // Request GREEEN line
    gpiod_line_settings *settings_green = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_green, GPIOD_LINE_DIRECTION_OUTPUT);

    gpiod_line_config *cfg_green = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_green, (*config).get_green_pin(), 1, settings_green);

    gpiod_request_config *req_green = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_green, "gpio-timer");

    request_green = gpiod_chip_request_lines(chip, req_green, cfg_green);
}

TrafficLightController::~TrafficLightController(){
    Logger::logDebug(debug_mode, "TrafficLightController DESTRUCTOR START");
    trafic_light_off();
    release_gpiod_line_requests();
    Logger::logDebug(debug_mode, "TrafficLightController DESTRUCTOR END");
}

void TrafficLightController::trafic_light_on() {
    Logger::logDebug(debug_mode, "trafic_light_on START");

    *is_thread_running = true;
    while (!(*stop_thread))
    {
        gpiod_line_request_set_value(request_red, *((*config).get_red_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds((*config).get_red_time()));
        gpiod_line_request_set_value(request_red, *((*config).get_red_pin()), GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds((*config).get_yellow_time()));
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds((*config).get_green_time()));
        gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_INACTIVE);

        int blinking_count = (*config).get_green_time_blinking() / 400;
        for (int i = 0; i <= blinking_count; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_ACTIVE);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_INACTIVE);
        }
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_INACTIVE);
    }
    *stop_thread = false;
    *is_thread_running = false;

    Logger::logDebug(debug_mode, "trafic_light_on END");
    Logger::logDebug(debug_mode, "is_thread_running=" + std::to_string(is_thread_running->load()));
    Logger::logDebug(debug_mode, "stop_thread=" + std::to_string(stop_thread->load()));
}

void TrafficLightController::trafic_light_off() {
    Logger::logDebug(debug_mode, "trafic_light_off START");

    gpiod_line_request_set_value(request_red, *((*config).get_red_pin()), GPIOD_LINE_VALUE_INACTIVE);
    gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_INACTIVE);
    gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_INACTIVE);

    Logger::logDebug(debug_mode, "trafic_light_off END");
    Logger::logDebug(debug_mode, "is_thread_running=" + std::to_string(is_thread_running->load()));
    Logger::logDebug(debug_mode, "stop_thread=" + std::to_string(stop_thread->load()));
}

void TrafficLightController::trafic_light_test() {
    Logger::logDebug(debug_mode, "trafic_light_test START");
    Logger::logDebug(debug_mode, "Trafic light test. Light on for 0.5 seconds one by one 3 times");

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    for (int i = 0; i < 3; i++)
    {
        gpiod_line_request_set_value(request_red, *((*config).get_red_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_red, *((*config).get_red_pin()), GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_green, *((*config).get_green_pin()), GPIOD_LINE_VALUE_INACTIVE);
    }

    Logger::logDebug(debug_mode, "trafic_light_test END");
    Logger::logDebug(debug_mode, "is_thread_running=" + std::to_string(is_thread_running->load()));
    Logger::logDebug(debug_mode, "stop_thread=" + std::to_string(stop_thread->load()));
}

void TrafficLightController::trafic_light_yellow_blink() {
    Logger::logDebug(debug_mode, "trafic_light_yellow_blink START");

    *is_thread_running = true;
    while (!(*stop_thread))
    {
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds((*config).get_yellow_blinking_period()));
        gpiod_line_request_set_value(request_yellow, *((*config).get_yellow_pin()), GPIOD_LINE_VALUE_INACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds((*config).get_yellow_blinking_period()));
    }
    *stop_thread = false;
    *is_thread_running = false;

    Logger::logDebug(debug_mode, "trafic_light_yellow_blink END");
    Logger::logDebug(debug_mode, "is_thread_running=" + std::to_string(is_thread_running->load()));
    Logger::logDebug(debug_mode, "stop_thread=" + std::to_string(stop_thread->load()));
}

void TrafficLightController::release_gpiod_line_requests() {
    Logger::logDebug(debug_mode, "release_gpiod_line_requests START");

    gpiod_line_request_release(request_red);
    gpiod_line_request_release(request_yellow);
    gpiod_line_request_release(request_green);
    
    Logger::logDebug(debug_mode, "release_gpiod_line_requests END");
}
