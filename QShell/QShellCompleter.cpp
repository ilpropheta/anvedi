#include "QShellCompleter.h"
#include <QStringList>
#include "QtWidgets\qlistview.h"
#include "qevent.h"

QShellCompleter::QShellCompleter(QWidget *parent)
	: QCompleter(parent)
{
}

QShellCompleter::QShellCompleter(QAbstractItemModel *model, QWidget *parent)
	: QCompleter(model, parent)
{
}

void QShellCompleter::setSeparator(const QString &separator)
{
	sep = separator;
}

QString QShellCompleter::separator() const
{
	return sep;
}

QStringList QShellCompleter::splitPath(const QString &path) const
{
	if (sep.isNull()) {
		return QCompleter::splitPath(path);
	}

	return path.split(sep);
}

QString QShellCompleter::pathFromIndex(const QModelIndex &index) const
{
	if (sep.isNull()) {
		return QCompleter::pathFromIndex(index);
	}

	// navigate up and accumulate data
	QStringList dataList;
	for (QModelIndex i = index; i.isValid(); i = i.parent()) {
		dataList.prepend(model()->data(i, completionRole()).toString());
	}

	return dataList.join(sep);
}