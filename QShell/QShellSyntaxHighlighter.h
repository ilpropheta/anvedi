#pragma once

#include "qsyntaxhighlighter.h"
#include "qstring.h"
#include "QtWidgets\qplaintextedit.h"

class QShellSyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	QShellSyntaxHighlighter(QTextDocument *parent = 0);

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat singleQuotationFormat;
	QTextCharFormat functionFormat;
};
