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

#include "Random.h"

namespace bp = boost::python;


// Note that class docstrings for all of these are now added in galsim/random.py

namespace galsim {

    // Need this special CallBack version that inherits from bp::wrapper whenever
    // you are wrapping something that has virtual functions you want to call from
    // python and have them resolve correctly.
    class BaseDeviateCallBack : public BaseDeviate,
                                public bp::wrapper<BaseDeviate>
    {
    public:
        BaseDeviateCallBack(long lseed=0) : BaseDeviate(lseed) {}
        BaseDeviateCallBack(const BaseDeviate& rhs) : BaseDeviate(rhs) {}
        BaseDeviateCallBack(std::string& str) : BaseDeviate(str) {}
        ~BaseDeviateCallBack() {}

    protected:
        // This is the special magic needed so the virtual function calls back to the
        // function defined in the python layer.
        double generate1()
        {
            if (bp::override py_func = this->get_override("generate1"))
                return py_func();
            else
                return BaseDeviate::generate1();
        }
    };

    void Generate(BaseDeviate& rng, size_t N, size_t idata)
    {
        double* data = reinterpret_cast<double*>(idata);
        rng.generate(N, data);
    }

    void AddGenerate(BaseDeviate& rng, size_t N, size_t idata)
    {
        double* data = reinterpret_cast<double*>(idata);
        rng.addGenerate(N, data);
    }

    void GenerateFromVariance(GaussianDeviate& rng, size_t N, size_t idata)
    {
        double* data = reinterpret_cast<double*>(idata);
        rng.generateFromVariance(N, data);
    }

    void GenerateFromExpectation(PoissonDeviate& rng, size_t N, size_t idata)
    {
        double* data = reinterpret_cast<double*>(idata);
        rng.generateFromExpectation(N, data);
    }

    struct PyBaseDeviate {

        static BaseDeviateCallBack* construct(const bp::object& seed) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct BaseDeviate from given seed.");
            return new BaseDeviateCallBack(0);
        }

