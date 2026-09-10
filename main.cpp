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
#include <boost/property_tree/ini_parser.hpp>
#include "lib/logger/Logger.cpp"

using namespace std;

string version = "v0.1.0-b01-SNAPSHOT";
string config_file_path = "./config.ini";
string traffic_light_config_name {"traffic_light_config_file"};
string traffic_light_config_file_path {"./configs/traffic_light_config.json"};
bool debug_mode = false;

gpiod_chip* chip {nullptr};
string current_traffic_light {"Default_Name"};
std::thread working_thread;
std::atomic<bool> *is_thread_running(nullptr);
std::atomic<bool> *stop_thread(nullptr);

map<string, TrafficLightConfig> traffic_light_configs;
map<string, TrafficLightController *> traffic_light_controllers;
TrafficLightController *current_controller{nullptr};


void help() {
    Logger::logInfo( "   Available commands:\n");
    Logger::logInfo("help : print instructions");
    Logger::logInfo("rc : reload configs");
    Logger::logInfo("cc: choose traffic light config");
    Logger::logInfo("tlon : enable trafic light mode");
    Logger::logInfo("tloff : disable all lights");
    Logger::logInfo("tlyb : enable only yellow light in blinking mode");
    Logger::logInfo("tlt : test trafic light - lights should blink one by one for a short period of time");
    Logger::logInfo("exit : exit program");
    Logger::logInfo("!!! NOTE: all commands case sensitive !!!");
}

void exit() {
    if (current_controller) {
        if (working_thread.joinable()) {
            *stop_thread = true;
            working_thread.join();
        }
        (*current_controller).trafic_light_off();
    }
    for(auto & pair : traffic_light_controllers) {
        delete pair.second;
    }
    gpiod_chip_close(chip);
    Logger::logInfo("\n-------------------------Goodbye!------------------------");
}

void stop_working_thread() {
    Logger::logDebug(debug_mode, "stop_working_thread START");
    Logger::logDebug(debug_mode, "is_thread_running=" + to_string(is_thread_running->load()));
    Logger::logDebug(debug_mode, "stop_thread="  + to_string(stop_thread->load()));

    if (*is_thread_running || working_thread.joinable()) {
        *stop_thread = true;
        working_thread.join();
    }

    Logger::logDebug(debug_mode, "stop_working_thread END");
}

void read_configs() {
    try {
        pt::ptree tree;
        pt::read_ini(config_file_path, tree);
        version = tree.get<string>("version");
        debug_mode = tree.get<bool>("debug_mode");
        traffic_light_config_file_path = tree.get<string>(traffic_light_config_name);
    } catch (const std::exception& e) {
        Logger::logError("\nError reading base config file: ");
        Logger::logError( e.what());
    }

    TrafficLightConfigMapper *mapper = new TrafficLightConfigMapper(traffic_light_config_file_path);
    for(const auto& item : mapper->map_all_from_file()) {
        traffic_light_configs[item.first] = item.second;
    }
    delete mapper;
}

TrafficLightController* create_controller(string instance_name, gpiod_chip* chip) {
    if(traffic_light_configs.size() == 0) {
        Logger::logError("Configs not found try reload configs!!!");
    }
    for (auto& pair : traffic_light_configs) {
        if (pair.first == instance_name) {
            TrafficLightController *controller = new TrafficLightController(&(pair.second), debug_mode, chip, is_thread_running, stop_thread);
            traffic_light_controllers[instance_name] = controller;
            return controller;
        }
    }
    Logger::logError("Error: Traffic light config not found for instance name: " + instance_name);
    return nullptr;
}

int main() {
    cout << "Initialization"<< std::flush;
    is_thread_running = new std::atomic<bool>(false);
    stop_thread = new std::atomic<bool>(false);
    for (int i = 0; i < 5; i++) {
        cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    cout << "\nRead configs"<< std::flush;
    for (int i = 0; i < 5; i++) {
        cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    read_configs();

    Logger::logInfo("\n" );
    Logger::logInfo("\n################### Welcome to Trafic Light Simulator! ###################\n################### Version: " + version + " ###################\n");


    
    // Open GPIO chip
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        Logger::logError("Error: Unable to open GPIO chip: ");
        Logger::logError(std::string("/dev/gpiochip0"));
        return 1;
    }

    help();

    string command;
    string parameter;

    do {
        command = "";
        Logger::logInfo("\nCommand: ");
        cin >> command;
        Logger::logInfo("");

        if (command == "help") {
            help();
        } else if (command == "rc") {
            read_configs();
        } else if (command == "cc") {
            parameter = "";
            Logger::logInfo("\nEnter config name: ");
            cin >> parameter;
            Logger::logInfo("");
            if(parameter == "" || parameter == " ") {
                Logger::logError("Name is empty");
            }
            if(parameter != current_traffic_light) {
                current_traffic_light = parameter;
                current_controller = create_controller(current_traffic_light, chip);
                if(current_controller == nullptr) {
                    Logger::logError("No traffic light selected. Use 'cc' to select a traffic light.");
                    continue;
                }
                Logger::logInfo("New traffic light selected: " + current_traffic_light);
            } else {
                Logger::logInfo("Traffic light already selected: " + current_traffic_light);
            }
        } else if (command == "tlon") {
            if(current_controller == nullptr) {
                Logger::logError("No traffic light selected. Use 'cc' to select a traffic light.");
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_on, current_controller);
        } else if (command == "tloff") {
            if(current_controller == nullptr) {
                Logger::logError("No traffic light selected. Use 'cc' to select a traffic light.");
                continue;
            }
            stop_working_thread();
            (*current_controller).trafic_light_off();
        } else if (command == "tlyb") {
            if(current_controller == nullptr) {
                Logger::logError("No traffic light selected. Use 'cc' to select a traffic light.");
                continue;
            }
            stop_working_thread();
            working_thread = std::thread(&TrafficLightController::trafic_light_yellow_blink, current_controller);
        } else if (command == "tlt") {
            if(current_controller == nullptr) {
                Logger::logError("No traffic light selected. Use 'cc' to select a traffic light.");
                continue;
            }
            stop_working_thread();
            (*current_controller).trafic_light_test();
        } else if (command == "exit") {
            stop_working_thread();
            exit();
        } else {
            Logger::logError("Invalid command. \nEnter help to see valid commands.");
        }
    } while(command != "exit");

    return 0;
}