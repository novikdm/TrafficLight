#include <iostream>
#include <gpiod.h>
#include <unistd.h>
using namespace std;

const char* TRAFIC_LIGHT_CHIP = "gpiochip0";
const unsigned int RED_PIN = 17;
const unsigned int YELLOW_PIN = 27;
const unsigned int GREEN_PIN = 22;

gpiod_chip* chip = nullptr;
gpiod_line* line_red = nullptr;
gpiod_line* line_yellow = nullptr;
gpiod_line* line_green = nullptr;

enum Command {
  HELP,
  TRON,
  TROF,
  TRYB,
  TRT,
  EXIT,
  DEF
};


void trafic_light_on(){
    gpiod_line_set_value(line_red, 1);
    gpiod_line_set_value(line_yellow, 1);
    gpiod_line_set_value(line_green, 1);
}

void trafic_light_off(){
    gpiod_line_set_value(line_red, 0);
    gpiod_line_set_value(line_yellow, 0);
    gpiod_line_set_value(line_green, 0);
}

void trafic_light_test(){
    cout << "Trafic light test. Light on for 0.5 seconds one by one 3 times" << std::endl;
    for(int i = 0; i < 3; i++){
        gpiod_line_set_value(line_red, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_set_value(line_red, 0);
        gpiod_line_set_value(line_yellow, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_set_value(line_yellow, 0);
        gpiod_line_set_value(line_green, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpiod_line_set_value(line_green, 0);
    }
}

void trafic_light_yellow_blink(){
    cout << "trafic_light_yellow_blink" << std::endl;
}

void help(){
    cout << "   Available commands:\n" << std::endl;
    cout << "help : print instructions" << std::endl;
    cout << "trlon : enable trafic light mode" << std::endl;
    cout << "trloff : disable all lights" << std::endl;
    cout << "trlyb : enable only yellow light in blinking mode" << std::endl;
    cout << "trltest : test trafic light - lights should blink one by one for a short period of time" << std::endl;
    cout << "exit : exit program" << std::endl;
    cout << "!!! NOTE: all commands case sensitive !!!" << std::endl;
}

void exit(){
    trafic_light_off();
    cout << "\n    Goodbye!" << std::endl;
}

int main() {

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        cout << "Error: Unable to open GPIO chip: " << TRAFIC_LIGHT_CHIP << std::endl;
        return 1;
    }

    // Get GPIO line
    line_red = gpiod_chip_get_line(chip, RED_PIN);
    line_yellow = gpiod_chip_get_line(chip, YELLOW_PIN);
    line_green = gpiod_chip_get_line(chip, GREEN_PIN);
    if (!line_red || !line_yellow || !line_green) {
        cout << "Error: Unable to get GPIO lines." << std::endl;
        cout << "Red line: " << (line_red ? "Success" : "Failure") << std::endl;
        cout << "Yellow line: " << (line_yellow ? "Success" : "Failure") << std::endl;
        cout << "Green line: " << (line_green ? "Success" : "Failure") << std::endl;
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
              exit();
              break;
            case TRON :
              trafic_light_on();
              break;
            case TROF :
              trafic_light_off();
              break;
            case TRYB :
              trafic_light_yellow_blink();
              break;
             case TRT :
              trafic_light_test();
              break;
            default :
              cout << "\nError! Invalid command. Please enter one of them:"  << std::endl;
              help();
              return 1;
        }

    } while(EXIT != command);

    gpiod_line_release(line_red);
    gpiod_line_release(line_yellow);
    gpiod_line_release(line_green);
    gpiod_chip_close(chip);
    return 0;
}