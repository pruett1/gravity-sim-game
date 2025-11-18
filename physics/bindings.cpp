#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "stellar_obj.h"
#include "stellar_system.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<StellarObject>);

PYBIND11_MODULE(physics, m) {
    py::bind_vector<std::vector<StellarObject>>(m, "StellarObjectList");

    py::class_<StellarObject>(m, "StellarObject")
        .def(py::init<const double, const double, const std::pair<double, double>, const std::pair<double, double>>(),
            py::arg("mass"), py::arg("density"), py::arg("position"), py::arg("velocity"))
        .def("get_position", &StellarObject::getPosition)
        .def("get_mass", &StellarObject::getMass)
        .def("get_radius", &StellarObject::getRadius);

    py::class_<StellarSystem>(m, "StellarSystem")
        .def(py::init<const std::vector<StellarObject>&, const float>(), py::arg("objects"), py::arg("dt"))
        .def(py::init<const float>(), py::arg("dt"))
        .def("add_object", &StellarSystem::addObject, py::arg("new_object"))
        .def("get_objects", &StellarSystem::getObjects, py::return_value_policy::reference_internal)
        .def("get_time", &StellarSystem::getTime)
        .def("rk4_step", &StellarSystem::rk4Step);
}