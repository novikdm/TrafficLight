#include <gpiod.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

const unsigned int RED_PIN = 17;
const unsigned int YELLOW_PIN = 27;
const unsigned int GREEN_PIN = 22;

gpiod_line_request* request_red {nullptr};
gpiod_line_request* request_yellow {nullptr};
gpiod_line_request* request_green {nullptr};

std::thread working_thread;

std::atomic<bool> is_thread_running(false);
std::atomic<bool> stop_thread(false);


void init_gpio_requests(gpiod_chip* chip) {
    // Request RED line
    gpiod_line_settings* settings_red = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_red, GPIOD_LINE_DIRECTION_OUTPUT);
    
    gpiod_line_config* cfg_red = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_red, &RED_PIN, 1, settings_red);
    
    gpiod_request_config* req_red = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_red, "gpio-timer");
    
    request_red = gpiod_chip_request_lines(chip, req_red, cfg_red);
    
    // Request YELLOW line
    gpiod_line_settings* settings_yellow = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_yellow, GPIOD_LINE_DIRECTION_OUTPUT);

    gpiod_line_config* cfg_yellow = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_yellow, &YELLOW_PIN, 1, settings_yellow);
    
    gpiod_request_config* req_yellow = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_yellow, "gpio-timer");

    request_yellow = gpiod_chip_request_lines(chip, req_yellow, cfg_yellow);

     // Request GREEEN line
     gpiod_line_settings* settings_green = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings_green, GPIOD_LINE_DIRECTION_OUTPUT);
    
    gpiod_line_config* cfg_green = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_green, &GREEN_PIN, 1, settings_green);
    
    gpiod_request_config* req_green = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_green, "gpio-timer");
    
    request_green = gpiod_chip_request_lines(chip, req_green, cfg_green);
}

void trafic_light_on() {
    is_thread_running = true;
    while(!stop_thread) {
        gpiod_line_request_set_value(request_red, RED_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
        gpiod_line_request_set_value(request_red, RED_PIN, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(13000));
        gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_INACTIVE);
        for(int i = 0; i < 5; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_ACTIVE);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_INACTIVE);
        }
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_INACTIVE);
    }
    stop_thread = false;
    is_thread_running = false;
}

void trafic_light_off() {
    gpiod_line_request_set_value(request_red, RED_PIN, GPIOD_LINE_VALUE_INACTIVE);
    gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_INACTIVE);
    gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_INACTIVE);
}

void trafic_light_test() {
    cout << "Trafic light test. Light on for 0.5 seconds one by one 3 times" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    for(int i = 0; i < 3; i++){
        gpiod_line_request_set_value(request_red, RED_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_red, RED_PIN, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_request_set_value(request_green, GREEN_PIN, GPIOD_LINE_VALUE_INACTIVE);
    }
}

void trafic_light_yellow_blink() {
    is_thread_running = true;
    while(!stop_thread) {
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_ACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        gpiod_line_request_set_value(request_yellow, YELLOW_PIN, GPIOD_LINE_VALUE_INACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    stop_thread = false;
    is_thread_running = false;
}

void help() {
    cout << "   Available commands:\n" << std::endl;
    cout << "help : print instructions" << std::endl;
    cout << "trlon : enable trafic light mode" << std::endl;
    cout << "trloff : disable all lights" << std::endl;
    cout << "trlyb : enable only yellow light in blinking mode" << std::endl;
    cout << "trltest : test trafic light - lights should blink one by one for a short period of time" << std::endl;
    cout << "exit : exit program" << std::endl;
    cout << "!!! NOTE: all commands case sensitive !!!" << std::endl;
}

void exit() {
    trafic_light_off();
    if (t.joinable()) {
        stop_thread = true;
        t.join();
    }
    gpiod_line_request_release(request_red);
    gpiod_line_request_release(request_yellow);
    gpiod_line_request_release(request_green);
    gpiod_chip_close(chip);
    cout << "\n    Goodbye!" << std::endl;
}

void stop_working_thread() {
    if (is_thread_running) {
        stop_thread = true;
        t.join();
    }
}

int main() {
    cout << "\n################### Welcome to Trafic Light Simulator! ###################\n################### Version: v0.0.1 ###################\n" << std::endl;
    
    // Open GPIO chip
    gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        cout << "Error: Unable to open GPIO chip: " << "/dev/gpiochip0" << std::endl;
        return 1;
    }

    init_gpio_requests(chip);

    if (!request_red || !request_yellow || !request_green) {
        cout << "Error: gpiod_chip_request_lines" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    help();

    string input;

    do {
        input = "";
        cout << "\nCommand: ";
        cin >> input;
        cout << std::endl;

        if (input == "help") {
            help();
        } else if (input == "trlon") {
            stop_working_thread();
            working_thread = std::thread(trafic_light_on);
        } else if (input == "trloff") {
            stop_working_thread();
            trafic_light_off();
        } else if (input == "trlyb") {
            stop_working_thread();
            working_thread = std::thread(trafic_light_yellow_blink);
        } else if (input == "trltest") {
            stop_working_thread();
            trafic_light_test();
        } else if (input == "exit") {
            stop_working_thread();
            exit();
        } else {
            cout << "\nError! Invalid command. \nEnter help to see valid commands."  << std::endl;
        }


    } while(input != "exit");

    return 0;
}