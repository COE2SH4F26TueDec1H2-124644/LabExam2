#include "SignalWave.hpp"

SignalWave::SignalWave() : samples(nullptr), length(0), gain(2.5f) {}

SignalWave::SignalWave(int count, float initVal, float extraVal) : samples(nullptr), length(count), gain(extraVal) {
    if (length > 0) {
        samples = new float[length];
        for (int i = 0; i < length; ++i) {
            samples[i] = initVal + static_cast<float>(i);
        }
    } else {
        length = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
SignalWave::SignalWave(const SignalWave& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
SignalWave::~SignalWave() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
SignalWave& SignalWave::operator=(const SignalWave& other) {
    (void)other;
    return *this;
}

float SignalWave::getSample(int index) const {
    if (index >= 0 && index < length && samples != nullptr) {
        return samples[index];
    }
    return static_cast<float>(0);
}

// TODO 4: Implement peakEnergy() (Maximum 8 lines of implementation)
float SignalWave::peakEnergy() const {
    return static_cast<float>(0);
}
