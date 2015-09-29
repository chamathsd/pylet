/*
* Copyright (c) 2015 Jake Schadaway.
* Licensed under the MIT License. See LICENSE.md for details.
*
* Special thanks to Qt for their code editor reference,
* from which the following code is forked.
*/

#include "code_editor_interface.h"
#include "code_editor_numbers.h"
#include <qtextobject.h>
#include <qpainter.h>

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent)
{
	lineNumbers = new LineNumberWidget(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumbersWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumbersArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumbersWidth(0);
	highlightCurrentLine();

	QTextDocument *doc = this->document();
	QFont monoFont = doc->defaultFont();
	monoFont.setFamily("Courier New");
	doc->setDefaultFont(monoFont);
}

int CodeEditor::lineNumbersWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
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
	painter.fillRect(event->rect(), Qt::lightGray);

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
			painter.drawText(0, top, lineNumbers->width(), fontMetrics().height(), Qt::AlignRight, number);
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

void CodeEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelection;

	if (!isReadOnly())
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::yellow).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelection.append(selection);
	}

	setExtraSelections(extraSelection);
}