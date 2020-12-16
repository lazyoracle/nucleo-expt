//Control Pins on the Nucleo F746ZG. Changes made here should also reflect in
//Google Drive Nucleo Pin Map
#define Pin_LCWP PC_6
#define Pin_RF_SW_1 PB_15
#define Pin_RF_SW_2 PB_13
// #define Pin_AD9959_Reset PB_12
// #define Pin_AD9959_SDIO_3 PA_15
// #define Pin_AD9959_IO_Update PC_7
// #define Pin_AD9959_SDIO_0 PB_5
// #define Pin_AD9959_SCLK PB_3
// #define Pin_AD9959_CSB PA_4
// #define Pin_AD9959_MISO PB_4
#define Pin_AD9959_Reset D10 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_SDIO_3 D9 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_IO_Update D8 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_SDIO_0 PB_5 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_SCLK PB_3 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_CSB PA_4 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
#define Pin_AD9959_MISO PB_4 //These are temp pins for demo. Revert to the above else WILL CAUSE PIN CONFLICT
// #define Pin_AD9910_X PB_8
// #define Pin_AD9910_X PB_9
// #define Pin_AD9910_X PA_5
// #define Pin_AD9910_X PA_6
// #define Pin_AD9910_X PA_7
// #define Pin_AD9910_X PD_14
// #define Pin_AD9910_X PD_15
// #define Pin_AD9910_X PF_12
#define Pin_CAM_1 PB_2
#define Pin_CAM_2 PD_13
#define Pin_SynthHD_1 PD_12
#define Pin_SynthHD_2 PD_11
#define Pin_ANT_1 PE_2
#define Pin_ANT_2 PA_0
#define Pin_MOTC PB_0
#define Pin_RBDP PE_0
// #define Pin_AD9910_X PF13
// #define Pin_AD9910_X PE9
// #define Pin_AD9910_X PE11
// #define Pin_AD9910_X PF14
// #define Pin_AD9910_X PE13
#define Pin_SHIM_1 PF_15
#define Pin_SHIM_2 PG_14
#define Pin_SHIM_3 PG_9
#define Pin_SHTR_8 PE_8
#define Pin_SHTR_7 PE_7
#define Pin_SHTR_1 PE_10
#define Pin_SHTR_2 PE_12
#define Pin_SHTR_3 PE_14
#define Pin_SHTR_4 PE_15
#define Pin_SHTR_5 PB_10
#define Pin_SHTR_6 PB_11


//Trigger Signal Parameters
const int PULSE_length = 50;