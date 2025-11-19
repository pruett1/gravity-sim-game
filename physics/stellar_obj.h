#pragma once
#include <utility>
#include <vector>
#include <iostream>

class StellarObject {
    public:
        StellarObject(const double mass, const double density, const std::pair<double, double>& position, 
            const std::pair<double, double>& velocity, const std::vector<int> color={255, 255, 255});

        ~StellarObject();

        std::pair<double, double> getPosition();

        void setPosition(std::pair<double, double> newPos);
        
        std::pair<double, double> getVelocity();

        void setVelocity(std::pair<double, double> newVec);

        void setColor(const std::vector<int> color);

        std::vector<int> getColor();

        double getMass();
        
        double getRadius();

    private:
        double mass; // in M_S (solar mass)
        double density; // in kg/m^3
        double radius;

        std::pair<double, double> position; // {{pos_x, pos_y}
        std::pair<double, double> velocity; // {vel_x, vel_y}}

        std::vector<int> color; //rgb color vals
};