#include <iostream>
#include "SignalWave.hpp"
#include "AmplifiedWave.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    float initV = static_cast<float>(1.0f);
    float extV = static_cast<float>(2.5f);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        SignalWave pDef;
        if (approxEqual(static_cast<double>(pDef.peakEnergy()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getSample(0)), 0.0)) {
            cout << "[PASS] TC 01: SignalWave default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: SignalWave default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        SignalWave p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getSample(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getSample(2)), static_cast<double>(initV + static_cast<float>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getSample(1) == static_cast<float>(initV + static_cast<float>(1))) {
            // Compare against a second identical instance
            SignalWave pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.peakEnergy()), static_cast<double>(pCheck.peakEnergy())) &&
                !approxEqual(static_cast<double>(p1.peakEnergy()), 0.0)) {
                cout << "[PASS] TC 02: SignalWave parameterized constructor & peakEnergy()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: SignalWave peakEnergy() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: SignalWave element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        SignalWave orig(3, initV, extV);
        SignalWave copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.peakEnergy()), static_cast<double>(orig.peakEnergy())) &&
            approxEqual(static_cast<double>(copyObj.getSample(1)), static_cast<double>(orig.getSample(1))) &&
            !approxEqual(static_cast<double>(copyObj.peakEnergy()), 0.0)) {
            cout << "[PASS] TC 03: SignalWave copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: SignalWave copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        SignalWave a(3, initV, extV);
        SignalWave b(2, initV, extV);
        b = a;
        SignalWave* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.peakEnergy()), static_cast<double>(a.peakEnergy())) &&
            approxEqual(static_cast<double>(b.getSample(2)), static_cast<double>(a.getSample(2))) &&
            !approxEqual(static_cast<double>(b.peakEnergy()), 0.0)) {
            cout << "[PASS] TC 04: SignalWave copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: SignalWave copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            SignalWave temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: SignalWave destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: SignalWave destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        SignalWave baseObj(3, initV, extV);
        AmplifiedWave childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.peakEnergy()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.peakEnergy()), static_cast<double>(baseObj.peakEnergy()))) {
            cout << "[PASS] TC 06: AmplifiedWave overridden peakEnergy()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: AmplifiedWave overridden peakEnergy() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        AmplifiedWave directChild(3, initV, extV);
        SignalWave* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->peakEnergy()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->peakEnergy()), static_cast<double>(directChild.peakEnergy()))) {
            cout << "[PASS] TC 07: AmplifiedWave polymorphic dispatch via SignalWave*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: AmplifiedWave polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        AmplifiedWave c1(3, initV, extV);
        AmplifiedWave c2(c1);
        if (approxEqual(static_cast<double>(c1.peakEnergy()), static_cast<double>(c2.peakEnergy())) &&
            !approxEqual(static_cast<double>(c2.peakEnergy()), 0.0)) {
            cout << "[PASS] TC 08: AmplifiedWave copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: AmplifiedWave copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        AmplifiedWave c1(3, initV, extV);
        AmplifiedWave c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.peakEnergy()), static_cast<double>(c2.peakEnergy())) &&
            !approxEqual(static_cast<double>(c2.peakEnergy()), 0.0)) {
            cout << "[PASS] TC 09: AmplifiedWave copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: AmplifiedWave copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        SignalWave* heapChild = new AmplifiedWave(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: AmplifiedWave virtual destructor via SignalWave*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: AmplifiedWave virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
