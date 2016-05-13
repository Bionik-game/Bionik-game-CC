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
    Threader();
    ~Threader();

signals:
    void quitting();

public slots:
    /**
     * Funkcja uruchamia podany obiekt w osobnym wątku.
     */
    void runInThread(QObject* object);

    /**
     * Jak wyżej, z tym że jednocześnie uruchamia od razu kilka obiektów w tym samym, nowym wątku.
     */
    void runInThread(std::vector<QObject*> objects);

    void start();
    void quit();
    void wait();
};

#endif // THREADER_H
