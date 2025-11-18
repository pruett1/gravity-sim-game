#include "stellar_obj.h"

StellarObject::StellarObject(const double mass, const double density, const std::pair<double, double>& position, const std::pair<double, double>& velocity) {
    this->mass = mass;
    this->density = density;
    this->position = position;
    this->velocity = velocity;
}

StellarObject::~StellarObject() {
    //if needed
}

std::pair<double, double> StellarObject::getPosition() {
    return position;
}

void StellarObject::setPosition(std::pair<double, double> newPos) {
    position = newPos;
}

std::pair<double, double> StellarObject::getVelocity() {
    return velocity;
}

void StellarObject::setVelocity(std::pair<double, double> newVec) {
    velocity = newVec;
}

double StellarObject::getMass() {
    return mass;
}

double StellarObject::getRadius() {
    // mass is in kg/m^3 but all distances in this are in AU so need to convert
    double AU = 149597870700; // m/AU
    double M_S = 1.988416e30; // kg/M_S
    double radius = (density * std::pow(AU, 3) / M_S) / mass; // kg / m^3 -> M_S / AU^3 -> 1 / AU^3
    radius = 1/radius; // 1 / AU^3 -> AU^3
    radius = std::cbrt(radius); // AU^3 -> AU
    return radius;
}