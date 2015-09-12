#include "threader.h"

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

Threader::ThreadKey Threader::runInThread(QObject *object)
{
    QThread* thread = new QThread();
    QObject::connect(this, &Threader::quitting, thread, &QThread::quit);
    threads.append(thread);

    object->moveToThread(thread);
    QObject::connect(thread, &QThread::finished, object, &QObject::deleteLater);

    return ThreadKey(thread);
}

void Threader::runInThread(QObject *object, Threader::ThreadKey &threadKey)
{
    QThread* thread = const_cast<QThread*>(threadKey.threadPointer);
    if (!threads.contains(thread))
        throw NoSuchThreadKeyException();

    object->moveToThread(thread);
    QObject::connect(thread, &QThread::finished, object, &QObject::deleteLater);
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



Threader::ThreadKey::ThreadKey(QThread *threadPointer)
    : threadPointer(threadPointer)
{
}
