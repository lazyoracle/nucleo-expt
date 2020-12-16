/*
Connections:
PA_4, CS
D10,   reset
D8,   update
D9,   sdio_3
GND,  pwr_dwn
PB_3, sclk (SPI CLK)
PB_5, sdio_0 (MOSI)

To-do
[Done] Hard-code 500MHz as reference clock -> set to 500e6, improved signal noise
[Done] Hard-code clk_mult to disabled -> set to 1, improved signal noise
[Done] Test write_register() on ftw and acr
[Done] Add reset_comm() function -> Pulse SDIO_3
[Done] Test reset_comm() to see if it allows multiple commands -> Can have frequency update inside while()
[Done] Add reset_comm to every command update
[Done] Add set_amp() function -> Direct ACR value write
[Done] Convert amplitude scaling to ACR value
[Done] Debug disappearing output -> Loop SDIO_3
[Done] Changing frequencies in a sequence -> Used wait_us() and functions
[Done] Debug compile and flash issues -> Press reset button
[Done] Test amp_ramp() function
[Done] Test freq_ramp() function -> used wait_us and got upto 30 us resolution, but shows some jitter
[Done] Change frequency/amplitude on trigger/interrupt
Figure out missing updates over SPI
Extend set_fr1 to include all bits
Implement set_fr2 and set_cfr
Set FR values at init to ensure no modulation and default settings
 */

#include "drivers/ad9959/ad9959.h"
#include "drivers/ad9910/AD9910.h"

/*Ad9959 specific vars begin */
using drivers::ad9959::AD9959;
DigitalOut start(LED2);
DigitalOut stop(LED3);

//state variables for fsm

InterruptIn trigger_AOM1(D2);
InterruptIn switch_AOM1(D3);

// NOTE: D1 (PG_14) and D0 (PG_9) cannot be used - as they are engaged in the communication line to the nucleo

constexpr drivers::ad9959::AD9959::Pins dds_pins = {
    PA_4, /* CS */
    D10,   /* reset */
    D8,   /* update */
    D9,   /* sdio_3 */
    PE_9, /* p0 */
    PE_11 /* p1 */
};

int state_AOM1;

int MOT_time = 10000; // Cooling time before sub-Doppler in us

SPI dds_spi(PB_5, PB_4, PB_3);
AD9959 dds(dds_spi, dds_pins, 500e6, 1);
/*AD9959 specific vars END */

/*AD9910 specific vars BEGIN */
#define AD9910_MOSI  D11  /// SPI MOSI on Nucleo SDIO
#define AD9910_MISO  D12  /// SPI MOSI on Nucleo
#define AD9910_SCK  D13   /// SPI CLK on Nucleo
#define AD9910_SPI_FREQ 27000000 //27 MHz
#define AD9910_REF_CLK_FREQ 40000000  //DDS has 40MhZ XTAL

//pin map
AD9910_pins HFDDS_pins;
AD9910_pins HFDDS1_pins;

//Default frequency and amplitude settings for 1st AD9910
int M = 245;  // Set MHz here
int K = 500; //Set KHz here
int H = 0; // Set Hz here
int A = 2; // Attenuation factor (range.. ??) putting A= 10 reduces the amplitude by a factor of 3 
/*AD9910 specific vars END */
// DigitalOut trig(D2); // from timing

///Default frequency and amplitude settings for 2nd AD9910
int M1 = 255;  // Set MHz here
int K1 = 500; //Set KHz here
int H1 = 0; // Set Hz here
int At1 = 1; 

void fsm_AOM1()
{
    if(state_AOM1 == 0)
    {
        dds.reset_comm();
        wait_us(2);
        dds.set_freq(AD9959::Channel1, 65e6);
        dds.io_update();
        state_AOM1 = 1;
        start = 1;
        stop = 0;
    }
    else if(state_AOM1 == 1)
    {
        dds.reset_comm();
        wait_us(2);
        dds.set_freq(AD9959::Channel1, 20e6);
        dds.io_update();
        wait_us(5);
        dds.set_amp(AD9959::Channel1, 512);
        dds.io_update();
        state_AOM1 = 2;
        start = 0;
        stop =  1;
    }
    else if (state_AOM1 == 2) 
    {
        dds.reset_comm();
        wait_us(2);
        dds.set_freq(AD9959::Channel1, 82e6);
        dds.io_update();
        wait_us(2);
        dds.set_amp(AD9959::Channel1, 200);
        dds.io_update();
        state_AOM1 = 3;
        start = 1;
        stop = 1;
    }
    else if(state_AOM1 == 3)
    {  
        dds.reset_comm();
        wait_us(2);
        dds.set_freq(AD9959::Channel1, 80e6);
        dds.io_update();
        wait_us(2);
        dds.set_amp(AD9959::Channel1, 350);
        dds.io_update();
        state_AOM1 = 0;
        start = 0;
        stop = 0;
    }
    
}

void AOM1_switch_on()
{
    dds.reset_comm();
    dds.set_amp(AD9959::Channel1, 1023);
    dds.io_update();
}

void AOM1_switch_off()
{
    dds.reset_comm();
    dds.set_amp(AD9959::Channel1, 0);
    dds.io_update();
}

