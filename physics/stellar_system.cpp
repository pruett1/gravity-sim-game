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

void StellarSystem::reset() {
    t = 0;
    numObj = 0;
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

    resolveCollisions(detectCollisions());
}

std::vector<std::pair<int, int>> StellarSystem::detectCollisions() {
    std::vector<std::pair<int, int>> collisions;
    for (int i = 0; i < numObj; i++) {
        for (int j = i+1; j < numObj; j++) {
            double dx = objects[j].getPosition().first - objects[i].getPosition().first; //dx = x_j - x_i
            double dy = objects[j].getPosition().second - objects[i].getPosition().second; //dy = y_j - y_i
            double r = sqrt(dx*dx + dy*dy);

            if (r < (objects[i].getRadius() + objects[j].getRadius())) {
                collisions.emplace_back(std::make_pair(i, j));
            }
        }
    }

    return collisions;
}

struct DSU {
    std::vector<int> x;
    DSU(int n): x(n) { 
        for (int i = 0; i < n; i++) { 
            x[i] = i;
        }
    }
    int find (int a) {
        return x[a] == a ? a : x[a] = find(x[a]);
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            x[b] = a;
        }
    }
};

struct MergeData {
    double m;
    double px, py;
    double com_x_msum, com_y_msum;
    std::vector<int> color_sum;
    double vol_sum;
    std::vector<int> idxs;
};

void StellarSystem::resolveCollisions(std::vector<std::pair<int, int>> collisions) {
    if (collisions.empty()) { return; }

    // Build DSU
    DSU dsu(numObj);
    for (const auto pair: collisions) {
        int a = pair.first;
        int b = pair.second;
        dsu.unite(a,b);
    }

    // collect groups
    std::unordered_map<int, std::vector<int>> groups;
    for (int i = 0; i < numObj; i++) {
        int root = dsu.find(i);
        groups[root].push_back(i);
    }

    std::vector<MergeData> merges;
    merges.reserve(groups.size());

    // append merges into groups
    for (auto& kv: groups) {
        std::vector<int> idxs = kv.second;
        if (idxs.size() == 1) { continue; } // cant merge a single star

        MergeData md;
        md.m = 0.0;
        md.px = md.py = 0.0;
        md.com_x_msum = md.com_y_msum = 0.0;
        md.color_sum = {0, 0, 0};
        md.vol_sum = 0.0;
        md.idxs = idxs;

        for (int idx: idxs) {
            StellarObject& obj = objects[idx];
            double m = obj.getMass();
            md.m += m;
            md.px += m * obj.getVelocity().first;
            md.py += m * obj.getVelocity().second;
            md.com_x_msum += m * obj.getPosition().first;
            md.com_y_msum += m * obj.getPosition().second;
            md.vol_sum += (4./3.) * (3.14159) * std::pow(obj.getRadius(), 3);

            std::vector<int> color = obj.getColor();
            for (int i = 0; i < 3; i++) {
                md.color_sum[i] += m * color[i]; // weighted color avg
            }
        }

        merges.push_back(std::move(md));
    }

    if (merges.empty()) { return; }

    // remove all indicies involved in merges, going from highest to lowest
    std::vector<int> remove;
    for (auto &m: merges) {
        for (int idx: m.idxs) {
            remove.push_back(idx);
        }
    }

    std::sort(remove.begin(), remove.end());
    remove.erase(std::unique(remove.begin(), remove.end()), remove.end()); //remove all dups from idx list

    for (auto it = remove.rbegin(); it != remove.rend(); it++) {
        int idx = *it;
        objects.erase(objects.begin() + idx);
    }

    // create new obj from merges
    for (auto& m: merges) {
        double m_t = m.m;
        double vx = m.px / m_t;
        double vy = m.py / m_t;
        double com_x = m.com_x_msum / m_t;
        double com_y = m.com_y_msum / m_t;
        double rho = m_t / m.vol_sum; // this is M_s / AU^3 need kg/m^3
        rho = rho / std::pow(149597870700, 3) * (1.988416e30); //M_s / AU^3 / (m / AU)^3 * (kg / M_s) = kg/m^3

        std::vector<int> color(3);
        for (int i = 0; i < 3; i++){
            color[i] = int(m.color_sum[i] / m_t);
            color[i] = std::min(255, color[i]);
        }

        StellarObject merged(m_t, rho, std::make_pair(com_x, com_y), std::make_pair(vx, vy), color);
        std::cout << "New merged object has a mass of " << m_t << " density of rho " << rho << std::endl;
        objects.push_back(std::move(merged));
    }

    numObj = (int)objects.size();
}