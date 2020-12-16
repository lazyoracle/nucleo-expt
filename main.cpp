/*
 * The main.cpp brings together the settings files defined in the rest of the code base and actually executes the experimental sequence. 
 * The while() in the main() runs through the sequence of different phases as defined in the experiment control.
 * The #ifdef blocks check for whether the code is running in sweep mode or normal mode and accordingly update the time sequences.
*/
#include "mbed.h"

//include drivers for daughter boards
#include "drivers/ad9959/ad9959.h"
using drivers::ad9959::AD9959;

//include settings
#include "pin_settings.h"
#include "run_settings.h"
#include "sweep_settings.h"

int run_count = 0;

//Serial Comms


//Initialise pins
DigitalOut LCWP(Pin_LCWP);

DigitalOut RF_SW_1(Pin_RF_SW_1);
DigitalOut RF_SW_2(Pin_RF_SW_2);

// DigitalOut AD9959_Reset (Pin_AD9959_Reset ); //These pins are initialised further below
// DigitalOut AD9959_SDIO_3 (Pin_AD9959_SDIO_3 ); //These pins are initialised further below
// DigitalOut AD9959_IO_Update(Pin_AD9959_IO_Update); //These pins are initialised further below
// DigitalOut AD9959_SDIO_0(Pin_AD9959_SDIO_0); //These pins are initialised further below
// DigitalOut AD9959_SCLK(Pin_AD9959_SCLK); //These pins are initialised further below
// DigitalOut AD9959_CSB(Pin_AD9959_CSB); //These pins are initialised further below
// DigitalOut AD9959_MISO(Pin_AD9959_MISO); //These pins are initialised further below

// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);
// DigitalOut AD9910_X(Pin_AD9910_X);

DigitalOut RBDP (Pin_RBDP );

DigitalOut CAM_1(Pin_CAM_1);
DigitalOut CAM_2(Pin_CAM_2);

DigitalOut SynthHD_1 (Pin_SynthHD_1 );
DigitalOut SynthHD_2(Pin_SynthHD_2 );

DigitalOut ANT_1 (Pin_ANT_1 );
DigitalOut ANT_2 (Pin_ANT_2 );

DigitalOut SHIM_1 (Pin_SHIM_1 );
DigitalOut SHIM_2 (Pin_SHIM_2 );
DigitalOut SHIM_3 (Pin_SHIM_3 );
DigitalOut MOTC(Pin_MOTC);

DigitalOut SHTR_8(Pin_SHTR_8);
DigitalOut SHTR_7(Pin_SHTR_7);
DigitalOut SHTR_1 (Pin_SHTR_1 );
DigitalOut SHTR_2 (Pin_SHTR_2 );
DigitalOut SHTR_3 (Pin_SHTR_3 );
DigitalOut SHTR_4 (Pin_SHTR_4 );
DigitalOut SHTR_5 (Pin_SHTR_5 );
DigitalOut SHTR_6 (Pin_SHTR_6 );


//Initialise AD9959
constexpr drivers::ad9959::AD9959::Pins dds_pins = {
    Pin_AD9959_CSB, /* CS */
    Pin_AD9959_Reset,   /* reset */
    Pin_AD9959_IO_Update,   /* update */
    Pin_AD9959_SDIO_3,   /* sdio_3 */
    PD_13, /* p0 on a spare pin from Nucleo */ 
    PD_12 /* p1 on a spare pin from Nucleo */
};
SPI dds_spi(Pin_AD9959_SDIO_0, Pin_AD9959_MISO, Pin_AD9959_SCLK);
AD9959 dds(dds_spi, dds_pins, 500e6, 1);


//Initialise Pulse Length for Trigger
void pulse(DigitalOut Pin)
{
    Pin = 1;
    wait_us(PULSE_length);
    Pin = 0;
}

//Control functions for AD9959. Must be followed by dds.io_update()
//These writes typically take ~20us, so account for this delay in the function call
void update_AD9959_freq(AD9959::Channel chan, int freq)
{
    //Update Frequency
    dds.reset_comm();
    dds.set_freq(chan, freq);
}

void update_AD9959_amp(AD9959::Channel chan, int ASF)
{
    //Update Amplitude
    dds.reset_comm();
    dds.set_amp(chan, ASF);
}

void update_AD9959_settings(AD9959::Channel chan, int freq, int ASF)
{
    //Update Amplitude and frequency
    dds.reset_comm();
    dds.set_freq(chan, freq);
    dds.set_amp(chan, ASF);
}


