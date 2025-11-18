#pragma once
#include <vector>
#include <cmath>

#include "stellar_obj.h"

class StellarSystem {
    public:
        StellarSystem(const std::vector<StellarObject>& objects);

        ~StellarSystem();

        void rk4Step();

        void addObject(const StellarObject& newObject);

    private:
        std::vector<StellarObject> objects;
        int numObj;
        double G; // Gravitational constant in AU^3 / (M_S * yr^2)
        float dt; // In years, currently hard set

        std::vector<std::pair<double, double>> computeAcc(std::vector<std::pair<double, double>> pos);
};