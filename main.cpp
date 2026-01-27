#include <gpiod.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

const unsigned int RED_PIN = 17;
const unsigned int YELLOW_PIN = 27;
const unsigned int GREEN_PIN = 22;

enum Command {
  HELP,
  TRON,
  TROF,
  TRYB,
  TRT,
  EXIT,
  DEF
};


void trafic_light_on(gpiod_line_request* request) {
    gpiod_line_request_set_value(request, RED_PIN, GPIOD_LINE_VALUE_INACTIVE);
}

void trafic_light_off(gpiod_line_request* request) {
    gpiod_line_request_release(request);
}

void trafic_light_test(gpiod_line_request* request) {
    cout << "Trafic light test. Light on for 0.5 seconds one by one 3 times" << std::endl;
    for (int i = 0; i < 10; i++) {
        cout << i << std::endl;
        gpiod_line_request_set_value(request, RED_PIN, GPIOD_LINE_VALUE_INACTIVE);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // sleep(1);
        gpiod_line_request_release(request);
    }
    // for(int i = 0; i < 3; i++){
    //     gpiod_line_set_value(line_red, 1);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     gpiod_line_set_value(line_red, 0);
    //     gpiod_line_set_value(line_yellow, 1);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     gpiod_line_set_value(line_yellow, 0);
    //     gpiod_line_set_value(line_green, 1);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     gpiod_line_set_value(line_green, 0);
    // }
}

void trafic_light_yellow_blink() {
    cout << "trafic_light_yellow_blink" << std::endl;
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

void exit(gpiod_line_request* request) {
    trafic_light_off(request);
    cout << "\n    Goodbye!" << std::endl;
}

int main() {
    
    // Open GPIO chip
    gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        cout << "Error: Unable to open GPIO chip: " << "/dev/gpiochip0" << std::endl;
        return 1;
    }
    
    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_ACTIVE);
    
    gpiod_line_config* cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg, &RED_PIN, 1, settings);
    
    gpiod_request_config* req = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req, "gpio-timer");
    
    gpiod_line_request* request_red = gpiod_chip_request_lines(chip, req, cfg);
    // gpiod_line_request* request_red {nullptr};
    if (!request_red) {
        perror("gpiod_chip_request_lines");
        gpiod_chip_close(chip);
        return 1;
    }

    cout << "\n################### Welcome to Trafic Light Simulator! ###################\n################### Version: v0.0.1 ###################\n" << std::endl;
    help();

    string input = "";

    Command command = EXIT;

    do {
        cout << "\nCommand: ";
        cin >> input;
        cout << "\n";

        //TODO: rework this part
        // Generated code by AI
        // Map input string to Command enum
        if (input == "help") {
            command = HELP;
        } else if (input == "trlon") {
            command = TRON;
        } else if (input == "trloff") {
            command = TROF;
        } else if (input == "trlyb") {
            command = TRYB;
        } else if (input == "trltest") {
            command = TRT;
        } else if (input == "exit") {
            command = EXIT;
        } else {
            command = DEF;
        }
        // Ugly part ends here

        switch(command) {
            case HELP :
              help();
              break;
            case EXIT :
              exit(request_red);
              break;
            case TRON :
              trafic_light_on(request_red);
              break;
            case TROF :
              trafic_light_off(request_red);
              break;
            case TRYB :
              trafic_light_yellow_blink();
              break;
             case TRT :
              trafic_light_test(request_red);
              break;
            default :
              cout << "\nError! Invalid command. Please enter one of them:"  << std::endl;
              help();
              return 1;
        }

    } while(EXIT != command);

    // gpiod_line_release(line_red);
    // gpiod_line_release(line_yellow);
    // gpiod_line_release(line_green);
    gpiod_chip_close(chip);
    return 0;
}