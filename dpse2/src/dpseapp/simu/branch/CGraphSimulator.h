#ifndef CGRAPHSIMULATOR_H
#define CGRAPHSIMULATOR_H

#include <QObject>

#include <ISimulator.h>

class CNodeEditorScene;


class CGraphSimulator : public QObject, public ISimulator
{
    Q_OBJECT
public:
    explicit CGraphSimulator(QObject *parent = nullptr);

    // ISimulator
    virtual void setScene(const CNodeEditorScene& scene);
    virtual bool run();
    virtual bool stop();

signals:

public slots:

private:
    const CNodeEditorScene *m_scene = nullptr;
};


#endif // CGRAPHSIMULATOR_H
