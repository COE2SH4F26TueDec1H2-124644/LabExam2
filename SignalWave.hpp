#ifndef SIGNALWAVE_HPP
#define SIGNALWAVE_HPP

#include <iostream>
using namespace std;

class SignalWave {
protected:
    float* samples;
    int length;
    float gain;

public:
    SignalWave();
    SignalWave(int count, float initVal, float extraVal);
    SignalWave(const SignalWave& other);
    virtual ~SignalWave();
    SignalWave& operator=(const SignalWave& other);

    float getSample(int index) const;
    virtual float peakEnergy() const;
};

#endif
