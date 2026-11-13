#include "AmplifiedWave.hpp"

AmplifiedWave::AmplifiedWave() : SignalWave() {}

AmplifiedWave::AmplifiedWave(int count, float initVal, float extraVal) : SignalWave(count, initVal, extraVal) {}

AmplifiedWave::AmplifiedWave(const AmplifiedWave& other) : SignalWave(other) {}

AmplifiedWave::~AmplifiedWave() {}

AmplifiedWave& AmplifiedWave::operator=(const AmplifiedWave& other) {
    if (this != &other) {
        SignalWave::operator=(other);
    }
    return *this;
}

// TODO 5: Override peakEnergy() (Maximum 8 lines of implementation)
float AmplifiedWave::peakEnergy() const {
    return static_cast<float>(0);
}
