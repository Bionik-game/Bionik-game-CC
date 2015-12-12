#include <iostream>
#include <vector>
#include <QApplication>
#include <QObject>
#include <QThread>
#include "GUI/mainwindow.h"
#include "Rozpoznawator/mainrozpoznawator.h"
#include "Sterowanie_klocki/mainklocki.h"
#include "Priorytetyzator/mainpriorytetyzator.h"
#include "Sterowanie_joystick/mainjoystick.h"
#include "Walidator/mainwalidator.h"
#include "Komunikacja/mainkomunikacja.h"
#include "Common-utils/threader.h"

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
     * to metoda run). Wyłączenie wątków nastąpi w destruktorze
     * klasy Threader.
     */
    Threader threader;

    MainRozpoznawator* rozpoznawator = new MainRozpoznawator;
    threader.runInThread(rozpoznawator);

    std::set<ColorBox::Color> colorSetNiebieski = {ColorBox::BLUE, ColorBox::GREEN};
    MainKlocki* klockiNiebieski = new MainKlocki(125, colorSetNiebieski);
    Threader::ThreadKey threadKey = threader.runInThread(klockiNiebieski);

    std::set<ColorBox::Color> colorSetCzerwony = {ColorBox::RED, ColorBox::GREEN};
    MainKlocki* klockiCzerwony = new MainKlocki(175, colorSetCzerwony);
    threader.runInThread(klockiCzerwony, threadKey);

    MainWalidator* walidatorCzerwony = new MainWalidator;
    threader.runInThread(walidatorCzerwony, threadKey);

    MainWalidator* walidatorNiebieski = new MainWalidator;
    threader.runInThread(walidatorNiebieski, threadKey);

    QMap<unsigned, QString> ipAddresses;
    ipAddresses.insert(125 ,"192.168.0.20:30001");
    ipAddresses.insert(175 ,"192.168.0.21:30001");
    MainKomunikacja* komunikacja = new MainKomunikacja(ipAddresses);
    threader.runInThread(komunikacja);


    /***** CONNECTIONS *****/

    /**
     * Wyświetlanie informacji w oknie aplikacji
     */
    QObject::connect(walidatorNiebieski, &MainWalidator::robotCommandUpdateCorrected, &window, &MainWindow::updateRobotCommandsNiebieski);
    QObject::connect(walidatorCzerwony, &MainWalidator::robotCommandUpdateCorrected, &window, &MainWindow::updateRobotCommandsCzerwony);

    /**
     * Łączenie danych między wątkami
     */
    QObject::connect(rozpoznawator, &MainRozpoznawator::boardPos, walidatorNiebieski, &MainWalidator::boardPos);
    QObject::connect(rozpoznawator, &MainRozpoznawator::boardPos, walidatorCzerwony, &MainWalidator::boardPos);

    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, klockiNiebieski, &MainKlocki::getCommands);
    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, klockiCzerwony, &MainKlocki::getCommands);

    QObject::connect(klockiNiebieski, &MainKlocki::robotCommandUpdate, walidatorNiebieski, &MainWalidator::robotCommandUpdateRaw);
    QObject::connect(klockiCzerwony, &MainKlocki::robotCommandUpdate, walidatorCzerwony, &MainWalidator::robotCommandUpdateRaw);

    QObject::connect(walidatorNiebieski, &MainWalidator::robotCommandUpdateCorrected, komunikacja, &MainKomunikacja::robotCommandUpdate);
    QObject::connect(walidatorCzerwony, &MainWalidator::robotCommandUpdateCorrected, komunikacja, &MainKomunikacja::robotCommandUpdate);

    /**
     * Połączenia pomiędzy przyciskami w GUI a funkcjami Rozpoznawatora
     */
    QObject::connect(&window, &MainWindow::startColourConfiguration, rozpoznawator, &MainRozpoznawator::colourCalibration);
    QObject::connect(&window, &MainWindow::startBoardConfiguration, rozpoznawator, &MainRozpoznawator::boardConfiguration);

     // Połączenia umożliwiające wyświetlanie obrazów z Rozpoznawatora w głównym oknie

    QObject::connect( rozpoznawator, &MainRozpoznawator::updateMainImage, &window, &MainWindow::updateMainImage);
    QObject::connect( rozpoznawator, &MainRozpoznawator::updateColourCalibImage, &window, &MainWindow::updateColourCalibImage ) ;
    QObject::connect( rozpoznawator, &MainRozpoznawator::updateBoardConfImage, &window, &MainWindow::updateBoardConfImage);

    /***********************/

    /**
     * Uruchamianie wątków
     *
     * Rozpoczęcie pracy nowego wątku następuje w momencie wywołania
     * metody start, która wywołuję metodę run już w nowym wątku.
     * Standardowa implementacja tej metody wywołuję z kolei metodę
     * exec, która obsługuję pętlę zdarzeń.
     */
    threader.start();

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
    return app.exec();
}