        static void wrap() {
            bp::class_<BaseDeviateCallBack>
                pyBaseDeviate("BaseDeviateImpl", "", bp::no_init);
            pyBaseDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long>())
                .def(bp::init<const BaseDeviate&>())
                .def(bp::init<std::string>())
                .def("seed", (void (BaseDeviate::*) (long) )&BaseDeviate::seed)
                .def("reset", (void (BaseDeviate::*) (long) )&BaseDeviate::reset)
                .def("reset", (void (BaseDeviate::*) (const BaseDeviate&) )&BaseDeviate::reset)
                .def("clearCache", &BaseDeviate::clearCache)
                .def("serialize", &BaseDeviate::serialize)
                .def("duplicate", &BaseDeviate::duplicate)
                .def("discard", &BaseDeviate::discard)
                .def("raw", &BaseDeviate::raw)
                .def("generate", &Generate)
                .def("add_generate", &AddGenerate)
                .def("__repr__", &BaseDeviate::repr)
                .def("__str__", &BaseDeviate::str)
                .enable_pickling()
                ;
        }

    };

    struct PyUniformDeviate {

        static UniformDeviate* construct(const bp::object& seed) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct UniformDeviate from given seed.");
            return new UniformDeviate(0);
        }

        static void wrap() {
            bp::class_<UniformDeviate, bp::bases<BaseDeviate> >
                pyUniformDeviate("UniformDeviateImpl", "", bp::no_init);
            pyUniformDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long>())
                .def(bp::init<const BaseDeviate&>())
                .def(bp::init<std::string>())
                .def("duplicate", &UniformDeviate::duplicate)
                .def("generate1", &UniformDeviate::generate1)
                .enable_pickling()
                ;
        }

    };

    struct PyGaussianDeviate {

        static GaussianDeviate* construct(const bp::object& seed, double mean, double sigma) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct GaussianDeviate from given seed.");
            return new GaussianDeviate(0, mean, sigma);
        }

        static void wrap() {
            bp::class_<GaussianDeviate, bp::bases<BaseDeviate> >
                pyGaussianDeviate("GaussianDeviateImpl", "", bp::no_init);
            pyGaussianDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, double, double>())
                .def(bp::init<const BaseDeviate&, double, double>())
                .def(bp::init<std::string, double, double>())
                .def("duplicate", &GaussianDeviate::duplicate)
                .def("generate1", &GaussianDeviate::generate1)
                .def("generate_from_variance", &GenerateFromVariance)
                .add_property("mean", &GaussianDeviate::getMean)
                .add_property("sigma", &GaussianDeviate::getSigma)
                .enable_pickling()
                ;
        }

    };

    struct PyBinomialDeviate {

        static BinomialDeviate* construct(const bp::object& seed, int N, double p) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct BinomialDeviate from given seed.");
            return new BinomialDeviate(0, N, p);
        }

        static void wrap() {
            bp::class_<BinomialDeviate, bp::bases<BaseDeviate> >
                pyBinomialDeviate("BinomialDeviateImpl", "", bp::no_init);
            pyBinomialDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, int, double>())
                .def(bp::init<const BaseDeviate&, int, double>())
                .def(bp::init<std::string, int, double>())
                .def("duplicate", &BinomialDeviate::duplicate)
                .def("generate1", &BinomialDeviate::generate1)
                .add_property("n", &BinomialDeviate::getN)
                .add_property("p", &BinomialDeviate::getP)
                .enable_pickling()
                ;
        }

    };

    struct PyPoissonDeviate {

        static PoissonDeviate* construct(const bp::object& seed, double mean) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct PoissonDeviate from given seed.");
            return new PoissonDeviate(0, mean);
        }

        static void wrap() {
            bp::class_<PoissonDeviate, bp::bases<BaseDeviate> >
                pyPoissonDeviate("PoissonDeviateImpl", "", bp::no_init);
            pyPoissonDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, double>())
                .def(bp::init<const BaseDeviate&, double>())
                .def(bp::init<std::string, double>())
                .def("duplicate", &PoissonDeviate::duplicate)
                .def("generate1", &PoissonDeviate::generate1)
                .def("generate_from_expectation", &GenerateFromExpectation)
                .add_property("mean", &PoissonDeviate::getMean)
                .enable_pickling()
                ;
        }

    };

    struct PyWeibullDeviate {

        static WeibullDeviate* construct(const bp::object& seed, double a, double b) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct WeibullDeviate from given seed.");
            return new WeibullDeviate(0, a, b);
        }

        static void wrap() {

            bp::class_<WeibullDeviate, bp::bases<BaseDeviate> >
                pyWeibullDeviate("WeibullDeviateImpl", "", bp::no_init);
            pyWeibullDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, double, double>())
                .def(bp::init<const BaseDeviate&, double, double>())
                .def(bp::init<std::string, double, double>())
                .def("duplicate", &WeibullDeviate::duplicate)
                .def("generate1", &WeibullDeviate::generate1)
                .add_property("a", &WeibullDeviate::getA)
                .add_property("b", &WeibullDeviate::getB)
                .enable_pickling()
                ;
        }

    };

    struct PyGammaDeviate {

        static GammaDeviate* construct(const bp::object& seed, double k, double theta) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct GammaDeviate from given seed.");
            return new GammaDeviate(0, k, theta);
        }

        static void wrap() {
            bp::class_<GammaDeviate, bp::bases<BaseDeviate> >
                pyGammaDeviate("GammaDeviateImpl", "", bp::no_init);
            pyGammaDeviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, double, double>())
                .def(bp::init<const BaseDeviate&, double, double>())
                .def(bp::init<std::string, double, double>())
                .def("duplicate", &GammaDeviate::duplicate)
                .def("generate1", &GammaDeviate::generate1)
                .add_property("k", &GammaDeviate::getK)
                .add_property("theta", &GammaDeviate::getTheta)
                .enable_pickling()
                ;
        }

    };

    struct PyChi2Deviate {

        static Chi2Deviate* construct(const bp::object& seed, double n) {
            if (seed.ptr() != Py_None)
                throw std::runtime_error("Cannot construct Chi2Deviate from given seed.");
            return new Chi2Deviate(0, n);
        }

        static void wrap() {
            bp::class_<Chi2Deviate, bp::bases<BaseDeviate> >
                pyChi2Deviate("Chi2DeviateImpl", "", bp::no_init);
            pyChi2Deviate
                .def("__init__", bp::make_constructor(&construct, bp::default_call_policies()))
                .def(bp::init<long, double>())
                .def(bp::init<const BaseDeviate&, double>())
                .def(bp::init<std::string, double>())
                .def("duplicate", &Chi2Deviate::duplicate)
                .def("generate1", &Chi2Deviate::generate1)
                .add_property("n", &Chi2Deviate::getN)
                .enable_pickling()
                ;
        }

    };


    void pyExportRandom() {
        PyBaseDeviate::wrap();
        PyUniformDeviate::wrap();
        PyGaussianDeviate::wrap();
        PyBinomialDeviate::wrap();
        PyPoissonDeviate::wrap();
        PyWeibullDeviate::wrap();
        PyGammaDeviate::wrap();
        PyChi2Deviate::wrap();
    }

} // namespace galsim
