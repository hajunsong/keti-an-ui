#ifndef STEPTHREAD_H
#define STEPTHREAD_H

#include <QThread>
#include <QObject>
#include <QProcess>

class StepThread : public QThread
{
    Q_OBJECT
public:
    StepThread();
    bool m_start;

private:
    void run();

signals:
    void finish();
};

#endif // STEPTHREAD_H
