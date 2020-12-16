# A Nucleo based experiment control system for Atom Interferometry setups

This is a library for building experiment control systems used in Atom Interferometry experiments as an alternative to the usual LabVIEW + FPGA setup found in most Atomic Physics lab. It is built to run on the [STM32 NUCLEO F476ZG](https://os.mbed.com/platforms/ST-Nucleo-F746ZG/) and developed using the [ARM mbed-os](https://os.mbed.com/) library. 


## Overview

The codebase has four distinct parts - 

* [`pin_settings.h`](pin_settings.h) - Lists the hardware connections
* [`run_settings.h`](run_settings.h) and [`sweep_settings.h`](sweep_settings.h) - Lists the experiment timings for different parts of the run
* [`main.cpp`](main.cpp) - Main code that runs the sequence of operations
* [`/drivers`](/drivers) - Stores the drivers for different daughter boards


The underlying philosophy for the structuring of the code-base is ease of extension, modification and scriptability.

* **Extension** - It should be easy to add extra control pins or sequence of operations or adapt to new functionality.
* **Modification** - Updating settings during experiment runs should be straightforward and decoupled from `main.cpp` source code
* **Scriptability** - We should be able to update the embedded system software from some front-end that doesn't involve directly interacting with the GUI-based IDE

## Operation Modes

There are two modes of operation for the experiment control:

* **Standard** - Normal running when the same settings keep repeating indefinitely
* **Sweep** - Sweep run for optimisations when various timing parameters are swept during a fixed number of runs

Changing between normal and sweep is very straightforward (keeping in line with the above philosophy). It effectively involves changing a single line of code in `sweep_settings.h` as shown below:

```c++
//No of trials
//Uncomment the macro below if you are turning on Sweeps
#define SWEEP 1
#define TOTAL_RUNS 5
```

The digital outputs from the NUCLEO are configured to operate in the following modes:

* **Trigger** - For controlling daughter boards that operate in an FSM mode
* **Switch** - Fast turning On and Off of devices
* **SPI** - SPI Data Bus (CLK, MISO, MOSI) for communicating with daughter boards
* **UART** - Serial 1-wire UART for communicating with daughter boards

In the `Trigger` mode, the pin produces a pulse of a preset length which essentially triggers the downstream controller (eg, the camera or the shim coils) to take the device to the next state in the FSM. The pulse length is defined in `pin_settings.h` as such: `const int PULSE_length = 50;` . 

In the `Switch` mode, the corresponding Digital Output Pin is either set to `High` or `Low` for a variable depending on the needs of that phase of the experiment.

## Experiment Timing Settings

If there is one thing you need to take away about the experiment timing settings in this control software, it is the following:

> **_The settings list the time delay between the last operation for the current phase and the first operation for the next phase. Inside a particular phase of the experiment, they also list the relative delays between subsequent operations starting with a zero delay for the first operation._**

We explain this through the example sequence below:

![Sample Experiment Sequence](/img/sample_sequence.jpg "Sample Experiment Sequence")

*Timing Diagram Courtesy: Ana Rakonjac*

We see the experiment control for multiple AOMs. In the State-Preparation phase, we note that there are 6 operations to be made. The timing settings for a configuration like this as listed in this excerpt from the [`run_settings.h`](run_settings.h) file would look as below:

```c++
//... Settings of previous phases
int AOM_0_Freq_STATE_PREP = 82000000;
int AOM_1_Amp_STATE_PREP = 250;
int AOM_1_SW_STATE_PREP = 0;
int AOM_1_Freq_STATE_PREP = 112000000;
int AOM_2_Delay_STATE_PREP = 1000; //corresponds to AOM_2 state change delay
int AOM_2_Amp_STATE_PREP = 650;
int AOM_2_SW_STATE_PREP = 0;
//...More settings for other devices
int AOM_3_Pow_STATE_PREP = 650;
int AOM_3_SW_STATE_PREP = 0;
int AOM_3_Freq_STATE_PREP = 252000000;
int STATE_PREP = 4000; //4ms to the next operation in Blow Away phase
//... More settings
```

## Build Instructions

The code base is built and tested using the [Mbed Studio](https://os.mbed.com/studio/) development platform. It uses the `ARMC6` compiler with `mbed-os 5.15.0`. In order to compile this code or expand on it, clone the github repo and import it as a program in Mbed Studio. Set the target to `NUCLEO-F746ZG`. That should automatically download the necessary libraries and also the board configuration files. The other important file for the compiling is the `mbed_app.json` file which stores details about the build configuration. This file is also included in this repo but the minimum config is listed here below. This ensures only the essential libraries are included and RTOS is excluded:

```json
{
    "requires": ["bare-metal"]
}
```

The mbed-os system can be fully built from the command line and as such, allows for scripting [*To be Implemented*]

## To-do

- [x] Include all Pin Settings
- [x] Include run timing settings
- [x] Include sweep timing settings
- [x] Integrate sweep timing settings
- [x] Include AD9959 driver code
- [x] Integrate AD9959 driver code
- [ ] Include DDS250 driver code
- [ ] Integrate DDS250 driver code
- [ ] Integrate SynthHD serial comm
- [ ] Include Serial controller for LCWP
- [x] Create Demo version of settings
- [ ] Create function and variable descriptions
- [ ] Scriptable UI
