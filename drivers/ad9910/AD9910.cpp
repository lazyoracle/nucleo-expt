#include "AD9910.h"


// AD9910 create instance
AD9910::AD9910(AD9910_pins pinMap, SPI &bus) : spi_bus{bus}, pin_map{pinMap}
{
    DPH = new DigitalOut(pin_map.DPH);
    DRO = new DigitalIn(pin_map.DRO);
    IOUP = new DigitalOut(pin_map.IOUP);
    PF0 = new DigitalOut(pin_map.PF0);
    PF1 = new DigitalOut(pin_map.PF1);
    PF2 = new DigitalOut(pin_map.PF2);
    RST = new DigitalOut(pin_map.RST);
    DRC = new DigitalOut(pin_map.DRC);
    OSK = new DigitalOut(pin_map.OSK);
    CS = new DigitalOut(pin_map.CS);
    PWR_DWN = new DigitalOut(pin_map.PWR_DWN);
}

void AD9910::init(unsigned long clk_freq)
{
    //Pin init
    RST->write(1);  // RESET = 1
    wait_us(1000);
    RST->write(0);  // RESET = 0
    IOUP->write(0); // IO_UPDATE = 0   
    CS->write(1);   // CS = 1
    OSK->write(0);   // OSK = 0
    PF0->write(0);
    PF1->write(0);
    PF2->write(0);
    PWR_DWN->write(0); //MISO pin is reused to control power down mode
    DRC->write(0);
    DPH->write(0);
    clock_frequency = clk_freq;

    refresh_buffer();
} ;

void AD9910::update_vals()
{
    IOUP->write(1);
    wait_us(1);
    IOUP->write(0);
}

void  AD9910::setprofile(uint64_t freq, float amp, uint8_t Num_Prof)
{
  DDS_Fout(&freq, amp, Num_Prof);
} ;

void AD9910::activateprofile(uint8_t Num_Prof)
{
  if (Num_Prof == Single_Tone_Profile_0)
  {
    PF0->write(0);
    PF1->write(0);
    PF2->write(0);
  }
  else if (Num_Prof == Single_Tone_Profile_1)
  {
    PF0->write(1);
    PF1->write(0);
    PF2->write(0);
  }
}

void AD9910::DDS_Fout(uint64_t *F_OUT, float Amplitude_dB, uint8_t Num_Prof)
{
  uint32_t FTW;
  uint32_t *jlob;

  FTW = ((uint32_t)(4294967296.0 *((float)*F_OUT / (float)DDS_Core_Clock)));
  jlob = & FTW;	

  CS->write(0);
  strBuffer[0] = Num_Prof; // Single_Tone_Profile_0;

  //ASF  - Amplitude 14bit 0...16127
  strBuffer[1] =  (uint16_t)powf(10,(Amplitude_dB+84.288)/20.0) >> 8;     
  strBuffer[2] =  (uint16_t)powf(10,(Amplitude_dB+84.288)/20.0);         
  strBuffer[3] = 0x00;
  strBuffer[4] = 0x00;

  strBuffer[5] = *(((uint8_t*)jlob)+ 3);
  strBuffer[6] = *(((uint8_t*)jlob)+ 2);
  strBuffer[7] = *(((uint8_t*)jlob)+ 1);
  strBuffer[8] = *(((uint8_t*)jlob));

  spi_bus.write(strBuffer, 9, rxBuffer, 0);
  CS->write(1);
  update_vals();
};

void AD9910::refresh_buffer()
{
  //Basic Settings
   CS->write(0);
   strBuffer[0] = CFR1_addr;
   strBuffer[1] = 0;// RAM_enable;//RAM_Playback_Amplitude;// | RAM_enable;//0x00; 
   strBuffer[2] = 0;//Inverse_sinc_filter_enable;//0; //Continuous_Profile_0_1; //0;//0x80;//0x00;
   strBuffer[3] = 0; //OSK_enable | Select_auto_OSK;//0x00;
   strBuffer[4] = SDIO_input_only;
   spi_bus.write(strBuffer, 5, rxBuffer, 0);
   CS->write(1);
   update_vals();

   CS->write(0);
   strBuffer[0] = CFR2_addr;
   strBuffer[1] = Enable_amplitude_scale_from_single_tone_profiles; //1;//0x00;
   strBuffer[2] = 0; //SYNC_CLK_enable;// | Read_effective_FTW;
   strBuffer[3] = 0; //0x08;//PDCLK_enable;
   strBuffer[4] = Sync_timing_validation_disable; // | Parallel_data_port_enable;
   spi_bus.write(strBuffer, 5, rxBuffer, 0);
   CS->write(1);
   update_vals();

   //PLL settings
   CS->write(0);
   strBuffer[0] = CFR3_addr;
   strBuffer[1] = VCO5;  // | DRV0_REFCLK_OUT_High_output_current;
   strBuffer[2] = Icp287uA;   
   strBuffer[3] = REFCLK_input_divider_ResetB | PLL_enable; // REFCLK_input_divider_bypass; //
   strBuffer[4]=((uint32_t)DDS_Core_Clock/clock_frequency)*2;  // multiplier for PLL
   spi_bus.write(strBuffer, 5, rxBuffer, 0);
   CS->write(1);
   update_vals();

   
   CS->write(0);
   strBuffer[0] = FSC_addr;
   strBuffer[1] = 0;
   strBuffer[2] = 0;
   strBuffer[3] = 0;
//   if (DACCurrentIndex==0) strBuffer[4] = 0x7F; //DAC current Normal
//    else strBuffer[4] = 0xFF;// Max carrent 255 = 31mA //DAC current HI
   strBuffer[4] = 0x7F; //DAC current Normal
   spi_bus.write(strBuffer, 5, rxBuffer, 0);
   CS->write(1);
   update_vals();
/* No longer setting an output profiel in init. Calling it later with setf()
   CS->write(0);
   strBuffer[0] = 0x0E;
   strBuffer[1] = 0x3F;
   strBuffer[2] = 0xFF;
   strBuffer[3] = 0x00;
   strBuffer[4] = 0x00;
   
   strBuffer[5] = 0x19;
   strBuffer[6] = 0x99;
   strBuffer[7] = 0x99;
   strBuffer[8] = 0x9A; // 100mhz
   spi_bus.write(strBuffer, 9, rxBuffer, 0);
   CS->write(1);
   update_vals();
    */
};