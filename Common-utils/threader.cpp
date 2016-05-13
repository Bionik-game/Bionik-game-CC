#include "threader.h"

#include <cassert>

Threader::Threader()
{
}

Threader::~Threader()
{
    quit();
    wait();

    for (QThread* thread : threads)
        delete thread;
}

void Threader::runInThread(QObject * object)
{
    QThread* thread = new QThread();
    QObject::connect(this, &Threader::quitting, thread, &QThread::quit);
    threads.append(thread);

    object->moveToThread(thread);
    QObject::connect(thread, &QThread::finished, object, &QObject::deleteLater);
}

void Threader::runInThread(std::vector<QObject*> objects)
{
    QThread* thread = new QThread();
    QObject::connect(this, &Threader::quitting, thread, &QThread::quit);
    threads.append(thread);

    for(QObject* object : objects)
    {
        object->moveToThread(thread);
        QObject::connect(thread, &QThread::finished, object, &QObject::deleteLater);
    }
}

void Threader::start()
{
    for (QThread* thread : threads)
        thread->start();
}

void Threader::quit()
{
    emit quitting();
}

void Threader::wait()
{
    for (QThread* thread : threads)
        thread->wait();
}
