#include "mbed.h"

namespace drivers {
    namespace synth_hd
    {
        class Synth_HD
        {
            public:
            void SynthHD_config(RawSerial& device, int settings[]);
            void SynthHD_set_freq(RawSerial& device, int Ch, double Frequency);
            void SynthHD_set_pow(RawSerial& device, int Ch, double Power);
            void SynthHD_output(RawSerial& device, int Ch, int output);
            void SynthHD_chirp_control(RawSerial& device, int Ch, int output);
        };
    }
}