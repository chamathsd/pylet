/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* Special thanks to Qt for their code editor reference,
* from which the following code is forked.
*/

#include "code_editor_interface.h"
#include "code_editor_numbers.h"
#include <qcoreapplication.h>
#include <qtextobject.h>
#include <qpainter.h>
#include <qdebug.h>

CodeEditor::CodeEditor(QSettings* s, QWidget* parent) : QPlainTextEdit(parent)
{
    lineNumbers = new LineNumberWidget(this);

    updateLineNumbersWidth(0);
    highlightCurrentLine();

    tabSpacing = s->value("Editor/iTabSpacing").toInt();
    tabsEmitSpaces = s->value("Editor/bTabsEmitSpaces").toBool();

    setFont(monoFont);
    setWordWrapMode(QTextOption::NoWrap);
    setTabStopWidth(tabSpacing * fontMetrics().width(' '));
    
    /* Syntax highlighter for Python documents */
    highlighter = new PythonHighlighter(this->document());

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumbersWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumbersArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

int CodeEditor::lineNumbersWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) 
    {
        max /= 10;
        ++digits;
    }

    int space = fontMetrics().width(QLatin1Char('9')) * digits;

    return space + 30; /* width offset */
}

void CodeEditor::updateLineNumbersWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumbersWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumbersArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumbers->scroll(0, dy);
    }
    else
    {
        lineNumbers->update(0, rect.y(), lineNumbers->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumbersWidth(0);
    }
}

void CodeEditor::lineNumbersPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumbers);
    QRect lineNumberBG = event->rect();
    lineNumberBG.setWidth(lineNumberBG.width() - 8);
    painter.fillRect(lineNumberBG, Qt::lightGray);

    monoFont.setPointSize(this->font().pointSize());
    painter.setFont(monoFont);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingGeometry(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(-20, top, lineNumbers->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumbers->setGeometry(QRect(cr.left(), cr.top(), lineNumbersWidth(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Tab:
        {
            if (tabsEmitSpaces)
            {
                event->accept();
                this->textCursor().insertText(QString(tabSpacing, ' '));
            }
            else
            {
                QPlainTextEdit::keyPressEvent(event);
            }
            break;
        }
        case Qt::Key_Return: case Qt::Key_Enter:
        {
            QString previousBlock = this->textCursor().block().text();
            QString::const_iterator iter = previousBlock.begin();
            bool validReturn = false;

            QString whitespace;
            while (iter != previousBlock.end())
            {
                const QChar &c = *iter;
                if (c == ' ' || c == '\t')
                {
                    whitespace.append(c);
                }
                else
                {
                    break;
                }
                ++iter;
            }

            QString colonSearch;
            QString returnSearch;

            foreach(QTextLayout::FormatRange r, textCursor().block().layout()->additionalFormats())
            {
                if (r.format == highlighter->normalFormat)
                {
                    colonSearch.append(previousBlock.mid(r.start, r.length));
                }
                if (r.format == highlighter->returnFormat)
                {
                    returnSearch.append(previousBlock.mid(r.start, r.length));
                }
            }

            if (colonSearch.contains(QRegExp(":")))
            {
                if (tabsEmitSpaces)
                {
                    whitespace += QString(tabSpacing, ' ');
                }
                else
                {
                    whitespace.append('\t');
                }
            }
            else if (returnSearch.contains(QRegExp("return")))
            {
                unsigned int tabOver = 0;
                foreach(QChar c, whitespace)
                {
                    if (c == ' ')
                    {
                        tabOver += 1;
                    }
                    else if (c == '\t')
                    {
                        tabOver += tabSpacing;
                    }
                }
                tabOver /= tabSpacing;
                if (tabOver > 0)
                {
                    tabOver -= 1;
                }

                if (tabsEmitSpaces)
                {
                    whitespace = QString(tabSpacing * tabOver, ' ');
                }
                else 
                {
                    whitespace = QString(tabOver, '\t');
                }
            }

            QPlainTextEdit::keyPressEvent(event);
            this->textCursor().insertText(whitespace);
            break;
        }
        default:
        {
            QPlainTextEdit::keyPressEvent(event);
            break;
        }
    }
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelection;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(190);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelection.append(selection);
    }

    setExtraSelections(extraSelection);
}

void CodeEditor::zoomInSlot()
{
    this->zoomIn(2);
}

void CodeEditor::zoomOutSlot()
{
    this->zoomOut(2);
}

void CodeEditor::resetZoom()
{
    QFont f = font();
    f.setPointSize(12);
    setFont(f);
}