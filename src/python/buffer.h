/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* Special thanks to Mateusz Loskot for the custom buffer implementation.
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <boost/python.hpp>
#include "Python.h"

namespace emb {

    typedef std::function<void(std::string)> stdout_write_type;

    PyObject* Stdout_write(PyObject* self, PyObject* args);
    PyObject* Stdout_flush(PyObject* self, PyObject* args);

    PyMODINIT_FUNC PyInit_emb(void);

    void set_stdout(stdout_write_type write);
    void reset_stdout();

}

std::string parsePyFile(const std::string &filename);

#endif // BUFFER_H
