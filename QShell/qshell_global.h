#ifndef QSHELL_GLOBAL_H
#define QSHELL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QSHELL_LIB
# define QSHELL_EXPORT Q_DECL_EXPORT
#else
# define QSHELL_EXPORT Q_DECL_IMPORT
#endif

#endif // QSHELL_GLOBAL_H
