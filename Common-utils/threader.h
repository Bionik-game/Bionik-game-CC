#ifndef THREADER_H
#define THREADER_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <exception>

class Threader : public QObject
{
    Q_OBJECT
private:
    QVector<QThread*> threads;

public:
    struct ThreadKey {
        friend class Threader;
    private:
        ThreadKey(QThread* threadPointer);
        const QThread* threadPointer;
    };
    class  NoSuchThreadKeyException : public std::exception {
    public:
        const char *what() const noexcept { return "Given ThreadKey does not belong to this Threader."; }
    };

    Threader();
    ~Threader();

signals:
    void quitting();

public slots:
    ThreadKey runInThread(QObject* object);
    void runInThread(QObject* object, ThreadKey& threadKey);
    void start();
    void quit();
    void wait();
};

#endif // THREADER_H
