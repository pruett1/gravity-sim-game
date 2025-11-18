from setuptools import setup, Extension
from setuptools import find_packages
import pybind11

ext_modules = [
    Extension(
        "physics",
        sources = ["physics/stellar_obj.cpp", "physics/stellar_system.cpp", "physics/bindings.cpp"],
        include_dirs = [pybind11.get_include()],
        language = "c++",
        extra_compile_args=["-O3", "-std=c++20"]
    ),
]

setup(
    name = "physics",
    version = "0.1.0",
    ext_modules = ext_modules,
    packages = ["physics"],
    zip_safe = False,
)