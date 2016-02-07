/*
* Copyright (c) 2016 Jake Dharmasiri.
* Licensed under the GNU GPLv3 License. See LICENSE.md for details.
*
* The following is a C++ reimplementation of the PyQt
* Python syntax highlighter described below:
* https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting
*/

#include "code_editor_highlighter.h"

PythonHighlighter::PythonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {

    QMap<QString, QTextCharFormat> styles;
    styles["normal"] = createFormat(QBrush("#000000"));
    styles["keyword"] = createFormat(QBrush("#FF7700"));
    styles["function"] = createFormat(QBrush("#900090"));
    styles["operator"] = createFormat(QBrush("#000000"));
    styles["brace"] = createFormat(QBrush("#000000"));
    styles["defclass"] = createFormat(QBrush("#0000FF"));
    styles["string"] = createFormat(QBrush("#00AA00"));
    styles["comment"] = createFormat(QBrush("#DD0000"));
    styles["comment2"] = createFormat(QBrush("#00AA00"));
    styles["self"] = createFormat(QBrush("#000000"));
    styles["numbers"] = createFormat(QBrush("#000000"));

    QList<QString> keywords;
    keywords 
        << "and" << "assert" << "break" << "class" << "continue" << "def"
        << "del" << "elif" << "else" << "except" << "exec" << "finally" << "for"
        << "from" << "global" << "if" << "import" << "in" << "is" << "lambda"
        << "not" << "or" << "pass" << "print" << "raise" << "return" << "try"
        << "while" << "yield" << "None" << "True" << "False";

    QList<QString> functions;
    functions
        << "abs" << "all" << "any" << "ascii" << "bin" << "bool" << "bytearray"
        << "bytes" << "callable" << "chr" << "classmethod" << "compile"
        << "complex" << "delattr" << "dict" << "dir" << "divmod" << "enumerate"
        << "eval" << "exec" << "filter" << "float" << "format" << "frozenset"
        << "getattr" << "globals" << "hasattr" << "hash" << "help" << "hex"
        << "id" << "input" << "int" << "isinstance" << "issubclass" << "iter"
        << "len" << "list" << "locals" << "map" << "max" << "memoryview" << "min"
        << "next" << "object" << "oct" << "open" << "ord" << "pow" << "print"
        << "property" << "range" << "repr" << "reversed" << "round" << "set"
        << "setattr" << "slice" << "sorted" << "staticmethod" << "str" << "sum"
        << "super" << "tuple" << "type" << "vars" << "zip" << "__import__";

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
    for (int i = 0; i < keywords.size(); ++i) {
        rules << std::make_tuple(QRegExp("\\b" + keywords.at(i) + "\\b"), 0, styles["keyword"]);
    }
    for (int i = 0; i < functions.size(); ++i) {
        rules << std::make_tuple(QRegExp("(^| )" + functions.at(i) + "\\("), 0, styles["function"]);
    }
    for (int i = 0; i < operators.size(); ++i) {
        rules << std::make_tuple(QRegExp(operators.at(i)), 0, styles["operator"]);
    }
    for (int i = 0; i < braces.size(); ++i) {
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

        // Numeric literals
        << std::make_tuple(QRegExp("\\b[+-]?[0-9]+[lL]?\\b"), 0, styles["numbers"])
        << std::make_tuple(QRegExp("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b"), 0, styles["numbers"])
        << std::make_tuple(QRegExp("\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b"), 0, styles["numbers"])

        // Space between '#" and newline
        << std::make_tuple(QRegExp("#[^\\n]*"), 0, styles["comment"]);

    tri_single = std::make_tuple(QRegExp("'''"), 1, styles["comment2"]);
    tri_double = std::make_tuple(QRegExp("\"\"\""), 2, styles["comment2"]);

    returnFormat = styles["keyword"];
    normalFormat = styles["normal"];
}

void PythonHighlighter::highlightBlock(const QString &text) {
    setFormat(0, text.length(), normalFormat);

    for (int i = 0; i < rules.size(); ++i) {
        QRegExp expression = std::get<0>(rules.at(i));
        int nth = std::get<1>(rules.at(i));
        int index = expression.indexIn(text, 0);

        while (index >= 0) {
            int length = expression.cap(nth).length();
            index = expression.pos(nth);
            setFormat(index, length, std::get<2>(rules.at(i)));
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    /* Multi-line strings */
    bool inMultiline = matchMultiline(text, tri_single);
    if (!inMultiline) {
        inMultiline = matchMultiline(text, tri_double);
    }

}

bool PythonHighlighter::matchMultiline(const QString &text, const std::tuple<QRegExp, int, QTextCharFormat> &multiRule) {
    QRegExp delimiter = std::get<0>(multiRule);
    int in_state = std::get<1>(multiRule);
    QTextCharFormat style = std::get<2>(multiRule);
    int start, add, end, length;

    if (previousBlockState() == in_state) {
        start = 0;
        add = 0;
    } else {
        start = delimiter.indexIn(text);
        add = delimiter.matchedLength();
    }

    while (start >= 0) {
        end = delimiter.indexIn(text, start + add);

        if (end >= add) {
            length = end - start + add + delimiter.matchedLength();
            setCurrentBlockState(0);
        } else {
            setCurrentBlockState(in_state);
            length = text.length() - start + add;
        }

        setFormat(start, length, style);
        start = delimiter.indexIn(text, start + length);
    }

    if (currentBlockState() == in_state) {
        return true;
    } else {
        return false;
    }
}

QTextCharFormat PythonHighlighter::createFormat(const QBrush &brush, const QString &style) {
    QTextCharFormat format;
    format.setForeground(brush);
    if (style.contains("bold")) {
        format.setFontWeight(QFont::Bold);
    }
    if (style.contains("italic")) {
        format.setFontItalic(true);
    }

    return format;
}
