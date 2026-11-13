#ifndef AMPLIFIEDWAVE_HPP
#define AMPLIFIEDWAVE_HPP

#include "SignalWave.hpp"

class AmplifiedWave : public SignalWave {
public:
    AmplifiedWave();
    AmplifiedWave(int count, float initVal, float extraVal);
    AmplifiedWave(const AmplifiedWave& other);
    virtual ~AmplifiedWave();
    AmplifiedWave& operator=(const AmplifiedWave& other);

    virtual float peakEnergy() const override;
};

#endif
