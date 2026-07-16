#include <gpiod.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <map>
#include "lib/config/TrafficLightConfigMapper.h"
#include "lib/config/TrafficLightConfig.h"
#include "lib/controller/TrafficLightController.h"

using namespace std;

gpiod_chip* chip {nullptr};
string current_traffic_light {"Default_Name"};
std::thread working_thread;
std::atomic<bool> is_thread_running(false);
std::atomic<bool> stop_thread(false);

map<string, TrafficLightConfig> traffic_light_configs;
map<string, TrafficLightController> traffic_light_controllers;
TrafficLightController *current_controller{nullptr};


void help() {
    cout << "   Available commands:\n" << std::endl;
    cout << "help : print instructions" << std::endl;
    cout << "cd <name> : choose traffic light by name" << std::endl;
    cout << "trlon : enable trafic light mode" << std::endl;
    cout << "trloff : disable all lights" << std::endl;
    cout << "trlyb : enable only yellow light in blinking mode" << std::endl;
    cout << "trltest : test trafic light - lights should blink one by one for a short period of time" << std::endl;
    cout << "exit : exit program" << std::endl;
    cout << "!!! NOTE: all commands case sensitive !!!" << std::endl;
}

void exit() {
    if (current_controller) {
        working_thread = std::thread(&TrafficLightController::trafic_light_off, current_controller);
        if (working_thread.joinable()) {
            stop_thread = true;
            working_thread.join();
        }
    }
    for(auto & pair : traffic_light_controllers) {
        pair.second.release_gpiod_line_requests();
    }
    gpiod_chip_close(chip);
    cout << "\n    Goodbye!" << std::endl;
}

void stop_working_thread() {
    if (is_thread_running) {
        stop_thread = true;
        working_thread.join();
    }
}

void read_configs() {
    TrafficLightConfigMapper mapper;
    for(const auto& item : mapper.map_all_from_file()) {
        traffic_light_configs[item.first] = item.second;
    }
}

TrafficLightController* create_controller(string instance_name, gpiod_chip* chip) {
    auto config = traffic_light_configs.find(instance_name);
    if (config != traffic_light_configs.end()) {
        return new TrafficLightController(config->second, chip);
    } else {
        std::cerr << "Error: Traffic light config not found for instance name: " << instance_name << std::endl;
        return nullptr;
    }
}

int main() {
    cout << "Initialization"<< std::flush;
    for (int i = 0; i < 5; i++) {
        cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    cout << "\nRead configs"<< std::flush;
    for (int i = 0; i < 5; i++) {
        cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    cout << "\n" << std::endl;
    cout << "\n################### Welcome to Trafic Light Simulator! ###################\n################### Version: v0.1.0-b01-SNAPSHOT ###################\n" << std::endl;



    // Open GPIO chip
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        cout << "Error: Unable to open GPIO chip: " << "/dev/gpiochip0" << std::endl;
        return 1;
    }

    help();

    string input;
    string parameter;

    do {
        input = "";
        cout << "\nCommand: ";
        cin >> input >> parameter;
        cout << std::endl;

        if (input == "help") {
            help();
        } else if (input == "cd") {
            if(parameter == "" || parameter == " ") {
                cout << "Error: name is empty" << std::endl;
            }
            if(parameter != current_traffic_light) {
                current_traffic_light = parameter;
                current_controller = create_controller(current_traffic_light, chip);
                cout << "New traffic light selected: " << current_traffic_light << std::endl;
            } else {
                cout << "Traffic light already selected: " << current_traffic_light << std::endl;
            }
        } else if (input == "trlon") {
            if(current_controller == nullptr) {
                cout << "Error: No traffic light selected. Use 'cd <name>' to select a traffic light." << std::endl;
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_on, current_controller);
        } else if (input == "trloff") {
            if(current_controller == nullptr) {
                cout << "Error: No traffic light selected. Use 'cd <name>' to select a traffic light." << std::endl;
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_off, current_controller);
        } else if (input == "trlyb") {
            if(current_controller == nullptr) {
                cout << "Error: No traffic light selected. Use 'cd <name>' to select a traffic light." << std::endl;
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_yellow_blink, current_controller);
        } else if (input == "trltest") {
            if(current_controller == nullptr) {
                cout << "Error: No traffic light selected. Use 'cd <name>' to select a traffic light." << std::endl;
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_test, current_controller);
        } else if (input == "exit") {
            stop_working_thread();
            exit();
        } else {
            cout << "\nError! Invalid command. \nEnter help to see valid commands."  << std::endl;
        }


    } while(input != "exit");

    return 0;
}