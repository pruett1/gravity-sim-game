#include "stellar_obj.h"

StellarObject::StellarObject(const double mass, const double density, const std::pair<double, double>& position, const std::pair<double, double>& velocity, const std::vector<int> color) {
    this->mass = mass;
    this->density = density;
    this->position = position;
    this->velocity = velocity;
    
    if (color.size() > 3 ||
        color[0] < 0 || color[0] > 255 ||
        color[1] < 0 || color[1] > 255 ||
        color[2] < 0 || color[1] > 255) {
        
        std::cerr << "Invalid format for (R, G, B). Must be 3 elements between 0 and 255. Defaulting color to (255, 255, 255)" << std::endl;
        this->color = {255, 255, 255};
    }
    else {
        this->color = color;
    }

    // mass is in kg/m^3 but all distances in this are in AU so need to convert
    double AU = 149597870700; // m/AU
    double M_S = 1.988416e30; // kg/M_S
    double r = (density * std::pow(AU, 3) / M_S) / mass; // kg / m^3 -> M_S / AU^3 -> 1 / AU^3
    r = 1/r; // 1 / AU^3 -> AU^3
    r = std::cbrt(r); // AU^3 -> AU
    this->radius = r;
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
    
    return radius;
}

void StellarObject::setColor(std::vector<int> newColor) {
        if (newColor.size() > 3 ||
        newColor[0] < 0 || newColor[0] > 255 ||
        newColor[1] < 0 || newColor[1] > 255 ||
        newColor[2] < 0 || newColor[1] > 255) {
        
        std::cerr << "Invalid format for (R, G, B). Must be 3 elements between 0 and 255. Color will remain unchanged." << std::endl;
    }
    else {
        color = newColor;
    }
}

std::vector<int> StellarObject::getColor() {
    return color;
}