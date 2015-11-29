#ifndef QSHELL_H
#define QSHELL_H

#include "qshell_global.h"
#include "QtWidgets\qplaintextedit.h"
#include "CommandProvider.h"
#include "QShellEngine.h"
#include "KeyHandlersManager.h"
#include "QShellEngine_Qt.h"

class QSHELL_EXPORT QShell : public QPlainTextEdit
{
	Q_OBJECT
public:
	QShell(QWidget* parent, const QString& promptString = "Q> ");

	// setters
	void SetEngine(std::shared_ptr<QShellEngine> engine);
	void SetEngine(std::shared_ptr<QShellEngine_Qt> engine);
	void SetPrompt(const QString& newPrompt);
	void SetHandlers(HandlersVec handlers);

	const QString& Prompt() const;

	void processKeyEvent(QKeyEvent* e);
	void processWheelEvent(QWheelEvent* e);
signals:
	void LoadScriptRequested(const QString&);
protected:
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);
	
	void contextMenuEvent(QContextMenuEvent *e);
	void insertFromMimeData ( const QMimeData * source );	
private:
	QString prompt;
	CommandProvider commands;
	KeyHandlersManager handlersMananger;
};

#endif // QSHELL_H
