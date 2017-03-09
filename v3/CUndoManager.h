#ifndef CUNDOMANAGER_H
#define CUNDOMANAGER_H

#include <QtCore/QByteArray>

#include "IUndoManager.h"

class CEditorScene;


class CUndoManager : public IUndoManager
{
public:
	CUndoManager(CEditorScene &scene);
	~CUndoManager();

	// reimp
	virtual void reset();
	virtual void addState();
	virtual void undo();
	virtual void redo();

private:

	CEditorScene *m_scene;
	QByteArray m_currentState;
};

#endif // CUNDOMANAGER_H
