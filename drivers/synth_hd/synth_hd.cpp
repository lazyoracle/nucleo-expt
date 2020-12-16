#include "synth_hd.h"
#include <string>
namespace drivers{
   namespace synth_hd{
        void Synth_HD::SynthHD_config(RawSerial& device, int *settings)
        {
            /*
            This configures the following settings for Ch0 (EOM) and Ch1 (ANT)
            Refer to SynthHD serial comms documentation to understand the commands below
            EOM is Channel 0 and Antenna is Channel 1
            EOM_1_Freq_Init
            ANT_1_Freq_Init
            EOM_1_Pow_Init
            ANT_1_Pow_Init
            EOM_1_Chirp_Frequency
            ANT_1_Chirp_Frequency
            EOM_1_Chirp_Deviation
            ANT_1_Chirp_Deviation
            EOM_1_Chirp_Type linear
            ANT_1_Chirp_Type linear
            EOM_1_Chirp_Samples 1
            ANT_1_Chirp_Samples 1
            RF Power High
            PA set to On
            Write all settings to EEPROM
            Enable Output
            */
            int Ch_0_Freq_Init = settings[0];
            int Ch_1_Freq_Init = settings[1];
            int Ch_0_Pow_Init = settings[2];
            int Ch_1_Pow_Init = settings[3];
            int Ch_0_Chirp_Frequency = settings[4];
            int Ch_1_Chirp_Frequency = settings[5];
            int Ch_0_Chirp_Deviation = (settings[7] - settings[6])/2.0;
            int Ch_1_Chirp_Deviation = (settings[9] - settings[8])/2.0;

            
            string ch0_temp = "C0<"+ to_string(Ch_0_Chirp_Frequency) + ">" + to_string(Ch_0_Chirp_Deviation) + ",1;1f" 
                                + to_string(Ch_0_Freq_Init) + "W" + to_string(Ch_0_Pow_Init) + "h1r1eE1";
            char const *ch0_char = ch0_temp.c_str();
            device.printf(ch0_char);

            string ch1_temp = "C1<"+ to_string(Ch_1_Chirp_Frequency) + ">" + to_string(Ch_1_Chirp_Deviation) + ",1;1f"
                                + to_string(Ch_0_Freq_Init) + "W" + to_string(Ch_0_Pow_Init) + "h1eE1";
            char const *ch1_char = ch1_temp.c_str();
            device.printf(ch1_char);
        }

        void Synth_HD::SynthHD_chirp_control(RawSerial& device, int Ch, int output)
        {
            string temp = "C" + to_string(Ch) + "/" + to_string(output);
            char const *temp_char = temp.c_str();
            device.printf(temp_char);
        }

        void Synth_HD::SynthHD_set_freq(RawSerial& device, int Ch, double Frequency)
        {
            string temp = "C" + to_string(Ch) + "f" + to_string(Frequency);
            char const *temp_char = temp.c_str();
            device.printf(temp_char);
        }

        void Synth_HD::SynthHD_set_pow(RawSerial& device, int Ch, double Power)
        {
            string temp = "C" + to_string(Ch) + "W" + to_string(Power);
            char const *temp_char = temp.c_str();
            device.printf(temp_char);
        }

        void Synth_HD::SynthHD_output(RawSerial& device, int Ch, int output)
        {
            string temp = "C" + to_string(Ch) + "E" + to_string(output);
            char const *temp_char = temp.c_str();
            device.printf(temp_char);
        }

    }
}