/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE for details.
*/

#include "console.h"
#include "src/python/buffer.h"
#include <qdebug.h>

Console::Console(InfoBox* infoBox, QWidget *parent) : QPlainTextEdit(parent) {
    setFont(monoFont);
    setWordWrapMode(QTextOption::WrapAnywhere);
    setTextInteractionFlags(Qt::TextSelectableByMouse);
    i = infoBox;
}

void Console::runFile(const QString &filename) {
    moveCursor(QTextCursor::End);
    textCursor().insertText(generateRestartString());
    std::string pyFile = filename.toStdString();
    QString readOut = QString::fromStdString(parsePyFile(pyFile));
    interpretError(readOut);
    textCursor().insertText(readOut);
    ensureCursorVisible();
}

void Console::parseString(const QString &inputString) {
    moveCursor(QTextCursor::End);
    std::string pyString = inputString.toStdString();
    QString readOut = QString::fromStdString(parsePyString(pyString));
    interpretError(readOut);
    textCursor().insertText(readOut);
    ensureCursorVisible();
}

void Console::throwError(const QString &message) {
    QString htmlString = "<font color = \"red\">[Pylet] " + message + "</font>";
    appendHtml(htmlString);
}

QString Console::generateRestartString() {
    int fontWidth = fontMetrics().width('=');
    QString spacer(((width() - 140) / fontWidth) / 2, '=');
    QString restartString = "\n" + spacer + " RESTART " + spacer + "\n";
    return restartString;
}

void Console::interpretError(const QString &checkString) {
    if (checkString.contains("AssertionError")) {
        i->errorLabel->setText("AssertionError");
        i->errorDesc->setText("Raised when an assert statement fails.");
    } else if (checkString.contains("AttributeError")) {
        i->errorLabel->setText("AttributeError");
        i->errorDesc->setText("Raised when an attribute reference or assignment fails.");
    } else if (checkString.contains("EOFError")) {
        i->errorLabel->setText("EOFError");
        i->errorDesc->setText("Raised when one of the built-in functions (input() or raw_input()) hits an end-of-file condition (EOF) without reading any data.");
    } else if (checkString.contains("IOError")) {
        i->errorLabel->setText("IOError");
        i->errorDesc->setText("Raised when an I/O operation (such as a print statement, the built-in open() function or a method of a file object) fails for an I/O-related reason, e.g., \"file not found\" or \"disk full\".");
    } else if (checkString.contains("ImportError")) {
        i->errorLabel->setText("ImportError");
        i->errorDesc->setText("Raised when an import statement fails to find the module definition or when a from ... import fails to find a name that is to be imported.");
    } else if (checkString.contains("KeyError")) {
        i->errorLabel->setText("KeyError");
        i->errorDesc->setText("Raised when a mapping (dictionary) key is not found in the set of existing keys.");
    } else if (checkString.contains("MemoryError")) {
        i->errorLabel->setText("MemoryError");
        i->errorDesc->setText("Raised when an operation runs out of memory but the situation may still be rescued (by deleting some objects). The associated value is a string indicating what kind of (internal) operation ran out of memory.");
    } else if (checkString.contains("NameError")) {
        i->errorLabel->setText("NameError");
        i->errorDesc->setText("Raised when a local or global name is not found. This applies only to unqualified names. The associated value is an error message that includes the name that could not be found.");
    } else if (checkString.contains("OverflowError")) {
        i->errorLabel->setText("OverflowError");
        i->errorDesc->setText("Raised when the result of an arithmetic operation is too large to be represented. This cannot occur for long integers (which would rather raise MemoryError than give up) and for most operations with plain integers, which return a long integer instead.");
    } else if (checkString.contains("SyntaxError")) {
        i->errorLabel->setText("SyntaxError");
        i->errorDesc->setText("Raised when the parser encounters a syntax error. This may occur in an import statement, in an exec statement, in a call to the built-in function eval() or input(), or when reading the initial script or standard input (also interactively).");
    } else if (checkString.contains("IndentationError")) {
        i->errorLabel->setText("IndentationError");
        i->errorDesc->setText("Base class for syntax errors related to incorrect indentation.");
    } else if (checkString.contains("TabError")) {
        i->errorLabel->setText("TabError");
        i->errorDesc->setText("Raised when indentation contains an inconsistent use of tabs and spaces.");
    } else if (checkString.contains("TypeError")) {
        i->errorLabel->setText("TypeError");
        i->errorDesc->setText("Raised when an operation or function is applied to an object of inappropriate type. The associated value is a string giving details about the type mismatch.");
    }
}
