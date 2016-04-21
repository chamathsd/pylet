#ifndef INTERPRETER_H
#define INTERPRETER_H

/*
* PythonInterpreter.h
*
*  Created on: Dec 18, 2010
*      Author: Christopher M. Bruns
*/

#include <boost/python.hpp>
#include <string>
#include <QObject>
#include <QEventLoop>
#include <QString>

class PythonInterpreter;

class PythonOutputRedirector
{
public:
    PythonOutputRedirector(PythonInterpreter *interpreter = NULL);
    void write(std::string const& str);
    void write_wide(std::wstring const& str);

private:
    PythonInterpreter *interpreter;
};

class PythonInputRedirector
{
public:
    PythonInputRedirector(PythonInterpreter *interpreter = NULL);
    std::string readline();

private:
    PythonInterpreter *interpreter;
};

class PythonInterpreter : public QObject
{
    Q_OBJECT

public:
    friend class PythonOutputRedirector;
    friend class PythinInputRedirector;

    PythonInterpreter();
    virtual ~PythonInterpreter();
    std::string readline();

Q_SIGNALS:
    void startReadline();
    void outputSent(QString msg);
    void commandComplete();
    void incompleteCommand(QString partialCmd);

public Q_SLOTS:
    // run python command
    void interpretLine(QString line);
    // accept input from sys.stdin.readline() command
    void finishReadline(QString line);
    void runScriptFile(QString fileName);

private:
    void onOutput(QString msg);

    QEventLoop readlineLoop;
    QString readlineString;

    boost::python::object main_module;
    boost::python::object main_namespace;
    PythonInputRedirector stdinRedirector;
    PythonOutputRedirector stdoutRedirector;
    PythonOutputRedirector stderrRedirector;
};

#endif // INTERPRETER_H
