# Traffic Light Simulator

Traffic Light Simulator is a small C++ console application that controls one or more traffic light configurations through Linux GPIO using `libgpiod`.

The program reads its settings from `config.ini`, loads traffic light definitions from a JSON config file, and lets you switch between different traffic light instances at runtime through a simple command-line interface.

## What It Does

- Opens a GPIO chip device and requests output lines for red, yellow, and green lights.
- Loads traffic light timing and pin configuration from JSON.
- Lets you choose a traffic light profile by name.
- Runs the selected traffic light in a repeating cycle.
- Supports a yellow-blink mode and a light-by-light test mode.
- Provides logging with optional debug output.

## Main Features

- **Config-driven control**
  - Base application settings are read from `config.ini`.
  - Traffic light instances are read from `configs/traffic_light_config.json`.

- **Multiple traffic light profiles**
  - You can define more than one traffic light profile in the JSON file.
  - Each profile can have its own GPIO pins and timing values.

- **Runtime commands**
  - Select a configuration.
  - Turn the traffic light cycle on.
  - Turn all lights off.
  - Start a yellow blinking mode.
  - Run a short test sequence.

- **Debug logging**
  - Debug messages can be enabled from the base config file.

## Command-Line Interface

After startup, the application accepts these case-sensitive commands:

- `help` - print the available commands
- `rcnf` - reload configuration files
- `chcnf` - choose a traffic light configuration by name
- `trlon` - enable normal traffic light mode
- `trloff` - disable all lights
- `trlyb` - enable yellow blinking mode
- `trltest` - run a short traffic light test sequence
- `exit` - stop the program and release GPIO resources

## Configuration

### Base Config

The application reads the following values from `config.ini`:

- `version`
- `debug_mode`
- `traffic_light_config_file`

Example:

```ini
version=0.1.0-b01-SNAPSHOT
debug_mode=false
traffic_light_config_file=./configs/traffic_light_config.json
```

### Traffic Light Config

Traffic light instances are stored in `configs/traffic_light_config.json`.

Each instance defines:

- `id`
- `name`
- `red_pin`
- `yellow_pin`
- `green_pin`
- `red_time`
- `yellow_time`
- `green_time`
- `green_time_blinking`
- `yellow_blinking_period`

Example structure:

```json
{
  "TrafficLight_1": {
    "id": "0",
    "name": "TrafficLight_1",
    "red_pin": "17",
    "yellow_pin": "27",
    "green_pin": "22",
    "red_time": "7000",
    "yellow_time": "1000",
    "green_time": "13000",
    "green_time_blinking": "2000",
    "yellow_blinking_period": "400"
  }
}
```

## Default Behavior

If you use the provided sample config, the app will:

- open `/dev/gpiochip0`
- request GPIO lines for pins `17`, `27`, and `22`
- cycle the lights using the timing values from the selected profile

## Project Structure

- `main.cpp` - program entry point and command loop
- `lib/controller/TrafficLightController.*` - GPIO control logic
- `lib/config/TrafficLightConfig.*` - traffic light configuration model
- `lib/config/TrafficLightConfigMapper.*` - JSON/INI mapping logic
- `lib/logger/Logger.cpp` - simple console logger
- `config.ini` - base application configuration
- `configs/traffic_light_config.json` - traffic light profiles

## Notes

- The program is intended for Linux systems with `libgpiod` available.
- Commands are case-sensitive.
- The sample configuration uses Raspberry Pi style GPIO pins, but any supported GPIO setup can be used if the pin numbers match your hardware.

## Status

This project is functional as a traffic light simulator and GPIO controller, but it is still a work in progress in a few areas, such as config cleanup and memory management.
