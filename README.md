
# PULLUP Counter

## About

The main purpose of the project was to automatically count and save the amount of pullups I make. So I developed a small device that goes into the pullup bar saves every pullup, shows how many pullups were done that day and has an USB interface for accessing the data and changing parameters of the device.

## Hardware
<img src="pullup_counter_readme\schematic.svg">
For the power supply I decided that 2 AAA batteries would be the best choice, as they're easily available and have good volumetric energy density. Embedding the Li-Po battery was of the table, because the project is stationary and charging wouldn't be possible. I choose a STM32L051C8 microcontroller as the brain of the device, for its low power consumption, and low operating voltage, which is mandatory in battery operated systems like this. The MCU has a backup battery used for maintaining operation of RTC clock while main batteries are discharged or being replaced. For display I used 7 segment display as they're cheap, small and easy to drive. For a memory in which I would store my database i used a 512kB SPI Flash - P25Q40H. For the commication with the device I went with USB serial port, so I included USB port and USB-UART converter. I also included a capacitive touch pad and little sensor IC. For detecting pullups I used proximity sesor consisting of IR Led diode and photodiode.

## Software

### Embedded

Project was made in VSCode using [stm32-for-vscode](https://github.com/bmd-studio/stm32-for-vscode) extension. For this project I used [stm32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) for generating initialization. I've also used drivers for SPI flash memory from this [github repository](https://github.com/nimaltd/w25qxx).

The device must be woken up before every training session, which can be done either by physical btn or capacitive touch pad. After that if device haven't detected a pullup within specified time, it goes back to sleep to prolong battery life.

Every pullup is represented with following struct

```C

typedef struct _entry_struct
{
    uint16_t id_;

    uint8_t hour_;
    uint8_t minutes_;

    uint8_t month_;
    uint8_t date_;
    uint8_t weekday_;
    uint8_t year_;
} entry_t

```

Because each entry's size is 8B and my flash memory is 512kB the maximum amount of pullups that can be saved within the device is over 65 thousand

### USB User Interface

![User Interface](pullup_counter_readme/UserInterface0.png)

User interface allows the user for easy change of important parameters, debugging and downloading and  clearing data from device's flash memory.

### Data analysys

Python script that reads and interprets data acquired from the device.
![Plot](pullup_counter_readme/PullupDistributionDuringDay.png)
