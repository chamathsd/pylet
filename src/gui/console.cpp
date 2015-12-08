/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*/

#include "console.h"
#include "src/python/buffer.h"

#include <iostream>

std::string parsePyFunc() {
    using namespace boost::python;

    PyImport_AppendInittab("emb", emb::PyInit_emb);
    Py_Initialize();
    PyImport_ImportModule("emb");

    // here comes the ***magic***
    std::string buffer;
    {
        // switch sys.stdout to custom handler
        emb::stdout_write_type write =
            [&buffer](std::string s) { buffer += s; };

        emb::set_stdout(write);
        PyRun_SimpleString("print(\'hello to buffer\')");
        PyRun_SimpleString("print(3.14)");
        PyRun_SimpleString("print(\'still talking to buffer\')");
        emb::reset_stdout();
    }
    Py_Finalize();

    std::clog << buffer << std::endl;

    return buffer;
}

Console::Console(QWidget *parent) : QPlainTextEdit(parent) {
    QString s = QString::fromStdString(parsePyFunc());
    this->textCursor().insertText(s);
}