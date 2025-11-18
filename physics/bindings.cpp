#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "stellar_obj.h"
#include "stellar_system.h"

namespace py = pybind11;

PYBIND11_MODULE(physics, m) {
    py::class_<StellarObject>(m, "StellarObject")
        .def(py::init<const double, const double, const std::pair<double, double>, const std::pair<double, double>>(),
            py::arg("mass"), py::arg("density"), py::arg("position"), py::arg("velocity"))
        .def("get_position", &StellarObject::getPosition)
        .def("get_velocity", &StellarObject::getVelocity)
        .def("get_radius", &StellarObject::getRadius);

    py::class_<StellarSystem>(m, "StellarSystem")
        .def(py::init<std::vector<const StellarObject>&>(), py::arg("objects"))
        .def("add_object", &StellarSystem::addObject, py::arg("new_object"))
        .def("rk4_step", &StellarSystem::rk4Step);
}