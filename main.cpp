#include <iostream>
#include <vector>
#include <QApplication>
#include <QObject>
#include <QThread>
#include "GUI/mainwindow.h"
#include "Rozpoznawator/mainrozpoznawator.h"
#include "Sterowanie_klocki/mainklocki.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    registerMetaTypes();

    /**
     * Obiekt klasy MainWindow działa w głównym wątku
     * (tj. tym samym, w którym wywoływana jest metoda main)
     */
    MainWindow window;

    /**
     * Obiekty poniższych klas reprezentujących elementy naszego
     * programu działają w swoim wątku wraz ze swoją pętlą zdarzeń,
     * implementacją niezależną od pętli zdarzeń (odpowiednik main,
     * to metoda run)
     */
    MainRozpoznawator* rozpoznawator = new MainRozpoznawator;
    QThread rozpoznawatorThread;
    rozpoznawator->moveToThread(&rozpoznawatorThread);
    QObject::connect(&rozpoznawatorThread, &QThread::finished, rozpoznawator, &QObject::deleteLater);

    std::set<ColorBox::Color> colorSet = {ColorBox::BLUE, ColorBox::GREEN};
    MainKlocki* klocki = new MainKlocki(1, colorSet);
    QThread klockiThread;
    klocki->moveToThread(&klockiThread);
    QObject::connect(&klockiThread, &QThread::finished, klocki, &QObject::deleteLater);


    /***** CONNECTIONS *****/

    /**
     * Wyłączenie głównego okna aplikacji spowoduje zatrzymanie pracy wątków.
     */
    QObject::connect(&window, &MainWindow::quittingApplication, &rozpoznawatorThread, &QThread::quit);
    QObject::connect(&window, &MainWindow::quittingApplication, &klockiThread, &QThread::quit);

    /**
     * Łączenie danych między wątkami
     */
    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, klocki, &MainKlocki::gameState);

    /**
     * Wyświetlanie informacji w oknie aplikacji
     */
    QObject::connect(klocki, &MainKlocki::robotCommandUpdate, &window, &MainWindow::updateRobotCommands);

    /***********************/

    /**
     * Uruchamianie wątków
     *
     * Rozpoczęcie pracy nowego wątku następuje w momencie wywołania
     * metody start, która wywołuję metodę run już w nowym wątku.
     * Standardowa implementacja tej metody wywołuję z kolei metodę
     * exec, która obsługuję pętlę zdarzeń.
     */
    rozpoznawatorThread.start();
    klockiThread.start();

    /**
     * Wyświetlanie okna interfejsu użytkownika
     */
    window.show();

    /**
     * Wywołanie obsługi pętli zdarzeń dla głównego wątku.
     * Jest to metoda blokująca (tzn. powrót z wywołania tej metody
     * nastąpi dopiero, gdy zakończymy pracę pętli np. poprzez
     * wyłączenie głównego okna.
     */
    int appReturnValue = app.exec();

    /**
     * Przed zakończeniem wykonania głównego wątku (i wywołaniu destruktora pozostałych wątków)
     * należy poczekać na zakończenie wywołanych wątków.
     */
    rozpoznawatorThread.wait();
    klockiThread.wait();

    return appReturnValue;
}

