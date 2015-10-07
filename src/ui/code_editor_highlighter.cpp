/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* The following is a C++ reimplementation of the PyQt
* Python syntax highlighter described below:
* https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting
*/

#include "code_editor_highlighter.h"

PythonHighlighter::PythonHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    QMap<QString, QTextCharFormat> styles;
    styles["keyword"] = createFormat(Qt::blue);
    styles["operator"] = createFormat(Qt::red);
    styles["brace"] = createFormat(Qt::darkGray);
    styles["defclass"] = createFormat(Qt::black, "bold");
    styles["string"] = createFormat(Qt::magenta);
    styles["string2"] = createFormat(Qt::darkMagenta);
    styles["comment"] = createFormat(Qt::darkGreen, "italic");
    styles["self"] = createFormat(Qt::black, "italic");
    styles["numbers"] = createFormat(Qt::darkCyan);

    QList<QString> keywords;
    keywords 
        << "and" << "assert" << "break" << "class" << "continue" << "def"
        << "del" << "elif" << "else" << "except" << "exec" << "finally" << "for"
        << "from" << "global" << "if" << "import" << "in" << "is" << "lambda"
        << "not" << "or" << "pass" << "print" << "raise" << "return" << "try"
        << "while" << "yield" << "None" << "True" << "False";

    QList<QString> operators;
    operators
        // Assignment
        << "="
        // Comparison
        << "==" << "!=" << "<" << "<=" << ">" << ">="
        // Arithmetic
        << "\\+" << "-" << "\\*" << "/" << "//" << "\\%" << "\\*\\*"
        // In-place
        << "+=" << "-=" << "\\*=" << "/=" << "\\%="
        // Bitwise
        << "\\^" << "\\|" << "\\&" << "\\~" << ">>" << "<<";

    QList<QString> braces;
    braces
        << "\\{" << "\\}" << "\\(" << "\\)" << "\\[" << "\\]";

    /* Blanket rules for keywords, operators, and braces */
    for (int i = 0; i < keywords.size(); ++i)
    {
        rules << std::make_tuple(QRegExp("\\b" + keywords.at(i) + "\\b"), 0, styles["keyword"]);
    }
    for (int i = 0; i < operators.size(); ++i)
    {
        rules << std::make_tuple(QRegExp(operators.at(i)), 0, styles["operator"]);
    }
    for (int i = 0; i < braces.size(); ++i)
    {
        rules << std::make_tuple(QRegExp(braces.at(i)), 0, styles["brace"]);
    }

    /* All other rules */
    rules
        // Self
        << std::make_tuple(QRegExp("\\bself\\b"), 0, styles["self"])

        // Double-quoted string
        << std::make_tuple(QRegExp("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\""), 0, styles["string"])

        // Single-quoted string
        << std::make_tuple(QRegExp("'[^'\\\\]*(\\\\.[^'\\\\]*)*'"), 0, styles["string"])

        // 'def' followed by identifier
        << std::make_tuple(QRegExp("\\bdef\\b\\s*(\\w+)"), 1, styles["defclass"])

        // 'class' followed by identifier
        << std::make_tuple(QRegExp("\\bclass\\b\\s*(\\w+)"), 1, styles["defclass"])

        // Space between '#" and newline
        << std::make_tuple(QRegExp("#[^\\n]*"), 0, styles["comment"])

        // Numeric literals
        << std::make_tuple(QRegExp("\\b[+-]?[0-9]+[lL]?\\b"), 0, styles["numbers"])
        << std::make_tuple(QRegExp("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b"), 0, styles["numbers"])
        << std::make_tuple(QRegExp("\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b"), 0, styles["numbers"]);
}

void PythonHighlighter::highlightBlock(const QString &text)
{
    for (int i = 0; i < rules.size(); ++i)
    {
        QRegExp expression = std::get<0>(rules.at(i));
        int nth = std::get<1>(rules.at(i));
        int index = expression.indexIn(text, 0);

        while (index >= 0)
        {
            int length = expression.cap(nth).length();
            index = expression.pos(nth);
            setFormat(index, length, std::get<2>(rules.at(i)));
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

QTextCharFormat PythonHighlighter::createFormat(const QBrush &brush, const QString &style)
{
    QTextCharFormat format;
    format.setForeground(brush);
    if (style.contains("bold"))
    {
        format.setFontWeight(QFont::Bold);
    }
    if (style.contains("italic"))
    {
        format.setFontItalic(true);
    }

    return format;
}