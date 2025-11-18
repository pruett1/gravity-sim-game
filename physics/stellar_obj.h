#pragma once
#include <utility>

class StellarObject {
    public:
        StellarObject(const double mass, const double density, const std::pair<double, double>& position, const std::pair<double, double>& velocity);

        ~StellarObject();

        std::pair<double, double> getPosition();

        void setPosition(std::pair<double, double> newPos);
        
        std::pair<double, double> getVelocity();

        void setVelocity(std::pair<double, double> newVec);

        double getMass();
        
        double getRadius();

    private:
        double mass; // in M_S (solar mass)
        double density; // in kg/m^3

        std::pair<double, double> position; // {{pos_x, pos_y}
        std::pair<double, double> velocity; // {vel_x, vel_y}}
};