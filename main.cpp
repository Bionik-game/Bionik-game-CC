#include <iostream>
#include <QApplication>
#include <QObject>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>

#include "GUI/mainwindow.h"
#include "Rozpoznawator/mainrozpoznawator.h"
#include "Sterowanie_klocki/mainklocki.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
    MainRozpoznawator rozpoznawator;
    MainKlocki klocki;


    /***** CONNECTIONS *****/

    /**
     * Wyłączenie głównego okna aplikacji spowoduje zatrzymanie pracy wątków.
     */
    QObject::connect(&window, &MainWindow::quittingApplication, &rozpoznawator, &MainRozpoznawator::quit);
    QObject::connect(&window, &MainWindow::quittingApplication, &klocki, &MainKlocki::quit);

    /**
     * Połączenia pomiędzy przyciskami w GUI a funkcjami Rozpoznawatora
     */
    QObject::connect(&window, &MainWindow::startColourConfiguration, &rozpoznawator,&MainRozpoznawator::colourCalibration);
    QObject::connect(&window, &MainWindow::startBoardConfiguration, &rozpoznawator,&MainRozpoznawator::boardConfiguration);
    /**
     * Łączenie danych między wątkami
     */
    QObject::connect(&rozpoznawator, &MainRozpoznawator::robotPositionUpdate, &klocki, &MainKlocki::updateRobotPosition);
    QObject::connect(&rozpoznawator, &MainRozpoznawator::boxesPositionUpdate, &klocki, &MainKlocki::updateBoxesPositions);

    /**
     * Wyświetlanie informacji w oknie aplikacji
     */
    QObject::connect(&klocki, &MainKlocki::robotCommandUpdate, &window, &MainWindow::updateRobotCommands);

    /***********************/

    /**
     * Uruchamianie wątków
     *
     * Rozpoczęcie pracy nowego wątku następuje w momencie wywołania
     * metody start, która wywołuję metodę run już w nowym wątku.
     * Standardowa implementacja tej metody wywołuję z kolei metodę
     * exec, która obsługuję pętlę zdarzeń.
     */
    rozpoznawator.start();
  //  klocki.start();

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
    rozpoznawator.wait();
  //  klocki.wait();

    return appReturnValue;
}