int main()
{
    while (true) 
    {
        
        #ifdef SWEEP
            //update params for sweep. In every run of the while, this picks a new value from the array
            //This code block executes only when the SWEEP defination is uncommented in sweep_settings.h
            // Param = Param_vals[run_count];
            AOM_0_Amp_Init = AOM_0_Amp_Init_vals[run_count];
            AOM_0_Freq_Init = AOM_0_Freq_Init_vals[run_count];
            AOM_1_Amp_Init = AOM_1_Amp_Init_vals[run_count];
            AOM_1_SW_Init = AOM_1_SW_Init_vals[run_count];
            AOM_1_Freq_Init = AOM_1_Freq_Init_vals[run_count];
            AOM_2_Amp_Init = AOM_2_Amp_Init_vals[run_count];
            AOM_2_SW_Init = AOM_2_SW_Init_vals[run_count];
            AOM_2_Freq_Init = AOM_2_Freq_Init_vals[run_count];
            EOM_1_Pow_Init = EOM_1_Pow_Init_vals[run_count];
            EOM_1_Freq_Init = EOM_1_Freq_Init_vals[run_count];
            ANT_1_Pow_Init = ANT_1_Pow_Init_vals[run_count];
            ANT_1_SW_Init = ANT_1_SW_Init_vals[run_count];
            ANT_1_Freq_Init = ANT_1_Freq_Init_vals[run_count];
            AOM_3_Pow_Init = AOM_3_Pow_Init_vals[run_count];
            AOM_3_SW_Init = AOM_3_SW_Init_vals[run_count];
            AOM_3_Freq_Init = AOM_3_Freq_Init_vals[run_count];
            AOM_4_Pow_Init = AOM_4_Pow_Init_vals[run_count];
            AOM_4_SW_Init = AOM_4_SW_Init_vals[run_count];
            AOM_4_Freq_Init = AOM_4_Freq_Init_vals[run_count];
            AOM_5_Pow_Init = AOM_5_Pow_Init_vals[run_count];
            AOM_5_SW_Init = AOM_5_SW_Init_vals[run_count];
            AOM_5_Freq_Init = AOM_5_Freq_Init_vals[run_count];
            EOM_2_Pow_Init = EOM_2_Pow_Init_vals[run_count];
            EOM_2_Freq_Init = EOM_2_Freq_Init_vals[run_count];
            ANT_2_Pow_Init = ANT_2_Pow_Init_vals[run_count];
            ANT_2_SW_Init = ANT_2_SW_Init_vals[run_count];
            ANT_2_Freq_Init = ANT_2_Freq_Init_vals[run_count];
            SHTR_1_Init = SHTR_1_Init_vals[run_count];
            SHTR_2_Init = SHTR_2_Init_vals[run_count];
            SHTR_3_Init = SHTR_3_Init_vals[run_count];
            SHTR_4_Init = SHTR_4_Init_vals[run_count];
            SHTR_5_Init = SHTR_5_Init_vals[run_count];
            SHTR_6_Init = SHTR_6_Init_vals[run_count];
            SHTR_7_Init = SHTR_7_Init_vals[run_count];
            SHTR_8_Init = SHTR_8_Init_vals[run_count];
            MOTC_Init = MOTC_Init_vals[run_count];
            INIT = INIT_vals[run_count];
            MOT = MOT_vals[run_count];
            AOM_ramp_Delay_SD = AOM_ramp_Delay_SD_vals[run_count];
            AOM_0_Freq_start = AOM_0_Freq_start_vals[run_count];
            AOM_0_Freq_end = AOM_0_Freq_end_vals[run_count];
            AOM_1_Amp_start = AOM_1_Amp_start_vals[run_count];
            AOM_1_Amp_end = AOM_1_Amp_end_vals[run_count];
            AOM_1_Freq_start = AOM_1_Freq_start_vals[run_count];
            AOM_1_Freq_end = AOM_1_Freq_end_vals[run_count];
            AOM_2_Amp_start = AOM_2_Amp_start_vals[run_count];
            AOM_2_Amp_end = AOM_2_Amp_end_vals[run_count];
            AOM_3_Amp_start = AOM_3_Amp_start_vals[run_count];
            AOM_3_Amp_end = AOM_3_Amp_end_vals[run_count];
            AOM_3_Freq_start = AOM_3_Freq_start_vals[run_count];
            AOM_3_Freq_end = AOM_3_Freq_end_vals[run_count];
            AOM_5_Amp_start = AOM_5_Amp_start_vals[run_count];
            AOM_5_Amp_end = AOM_5_Amp_end_vals[run_count];
            AOM_ramp_duration = AOM_ramp_duration_vals[run_count];
            AOM_ramp_time_step = AOM_ramp_time_step_vals[run_count];
            MOTC_Delay_SD = MOTC_Delay_SD_vals[run_count];
            MOTC_SD = MOTC_SD_vals[run_count];
            SHIM_Delay_SD = SHIM_Delay_SD_vals[run_count];
            SUB_DOPPLER = SUB_DOPPLER_vals[run_count];
            AOM_0_Freq_STATE_PREP = AOM_0_Freq_STATE_PREP_vals[run_count];
            AOM_1_Amp_STATE_PREP = AOM_1_Amp_STATE_PREP_vals[run_count];
            AOM_1_SW_STATE_PREP = AOM_1_SW_STATE_PREP_vals[run_count];
            AOM_1_Freq_STATE_PREP = AOM_1_Freq_STATE_PREP_vals[run_count];
            AOM_2_Delay_STATE_PREP = AOM_2_Delay_STATE_PREP_vals[run_count];
            AOM_2_Amp_STATE_PREP = AOM_2_Amp_STATE_PREP_vals[run_count];
            AOM_2_SW_STATE_PREP = AOM_2_SW_STATE_PREP_vals[run_count];
            ANT_1_Delay_STATE_PREP = ANT_1_Delay_STATE_PREP_vals[run_count];
            ANT_1_SW_STATE_PREP = ANT_1_SW_STATE_PREP_vals[run_count];
            ANT_1_Freq_Start_STATE_PREP = ANT_1_Freq_Start_STATE_PREP_vals[run_count];
            ANT_1_Freq_End_STATE_PREP = ANT_1_Freq_End_STATE_PREP_vals[run_count];
            AOM_3_Amp_STATE_PREP = AOM_3_Amp_STATE_PREP_vals[run_count];
            AOM_3_SW_STATE_PREP = AOM_3_SW_STATE_PREP_vals[run_count];
            AOM_3_Freq_STATE_PREP = AOM_3_Freq_STATE_PREP_vals[run_count];
            AOM_5_Amp_STATE_PREP = AOM_5_Amp_STATE_PREP_vals[run_count];
            AOM_5_SW_STATE_PREP = AOM_5_SW_STATE_PREP_vals[run_count];
            ANT_2_SW_STATE_PREP = ANT_2_SW_STATE_PREP_vals[run_count];
            ANT_2_Freq_Start_STATE_PREP = ANT_2_Freq_Start_STATE_PREP_vals[run_count];
            ANT_2_Freq_End_STATE_PREP = ANT_2_Freq_End_STATE_PREP_vals[run_count];
            SHTR_1_STATE_PREP = SHTR_1_STATE_PREP_vals[run_count];
            SHTR_2_STATE_PREP = SHTR_2_STATE_PREP_vals[run_count];
            SHTR_3_STATE_PREP = SHTR_3_STATE_PREP_vals[run_count];
            SHTR_4_STATE_PREP = SHTR_4_STATE_PREP_vals[run_count];
            SHTR_5_STATE_PREP = SHTR_5_STATE_PREP_vals[run_count];
            SHTR_6_STATE_PREP = SHTR_6_STATE_PREP_vals[run_count];
            SHTR_7_STATE_PREP = SHTR_7_STATE_PREP_vals[run_count];
            SHTR_8_STATE_PREP = SHTR_8_STATE_PREP_vals[run_count];
            STATE_PREP = STATE_PREP_vals[run_count];
            AOM_Delay_BLOW_AWAY = AOM_Delay_BLOW_AWAY_vals[run_count];
            AOM_SW_BLOW_AWAY = AOM_SW_BLOW_AWAY_vals[run_count];
            BLOW_AWAY = BLOW_AWAY_vals[run_count];
            AOM_0_Freq_RAMAN = AOM_0_Freq_RAMAN_vals[run_count];
            AOM_1_Amp_RAMAN = AOM_1_Amp_RAMAN_vals[run_count];
            AOM_1_Freq_RAMAN = AOM_1_Freq_RAMAN_vals[run_count];
            AOM_2_Amp_RAMAN = AOM_2_Amp_RAMAN_vals[run_count];
            AOM_SW_1_Delay_RAMAN = AOM_SW_1_Delay_RAMAN_vals[run_count];
            AOM_SW_1_RAMAN = AOM_SW_1_RAMAN_vals[run_count];
            AOM_SW_2_Delay_RAMAN = AOM_SW_2_Delay_RAMAN_vals[run_count];
            AOM_SW_2_RAMAN = AOM_SW_2_RAMAN_vals[run_count];
            AOM_SW_3_Delay_RAMAN = AOM_SW_3_Delay_RAMAN_vals[run_count];
            AOM_2_SW_3_RAMAN = AOM_2_SW_3_RAMAN_vals[run_count];
            EOM_1_Pow_RAMAN = EOM_1_Pow_RAMAN_vals[run_count];
            EOM_Chirp_Delay_RAMAN = EOM_Chirp_Delay_RAMAN_vals[run_count];
            EOM_1_Freq_Start_RAMAN = EOM_1_Freq_Start_RAMAN_vals[run_count];
            EOM_1_Freq_End_RAMAN = EOM_1_Freq_End_RAMAN_vals[run_count];
            EOM_1_Chirp_Step_RAMAN = EOM_1_Chirp_Step_RAMAN_vals[run_count];
            AOM_3_Amp_RAMAN = AOM_3_Amp_RAMAN_vals[run_count];
            AOM_3_Freq_RAMAN = AOM_3_Freq_RAMAN_vals[run_count];
            AOM_5_Amp_RAMAN = AOM_5_Amp_RAMAN_vals[run_count];
            EOM_2_Pow_RAMAN = EOM_2_Pow_RAMAN_vals[run_count];
            EOM_2_Freq_Start_RAMAN = EOM_2_Freq_Start_RAMAN_vals[run_count];
            EOM_2_Freq_End_RAMAN = EOM_2_Freq_End_RAMAN_vals[run_count];
            EOM_2_Chirp_Step_RAMAN = EOM_2_Chirp_Step_RAMAN_vals[run_count];
            SHTR_1_RAMAN = SHTR_1_RAMAN_vals[run_count];
            SHTR_2_RAMAN = SHTR_2_RAMAN_vals[run_count];
            SHTR_3_RAMAN = SHTR_3_RAMAN_vals[run_count];
            SHTR_4_RAMAN = SHTR_4_RAMAN_vals[run_count];
            SHTR_5_RAMAN = SHTR_5_RAMAN_vals[run_count];
            SHTR_6_RAMAN = SHTR_6_RAMAN_vals[run_count];
            SHTR_7_RAMAN = SHTR_7_RAMAN_vals[run_count];
            SHTR_8_RAMAN = SHTR_8_RAMAN_vals[run_count];
            RAMAN = RAMAN_vals[run_count];
            AOM_Delay_DET = AOM_Delay_DET_vals[run_count];
            AOM_SW_DET = AOM_SW_DET_vals[run_count];
            CAM_Delay_DET = CAM_Delay_DET_vals[run_count];
            DET = DET_vals[run_count];
            END = END_vals[run_count];
        #endif

        //Init Phase
        update_AD9959_freq(AD9959::Channel0, AOM_0_Freq_Init);
        update_AD9959_amp(AD9959::Channel0, AOM_0_Amp_Init);
        update_AD9959_freq(AD9959::Channel1, AOM_1_Freq_Init);
        update_AD9959_freq(AD9959::Channel1, AOM_1_Amp_Init);
        dds.io_update(); //dds.io_update() has a lag of ~100ns, call this just when you need the output updated
        MOTC = 0;
        wait_us(INIT);
        

        //MOT phase
        wait_us(MOT);

        
        //Sub-Doppler Phase    
        update_AD9959_freq(AD9959::Channel1, AOM_1_Freq_end);
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
        wait_us(SUB_DOPPLER);

        
        //State Prep Phase
        update_AD9959_freq(AD9959::Channel0, AOM_0_Freq_STATE_PREP);
        update_AD9959_settings(AD9959::Channel1, AOM_1_Freq_STATE_PREP, 0);
        dds.io_update();
        update_AD9959_settings(AD9959::Channel1, AOM_1_Freq_STATE_PREP, AOM_1_Amp_STATE_PREP);
        MOTC = 1;
        wait_us(STATE_PREP);
        
        //Blow Away Phase
        wait_us(AOM_Delay_BLOW_AWAY);
        dds.io_update();
        update_AD9959_amp(AD9959::Channel1, 0);
        wait_us(AOM_1_SW_STATE_PREP);
        dds.io_update();
        wait_us(BLOW_AWAY);
        

        //Raman Phase
        update_AD9959_settings(AD9959::Channel1, AOM_1_Freq_RAMAN, AOM_1_Amp_RAMAN);
        MOTC = 0;
        wait_us(RAMAN);


        //Detection Phase
        wait_us(AOM_Delay_DET);
        dds.io_update();
        update_AD9959_amp(AD9959::Channel1, 0);
        wait_us(AOM_SW_DET);
        dds.io_update();
        wait_us(DET);


        //END Phase
        wait_us(END);
        
        //limit runs to end sweep. This code is not executed during non-sweep runs
        #ifdef SWEEP
            if(run_count == TOTAL_RUNS - 1) break;
        #endif

        run_count += 1;
    }
}

