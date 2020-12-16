//Time Steps in us, Frequency in Hz, Amplitude [0, 1023] and Power in dBm
//DO NOT TRY TO OPTIMISE THIS BY DECLARING AS const OR #define
//This must NOT be const to allow for a more decoupled codebase

//Init - Initialise all devices with a constant value for MOT phase, typically 1-5ms
int AOM_0_Amp_Init =  0;
int AOM_0_Freq_Init =  0;
int AOM_1_Amp_Init = 0;
int AOM_1_SW_Init =  0;
int AOM_1_Freq_Init =  0;
int AOM_2_Amp_Init = 0;
int AOM_2_SW_Init = 0;
int AOM_2_Freq_Init =  0;
int EOM_1_Pow_Init =  0;
int EOM_1_Freq_Init =  0;
int ANT_1_Pow_Init =  0;
int ANT_1_SW_Init =  0;
int ANT_1_Freq_Init = 0;
int AOM_3_Pow_Init =  0;
int AOM_3_SW_Init = 0;
int AOM_3_Freq_Init =  0;
int AOM_4_Pow_Init =  0;
int AOM_4_SW_Init = 0;
int AOM_4_Freq_Init =  0;
int AOM_5_Pow_Init =  0;
int AOM_5_SW_Init = 0;
int AOM_5_Freq_Init =  0;
int EOM_2_Pow_Init =  0;
int EOM_2_Freq_Init =  0;
int ANT_2_Pow_Init =  0;
int ANT_2_SW_Init =  0;
int ANT_2_Freq_Init = 0;
int SHTR_1_Init = 0;
int SHTR_2_Init = 0;
int SHTR_3_Init = 0;
int SHTR_4_Init = 0;
int SHTR_5_Init = 0;
int SHTR_6_Init = 0;
int SHTR_7_Init = 0;
int SHTR_8_Init = 0;
int MOTC_Init =  0;
int INIT = 5000;


//MOT - MOT phase when all settings remain constant, typically 0.5-1s
int MOT = 500000;

//Sub-Doppler phase typically 15-30ms long
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

int MOTC_Delay_SD = 0;
int MOTC_SD = 0;

int SHIM_Delay_SD = 0;

int SUB_DOPPLER = 15000;

//State-prep phase typically <5ms
int AOM_0_Freq_STATE_PREP = 0;
int AOM_1_Amp_STATE_PREP = 0;
int AOM_1_SW_STATE_PREP = 0;
int AOM_1_Freq_STATE_PREP = 0;
int AOM_2_Delay_STATE_PREP = 0;
int AOM_2_Amp_STATE_PREP = 0;
int AOM_2_SW_STATE_PREP = 0;
int ANT_1_Delay_STATE_PREP = 0;
int ANT_1_SW_STATE_PREP = 0;
int ANT_1_Freq_Start_STATE_PREP = 0;
int ANT_1_Freq_End_STATE_PREP = 0;
int ANT_1_Chirp_Freq_STATE_PREP = 0;
int AOM_3_Amp_STATE_PREP = 0;
int AOM_3_SW_STATE_PREP = 0;
int AOM_3_Freq_STATE_PREP = 0;
int AOM_5_Amp_STATE_PREP = 0;
int AOM_5_SW_STATE_PREP = 0;
int ANT_2_SW_STATE_PREP = 0;
int ANT_2_Freq_Start_STATE_PREP = 0;
int ANT_2_Freq_End_STATE_PREP = 0;
int ANT_2_Chirp_Freq_STATE_PREP = 0;
int SHTR_1_STATE_PREP = 0;
int SHTR_2_STATE_PREP = 0;
int SHTR_3_STATE_PREP = 0;
int SHTR_4_STATE_PREP = 0;
int SHTR_5_STATE_PREP = 0;
int SHTR_6_STATE_PREP = 0;
int SHTR_7_STATE_PREP = 0;
int SHTR_8_STATE_PREP = 0;
int STATE_PREP = 4000;

//Blow-Away Phase typically <5ms
int AOM_Delay_BLOW_AWAY = 0;
int AOM_SW_BLOW_AWAY = 0;
int BLOW_AWAY = 4000;

//Raman Phase typically ~100ms
int AOM_0_Freq_RAMAN = 0;
int AOM_1_Amp_RAMAN = 0;
int AOM_1_Freq_RAMAN = 0;
int AOM_2_Amp_RAMAN = 0;
int AOM_SW_1_Delay_RAMAN = 0;
int AOM_SW_1_RAMAN = 0;
int AOM_SW_2_Delay_RAMAN = 0;
int AOM_SW_2_RAMAN = 0;
int AOM_SW_3_Delay_RAMAN = 0;
int AOM_2_SW_3_RAMAN = 0;
int EOM_1_Pow_RAMAN = 0;
int EOM_Chirp_Delay_RAMAN = 0;
int EOM_1_Freq_Start_RAMAN = 0;
int EOM_1_Freq_End_RAMAN = 0;
int EOM_1_Chirp_Freq_RAMAN = 0;
int AOM_3_Amp_RAMAN = 0;
int AOM_3_Freq_RAMAN = 0;
int AOM_5_Amp_RAMAN = 0;
int EOM_2_Pow_RAMAN = 0;
int EOM_2_Freq_Start_RAMAN = 0;
int EOM_2_Freq_End_RAMAN = 0;
int EOM_2_Chirp_Freq_RAMAN = 0;
int SHTR_1_RAMAN = 0;
int SHTR_2_RAMAN = 0;
int SHTR_3_RAMAN = 0;
int SHTR_4_RAMAN = 0;
int SHTR_5_RAMAN = 0;
int SHTR_6_RAMAN = 0;
int SHTR_7_RAMAN = 0;
int SHTR_8_RAMAN = 0;

int RAMAN = 20000;

//Detection phase typically <15ms
int AOM_Delay_DET = 0;
int AOM_SW_DET = 0;
int CAM_Delay_DET = 0;

int DET = 5000;

//End phase - dead time
int END = 1000;