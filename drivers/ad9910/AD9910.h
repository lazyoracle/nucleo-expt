#ifndef AD9910_H
#define AD9910_H

#include "mbed.h"
#include <SPI.h>

#define AD9910_FREQ_MAX  400000000UL    ///< Maximum Generated Frequency
#define AD9910_FREQ_MIN  100000000UL      ///< Minimum Generated Frequency

#define M_ADR 24
#define K_ADR 28
#define H_ADR 32
#define A_ADR 36
#define MOD_INDEX_ADR 40
#define MOD_FREQK_ADR 44
#define MOD_FREQH_ADR 48
#define MOD_AM_DEPTH_ADR 52
#define MOD_FM_DEVK_ADR 56
#define MOD_FM_DEVH_ADR 60
#define MAIN_SETTINGS_FLAG_ADR 100 // defualt settings 
// ADR 101 reserved for clock settings
#define MODULATION_SETTINGS_FLAG 102 // defualt settings


#define CFR1_addr 0x00 // CFR1 Control Function Register 1
#define SDIO_input_only 0x02

#define CFR2_addr 0x01 // CFR2 Control Function Register 1
#define Enable_amplitude_scale_from_single_tone_profiles 0x01
#define Sync_timing_validation_disable 0x20

#define CFR3_addr 0x02 // CFR3 Control Function Register 1
#define REFCLK_input_divider_ResetB 0x40
#define PLL_enable 0x01
#define BASE_DDS_CORE_CLOCK 1000000000.0
#define DDS_Core_Clock 1000000000.0

// [31:8] - Open, [7:0] number controls the full-scale output current of the main DAC
#define FSC_addr 0x03

// Single Tone Profiles
#define Single_Tone_Profile_0     		0x0E
#define Single_Tone_Profile_1     		0x0F
#define Single_Tone_Profile_2     		0x10
#define Single_Tone_Profile_3     		0x11
#define Single_Tone_Profile_4     		0x12
#define Single_Tone_Profile_5     		0x13
#define Single_Tone_Profile_6     		0x14
#define Single_Tone_Profile_7     		0x15


//VCO Range Bit Settings 
#define VCO0 0x00 // 400-460Mhz
#define VCO1 0x01 // 455-530Mhz
#define VCO2 0x02 // 530-615Mhz
#define VCO3 0x03 // 650-790Mhz
#define VCO4 0x04 // 760-875Mhz
#define VCO5 0x05 // 920-1030Mhz

// Charge Pump Current, Icp
#define Icp212uA 												0x00
#define Icp237uA 												0x08
#define Icp262uA 												0x10
#define Icp287uA 												0x18
#define Icp312uA 												0x20
#define Icp337uA 												0x28
#define Icp363uA 												0x30
#define Icp387uA 												0x38


static const unsigned char cos_values[]={22,22,22,21,21,20,20,19,18,17,16,15,13,12,11,10,9,7,6,5,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,5,6,7,9,10,11,12,13,15,16,17,18,19,20,20,21,21,22,22};
static const unsigned char sin_values[]={11,12,13,15,16,17,18,19,20,20,21,21,22,22,22,22,22,21,21,20,20,19,18,17,16,15,13,12,11,10,9,7,6,5,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,5,6,7,9,10,11};
static const unsigned char hifreq[]={22,19,11,3,0,3,10,16,18,14,8,2,0,2,5,8,9,6,3,1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,3,6,9,8,6,2,0,2,8,14,18,16,10,3,0,3,11,19};


 typedef struct AD9910_pins
 {
    PinName DPH = D1;
    PinName DRO = D1;
    PinName IOUP = D1;
    PinName PF0 = D1;
    PinName PF1 = D1;
    PinName PF2 = D1;
    PinName RST = D1;
    PinName DRC = D1;
    PinName OSK = D1;
    PinName CS = D1;
    PinName PWR_DWN = D1;
 } AD9910_pins;


class AD9910
{
  public:
   
    AD9910_pins pin_map;
    DigitalOut *DPH;
    DigitalOut *IOUP;
    DigitalOut *PF0;
    DigitalOut *PF1;
    DigitalOut *PF2;
    DigitalOut *RST;
    DigitalOut *DRC;
    DigitalOut *OSK;
    DigitalOut *CS;
    DigitalOut *PWR_DWN;
    DigitalIn *DRO;
    SPI &spi_bus;
    char strBuffer[9]={129, 165, 15, 255};
    char rxBuffer[1];
    unsigned long clock_frequency;

    /*!
       Constructor
       creates an object and sets the SPI parameters.
       see the Arduino SPI library for the parameter values.
       @param pin the SPI Slave Select Pin to use
       @param mode the SPI Mode (see SPI mode define values)
       @param speed the SPI Serial Speed (see SPI speed values)
       @param order the SPI bit order (see SPI bit order values)
    */
    AD9910(AD9910_pins pinMap, SPI &bus) ;
    /*!
       initialize and start the SPI interface. Call this once after instanciating
       the object
    */
    void init(unsigned long clk_freq) ;

    void setprofile(uint64_t freq, float amp, uint8_t Num_Prof) ;
    void activateprofile(uint8_t Num_Prof) ;

    void update_vals() ;

//    void DDS_Fout(uint64_t *F_OUT, int16_t Amplitude_dB, uint8_t Num_Prof);
    void DDS_Fout(uint64_t *F_OUT, float Amplitude_dB, uint8_t Num_Prof);

    void refresh_buffer();
};


#endif