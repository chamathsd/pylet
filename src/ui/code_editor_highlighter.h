/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* The following is a C++ reimplementation of the PyQt
* Python syntax highlighter described below:
* https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting
*/

#ifndef CODE_EDITOR_HIGHLIGHTER_H
#define CODE_EDITOR_HIGHLIGHTER_H

#include <qsyntaxhighlighter.h>
#include <tuple>

class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    PythonHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    QList<std::tuple<QRegExp, int, QTextCharFormat>> rules;
    std::tuple<QRegExp, int, QTextCharFormat> tri_single, tri_double;

    bool matchMultiline(const QString &text, const std::tuple<QRegExp, int, QTextCharFormat> &multiRule);
    QTextCharFormat createFormat(const QBrush &brush, const QString &style = "");
};

#endif // CODE_EDITOR_HIGHLIGHTER_H