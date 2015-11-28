#pragma once

#include "QtWidgets\qcompleter.h"

class QShellCompleter : public QCompleter
{
	Q_OBJECT
		Q_PROPERTY(QString separator READ separator WRITE setSeparator)

public:
	explicit QShellCompleter(QWidget *parent = 0);
	explicit QShellCompleter(QAbstractItemModel *model, QWidget *parent = 0);

	QString separator() const;
	public slots:
		void setSeparator(const QString &separator);
protected:
	QStringList splitPath(const QString &path) const;
	QString pathFromIndex(const QModelIndex &index) const;
private:
	QString sep;
};


