#pragma once
#include <vector>
#include <cmath>
#include <iostream>

#include "stellar_obj.h"

class StellarSystem {
    public:
        StellarSystem(const std::vector<StellarObject>& objects, const float dt);

        ~StellarSystem();

        void rk4Step();

        void addObject(const StellarObject& newObject);

        std::vector<StellarObject>& getObjects();

        float getTime();

    private:
        std::vector<StellarObject> objects;
        int numObj;
        double G; // Gravitational constant in AU^3 / (M_S * yr^2)
        float dt; // In years
        float t; // Just for tracking total time passed

        std::vector<std::pair<double, double>> computeAcc(std::vector<std::pair<double, double>> pos);
};