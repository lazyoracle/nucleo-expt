# A Nucleo based experiment control system for Atom Interferometry setups

This is a library for building experiment control systems used in Atom Interferometry experiments as an alternative to the usual LabVIEW + FPGA setup found in most Atomic Physics labs. It is built to run on the [STM32 NUCLEO F476ZG](https://os.mbed.com/platforms/ST-Nucleo-F746ZG/) and developed using the [ARM mbed-os](https://os.mbed.com/) library. 

## Overview

The codebase has four distinct parts -

* [`pin_settings.h`](pin_settings.h) - Lists the hardware connections. (*Check [`pin.csv`](pin.csv) or [`Nucleo Pin Map`](https://docs.google.com/spreadsheets/d/1NeUXG6S66NVjrvP1WDiC6XzqJkbjHOXEzv3fm1vFHmM/edit#gid=1854153454) for a tabulated list. Ensure that the pin map is updated and in sync with any changes made here.*)
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

## Timing & Configuration Settings

If there is one thing you need to take away about the experiment timing settings in this control software, it is the following:

> **_The settings list the time delay between the last operation for the current phase and the first operation for the next phase. Inside a particular phase of the experiment, they also list the relative delays between subsequent operations starting with a zero delay for the first operation._**

We explain this through the example sequence below:

![Sample Experiment Sequence](/img/sample_sequence.jpg "Sample Experiment Sequence")

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

We also note that the Sub Doppler phase has multiple RF sources ramping down. Various settings involved with configuring this ramp can be seen in the code snippet below from [`run_settings.h`](run_settings.h):

```c++
int AOM_ramp_Delay_SD = 0;

int AOM_0_Freq_start = 65000000;
int AOM_0_Freq_end = 90000000;
int AOM_1_Amp_start = 1023;
int AOM_1_Amp_end = 500;
int AOM_1_Freq_start = 100000000;
int AOM_1_Freq_end = 130000000;
int AOM_2_Amp_start = 1023;
int AOM_2_Amp_end = 500;
int AOM_3_Amp_start = 1023;
int AOM_3_Amp_end = 500;
int AOM_3_Freq_start = 220000000;
int AOM_3_Freq_end = 275000000;
int AOM_5_Amp_start = 1023;
int AOM_5_Amp_end = 1023;

int AOM_ramp_duration = 10000;
int AOM_ramp_time_step = 100;
```

The snippet above has three parts:

* `AOM_ramp_Delay_SD` - The delay in starting the ramp as measured from the start of the first operation in the Sub-Doppler (SD) Phase.
* `AOM_X_Freq_XXX` and `AOM_X_Amp_XXX` - The start and end frequency/amplitude of the ramp for the `AOM_X`
* `AOM_ramp_duration` and `AOM_ramp_time_step` - The duration of the ramp (in us) and the time step (in us). These two parameters are fixed for all the AOMs since the ramp updates are batched. Inside [`main.cpp`](main.cpp), the total number of ramp steps and the frequency/amplitude step is calculated as indicated below:

```c++
update_AD9959_freq(AD9959::Channel1, AOM_1_Freq_start);
dds.io_update();
wait_us(AOM_ramp_Delay_SD);
//Calculate ramp parameters
int total_ramp_steps = AOM_ramp_duration/AOM_ramp_time_step;
int AOM_1_Amp_ramp_step = (AOM_1_Amp_end - AOM_1_Amp_start)/total_ramp_steps;
int ramp_asf =  AOM_1_Amp_start;
//Start ramp. The AOM_ramp_time_step should not be smaller than ~100us
for(int i = 0; i < total_ramp_steps; i++)
{
    update_AD9959_amp(AD9959::Channel1, ramp_asf);
    dds.io_update();
    ramp_asf -= AOM_1_Amp_ramp_step;
    wait_us(AOM_ramp_time_step);
}
update_AD9959_settings(AD9959::Channel1, AOM_1_Freq_end, AOM_1_Amp_end);
dds.io_update();
```

## Integrating Daughter Boards

The expected workflow for integrating new daughter boards, eg, the AD9910 RF Synthesizer, is as follows:

* Independently develop the daughter board driver using mbed-os and test that it meets all the noise, timing and delay requirements. The AD9959 driver was developed in this fashion before integrating it.
* Include a header file and a namespace inside the driver as seen in [`ad9959.h`](/drivers/ad9959/ad9959.h). This ensures clean and readable code, eg, this update command for Channel 0 in AD9959 `update_AD9959_freq(AD9959::Channel0, AOM_0_Freq_Init);`
* Place the daughter board driver inside `/drivers` and include the header file and namespace.
* Update the corresponding pin and run configuration settings in the respective header files. Make sure the pins are properly initialised in the `main.cpp`

## Build Instructions

The code base is built and tested using the [Mbed Studio](https://os.mbed.com/studio/) development platform. It uses the `ARMC6` compiler with `mbed-os 5.15.0`. The latest `mbed-os` release is 6.0 and the code has **NOT** been tested with it. In order to compile this code or expand on it, clone the github repo and import it as a program in Mbed Studio. Set the target to `NUCLEO-F746ZG`. That should automatically download the necessary libraries and also the board configuration files. The other important file for the compiling is the [`mbed_app.json`](mbed_app.json) file which stores details about the build configuration. This file is also included in this repo but the minimum config is listed here below. This ensures only the essential libraries are included and RTOS is excluded:

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
- [x] Integrate SynthHD serial comm
- [ ] Complete main.cpp for all operation phases
- [x] Create Demo version of settings
- [x] Create function and variable descriptions
- [x] Instructions for extending & integrating modules
- [ ] UI for updating settings files with warnings, visuals
- [ ] Setup code compile and build using mbed-cli
- [ ] GUI for user end controlling
