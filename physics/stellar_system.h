#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "stellar_obj.h"

class StellarSystem {
    public:
        StellarSystem(const std::vector<StellarObject>& objects, const float dt); // init with some objects

        StellarSystem(const float dt); // init with only dt, no objects will be initalized

        ~StellarSystem();

        void rk4Step();

        void addObject(const StellarObject& newObject);

        void reset();

        std::vector<StellarObject>& getObjects();

        float getTime();

    private:
        std::vector<StellarObject> objects;
        int numObj;
        double G; // Gravitational constant in AU^3 / (M_S * yr^2)
        float dt; // In years
        float t; // Just for tracking total time passed

        std::vector<std::pair<double, double>> computeAcc(std::vector<std::pair<double, double>> pos);

        std::vector<std::pair<int, int>> detectCollisions();

        void resolveCollisions(std::vector<std::pair<int, int>>);
};