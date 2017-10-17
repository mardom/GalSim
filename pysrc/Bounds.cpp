/* -*- c++ -*-
 * Copyright (c) 2012-2017 by the GalSim developers team on GitHub
 * https://github.com/GalSim-developers
 *
 * This file is part of GalSim: The modular galaxy image simulation toolkit.
 * https://github.com/GalSim-developers/GalSim
 *
 * GalSim is free software: redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions, and the disclaimer given in the accompanying LICENSE
 *    file.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the disclaimer given in the documentation
 *    and/or other materials provided with the distribution.
 */

#include "galsim/IgnoreWarnings.h"
#include "boost/python.hpp"

#include "Bounds.h"

namespace bp = boost::python;

namespace galsim {
namespace {

template <typename T>
struct PyPosition {

    static void wrap(std::string const & suffix) {

        bp::class_< Position<T> > pyPosition(("Position" + suffix).c_str(), bp::no_init);
        pyPosition
            .def(bp::init<T,T>())
            .def_readonly("x", &Position<T>::x)
            .def_readonly("y", &Position<T>::y);
    }

};

template <typename T>
struct PyBounds {

    static void wrap(std::string const & suffix) {
        bp::class_< Bounds<T> > pyBounds(("Bounds" + suffix).c_str(), bp::no_init);
        pyBounds
            .def(bp::init<T,T,T,T>())
            .add_property("xmin", &Bounds<T>::getXMin)
            .add_property("xmax", &Bounds<T>::getXMax)
            .add_property("ymin", &Bounds<T>::getYMin)
            .add_property("ymax", &Bounds<T>::getYMax);
    }

};

} // anonymous

void pyExportBounds() {
    PyPosition<double>::wrap("D");
    PyPosition<int>::wrap("I");
    PyBounds<double>::wrap("D");
    PyBounds<int>::wrap("I");
}

} // namespace galsim
