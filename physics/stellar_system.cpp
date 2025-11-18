#include "stellar_system.h"

StellarSystem::StellarSystem(const std::vector<StellarObject>& objects, const float dt) {
    this->objects.reserve(objects.size());
    this->objects = objects;

    this->G = 39.477; // Gravitational constant in AU^3 / (M_S * yr^2)
    this->dt = dt; 
    this->t = 0;
    this->numObj = objects.size();
}

StellarSystem::StellarSystem(const float dt) {
    this->G = 39.477;
    this->dt = dt;
    this->t = 0;
    this->numObj = 0;
    this->objects = std::vector<StellarObject>();
}

StellarSystem::~StellarSystem() {
    objects.clear();
}

void StellarSystem::addObject(const StellarObject& newObject) {
    objects.push_back(newObject);
    numObj++;
}

std::vector<StellarObject>& StellarSystem::getObjects() {
    return objects;
}

float StellarSystem::getTime() {
    return t;
}

std::vector<std::pair<double, double>> StellarSystem::computeAcc(std::vector<std::pair<double, double>> pos) {
    std::vector<std::pair<double, double>> acc(numObj, std::make_pair(0,0));

    for (int i = 0; i < numObj; i++) {
        for (int j = 0; j < numObj; j++) {
            if (i == j) {
                continue;
            }

            double dx = pos[j].first - pos[i].first; //dx = x_j - x_i
            double dy = pos[j].second - pos[i].second; //dy = y_j - y_i
            double r = sqrt(dx*dx + dy*dy);
            
            double mass = objects[j].getMass();
            double inv_r3 = 1 / (r*r*r);

            acc[i].first += G * mass * dx * inv_r3;
            acc[i].second += G * mass * dy * inv_r3;
        }
    }

    return acc;
}

void StellarSystem::rk4Step() {
    std::vector<std::pair<double, double>> initP;
    std::vector<std::pair<double, double>> initV;
    std::vector<std::pair<double, double>> interP;
    std::vector<std::pair<double, double>> interV;

    std::vector<std::pair<double, double>> k1_p;
    std::vector<std::pair<double, double>> k1_v;

    std::vector<std::pair<double, double>> k2_p;
    std::vector<std::pair<double, double>> k2_v;

    std::vector<std::pair<double, double>> k3_p;
    std::vector<std::pair<double, double>> k3_v;

    std::vector<std::pair<double, double>> k4_p;
    std::vector<std::pair<double, double>> k4_v;

    initP.reserve(numObj);
    initV.reserve(numObj);
    interP.resize(numObj);
    interV.resize(numObj);

    k1_p.reserve(numObj);
    k1_v.reserve(numObj);

    k2_p.reserve(numObj);
    k2_v.reserve(numObj);

    k3_p.reserve(numObj);
    k3_v.reserve(numObj);

    k4_p.reserve(numObj);
    k4_v.reserve(numObj);

    // dp/dt = v
    // dv/dt = a(p)

    //calc k1
    for (int i = 0; i < numObj; i++) {
        initP.push_back(objects[i].getPosition());
        initV.push_back(objects[i].getVelocity());
    }

    k1_p = initV;
    k1_v = computeAcc(initP);

    //calc k2
    for (int i = 0; i < numObj; i++) {
        interP[i].first = initP[i].first + 0.5 * dt * k1_p[i].first;
        interP[i].second = initP[i].second + 0.5 * dt * k1_p[i].second;
        interV[i].first = initV[i].first + 0.5 * dt * k1_v[i].first;
        interV[i].second = initV[i].second + 0.5 * dt * k1_v[i].second;
    }

    k2_p = interV;
    k2_v = computeAcc(interP);

    //calc k3
    for (int i = 0; i < numObj; i++) {
        interP[i].first = initP[i].first + 0.5 * dt * k2_p[i].first;
        interP[i].second = initP[i].second + 0.5 * dt * k2_p[i].second;
        interV[i].first = initV[i].first + 0.5 * dt * k2_v[i].first;
        interV[i].second = initV[i].second + 0.5 * dt * k2_v[i].second;
    }

    k3_p = interV;
    k3_v = computeAcc(interP);

    //calc k4
    for (int i = 0; i < numObj; i++) {
        interP[i].first = initP[i].first + dt * k3_p[i].first;
        interP[i].second = initP[i].second + dt * k3_p[i].second;
        interV[i].first = initV[i].first + dt * k3_v[i].first;
        interV[i].second = initV[i].second + dt * k3_v[i].second;
    }

    k4_p = interV;
    k4_v = computeAcc(interP);

    // update stellar objects p and v
    for (int i = 0; i < numObj; i++) {
        double p_x = initP[i].first;
        double p_y = initP[i].second;
        double v_x = initV[i].first;
        double v_y = initV[i].second;

        double dp_x = dt * (1/6.) * (k1_p[i].first + 2 * k2_p[i].first + 2 * k3_p[i].first + k4_p[i].first);
        double dp_y = dt * (1/6.) * (k1_p[i].second + 2 * k2_p[i].second + 2 * k3_p[i].second + k4_p[i].second);
        double dv_x = dt * (1/6.) * (k1_v[i].first + 2 * k2_v[i].first + 2 * k3_v[i].first + k4_v[i].first);
        double dv_y = dt * (1/6.) * (k1_v[i].second + 2 * k2_v[i].second + 2 * k3_v[i].second + k4_v[i].second);

        objects[i].setPosition(std::make_pair(p_x + dp_x, p_y + dp_y));
        objects[i].setVelocity(std::make_pair(v_x + dv_x, v_y + dv_y));
    }

    t += dt;
}