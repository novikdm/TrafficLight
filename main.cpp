#include <iostream>
// #include <gpiod.h>
#include <unistd.h>
using namespace std;


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
    cout << "trafic_light_on" << std::endl;
}

void trafic_light_off(){
    cout << "trafic_light_off" << std::endl;
}

void trafic_light_test(){
    cout << "trafic_light_test" << std::endl;
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
    return 0;
}