// main() runs in its own thread in the OS
int main()
{
    HFDDS_pins.DPH = D15;
    HFDDS_pins.DRO = D14;
    HFDDS_pins.IOUP = PB_1;
    HFDDS_pins.PF0 = PB_2;
    HFDDS_pins.PF1 = PC_2;
    HFDDS_pins.PF2 = D4;
    HFDDS_pins.RST = PF_4;
    HFDDS_pins.DRC = D7;
    HFDDS_pins.OSK = PB_6;
    HFDDS_pins.CS = D3;
    HFDDS_pins.PWR_DWN = D12; 

    HFDDS1_pins.DPH = PC_6;
    HFDDS1_pins.DRO = PB_15;
    HFDDS1_pins.IOUP = PE_10;
    HFDDS1_pins.PF0 = PB_12;
    HFDDS1_pins.PF1 = PA_15;
    HFDDS1_pins.PF2 = PC_7;
    HFDDS1_pins.RST = PA_0; //PB_4 doesn't work as a replacement for PF_4 - needs ADC port;  When RST is removed the signal is off
    HFDDS1_pins.DRC = PE_7;
    HFDDS1_pins.OSK = PD_13;
    HFDDS1_pins.CS = PE_15;
    HFDDS1_pins.PWR_DWN = PE_8; 

    //Initialize the SPI bus for AD9910 - 1st 
    SPI highRfSpi(AD9910_MOSI, AD9910_MISO, AD9910_SCK);
    highRfSpi.frequency(AD9910_SPI_FREQ);
 
    AD9910 HFDDS = AD9910(HFDDS_pins, highRfSpi);
 
    HFDDS.init(AD9910_REF_CLK_FREQ);
   //  HFDDS.setprofile(M*1000000L + K*1000L + H, A*-1, Single_Tone_Profile_0);
    //M = 250;
    HFDDS.setprofile(M*1000000L + K*1000L + H, A*-1, Single_Tone_Profile_1);
    HFDDS.activateprofile(Single_Tone_Profile_1);
    /*AD9910 Setup END */ // when we switch betn profiles it takes 300 ns and within profile 20-30 uS

//for AD9910_1 i.e. 2nd 9910
    AD9910 HFDDS1 = AD9910(HFDDS1_pins, highRfSpi); 

    HFDDS1.init(AD9910_REF_CLK_FREQ);
 //   HFDDS1.setprofile(M1*1000000L + K1*1000L + H1, At1*-1, Single_Tone_Profile_0);
    //M1 = 250;
    HFDDS1.setprofile(M1*1000000L + K1*1000L + H1, At1*-1, Single_Tone_Profile_1);
    HFDDS1.activateprofile(Single_Tone_Profile_1); //setting profile_2 was wrong, worked by changing to _1
    /*AD9910 Setup END */
 

    /*AD9959 Setup BEGIN */
    double f0 = 80000000;
    double f1 = 110000000;
    double f2 = 80000000;
    float asf0 = 0.196;
    float asf1 = 0.220;
    float asf2 = 0.186;
    double asfd0 = round(asf0 * 1023);
    double asfd1 = round(asf1 * 1023);
    double asfd2 = round(asf2 * 1023);

    dds.reset();
    dds.init();
    dds.set_freq(AD9959::Channel0, f0);
    dds.set_amp(AD9959::Channel0, asfd0);
    dds.set_freq(AD9959::Channel1, f1);
    dds.set_amp(AD9959::Channel1, asfd1);
    dds.set_freq(AD9959::Channel2, f2);
    dds.set_amp(AD9959::Channel2, asfd2);
    dds.io_update();
 //   dds.io_update();
 //   dds.set_freq(AD9959::Channel3, f3);
 //   dds.set_amp(AD9959::Channel3, asfd3);
 //   dds.io_update();
    dds.reset_comm();
    //Minimum pulse length for trigger seems to be 10 us for reliable operation
 //   trigger_AOM1.rise(&fsm_AOM1);
 //   switch_AOM1.rise(&AOM1_switch_on);
 //   switch_AOM1.fall(&AOM1_switch_off);
    
    /*AD9910 Setup END */

wait_us(MOT_time); 

 //Calculate ramp parameters

 int AD9910_1_ramp_duration = 10000000; //100 ms
 int AD9910_1_ramp_time_step =  20000; // 2ms
 float AD9910_1_Amp_start = (float)At1; // previously defined
 float AD9910_1_Amp_end = (float)At1*25.0f; // reduce by a factor e.g. 5 here
 int total_ramp_steps = AD9910_1_ramp_duration/AD9910_1_ramp_time_step; // no. of steps for ramp
 float AD9910_1_Amp_ramp_step = (AD9910_1_Amp_end - AD9910_1_Amp_start)/total_ramp_steps; // Amplitude (i.e attenuation) in steps of 

// set new parameters for AD9910_1
        //Start ramp. The AOM_ramp_time_step should not be smaller than ~100us
        for(int i = 0; i < total_ramp_steps; i++)
        {
            HFDDS1.setprofile(M1*1000000L + K1*1000L + H1,  ((float)At1+(float)i*AD9910_1_Amp_ramp_step)*-1.0f, Single_Tone_Profile_1);
            HFDDS1.activateprofile(Single_Tone_Profile_1);
            wait_us(AD9910_1_ramp_time_step);
        }


    while(true)
    {
      wait_us(1000000); 
    }
} 