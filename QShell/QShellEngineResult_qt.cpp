#include "QShellEngineResult_Qt.h"
#include <QString>

QShellEngineResult_Qt::QShellEngineResult_Qt( QScriptValue value ) : m_value(value)
{

}

QString QShellEngineResult_Qt::ToQString() const
{
	return m_value.toString();
}

bool QShellEngineResult_Qt::IsPrintable() const
{
	return false == (m_value.isNull() || m_value.isUndefined());
}

bool QShellEngineResult_Qt::IsError() const
{
	return m_value.isError();
}

