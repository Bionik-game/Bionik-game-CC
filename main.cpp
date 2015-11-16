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

    MainPriorytetyzator* priorytetyzator = new MainPriorytetyzator;
    Threader::ThreadKey threadKey = threader.runInThread(priorytetyzator);

    std::set<ColorBox::Color> colorSet = {ColorBox::BLUE, ColorBox::GREEN};
    MainKlocki* klocki = new MainKlocki(1, colorSet);
    threader.runInThread(klocki, threadKey);

    MainJoystick* gamepadBionik = new MainJoystick(1, "/dev/gamepadBionik");
    threader.runInThread(gamepadBionik, threadKey);

    MainJoystick* gamepadCzerwony = new MainJoystick(2, "/dev/gamepadCzerwony");
    threader.runInThread(gamepadCzerwony, threadKey);

    MainJoystick* gamepadNiebieski = new MainJoystick(3, "/dev/gamepadNiebieski");
    threader.runInThread(gamepadNiebieski, threadKey);

    MainWalidator* walidator = new MainWalidator;
    threader.runInThread(walidator);

    QMap<unsigned, QString> ipAddresses;
    ipAddresses.insert(125 ,"192.168.0.20");
    MainKomunikacja* komunikacja = new MainKomunikacja(ipAddresses);
    threader.runInThread(komunikacja);


    /***** CONNECTIONS *****/

    /**
     * Wyświetlanie informacji w oknie aplikacji
     */
    QObject::connect(gamepadBionik, &MainJoystick::robotCommandUpdate, &window, &MainWindow::updateRobotCommandsBionik);
    QObject::connect(gamepadCzerwony, &MainJoystick::robotCommandUpdate, &window, &MainWindow::updateRobotCommandsCzerwony);
    QObject::connect(gamepadNiebieski, &MainJoystick::robotCommandUpdate, &window, &MainWindow::updateRobotCommandsNiebieski);

    /**
     * Łączenie danych między wątkami
     */
    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, priorytetyzator, &MainPriorytetyzator::gameState);
    //QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, walidator, &MainWalidator::gameState);
    QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsColors, klocki, &MainKlocki::getCommands);
    QObject::connect(klocki, &MainKlocki::robotCommandUpdate, walidator, &MainWalidator::robotCommandUpdateRaw);
    //QObject::connect(gamepadBionik, &MainJoystick::robotCommandUpdate, walidator, &MainWalidator::robotCommandUpdateRaw);
    QObject::connect(walidator, &MainWalidator::robotCommandUpdateCorrected, komunikacja, &MainKomunikacja::robotCommandUpdate);
    QObject::connect(rozpoznawator, &MainRozpoznawator::boardPos, walidator, &MainWalidator::boardPos);

    /**
     * Połączenia pomiędzy przyciskami w GUI a funkcjami Rozpoznawatora
     */
    QObject::connect(&window, &MainWindow::startColourConfiguration, rozpoznawator, &MainRozpoznawator::colourCalibration);
    QObject::connect(&window, &MainWindow::startBoardConfiguration, rozpoznawator, &MainRozpoznawator::boardConfiguration);

     // Połączenia umożliwiające wyświetlanie obrazów z Rozpoznawatora w głównym oknie

    QObject::connect( rozpoznawator, &MainRozpoznawator::updateMainImage, &window, &MainWindow::updateMainImage);
    QObject::connect( rozpoznawator, &MainRozpoznawator::updateColourCalibImage, &window, &MainWindow::updateColourCalibImage ) ;
    QObject::connect( rozpoznawator, &MainRozpoznawator::updateBoardConfImage, &window, &MainWindow::updateBoardConfImage);


    //gamepadBionik
    QObject::connect(gamepadBionik, &MainJoystick::gamePadRequest, priorytetyzator, &MainPriorytetyzator::gamePadRequest);
    QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsJoystick, gamepadBionik, &MainJoystick::getCommands);

    //gamepadCzerwony
    //QObject::connect(gamepadCzerwony, &MainJoystick::gamePadRequest, priorytetyzator, &MainPriorytetyzator::gamePadRequest);
    //QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsJoystick, gamepadCzerwony, &MainJoystick::getCommands);

    //gamepadNiebieski
    //QObject::connect(gamepadNiebieski, &MainJoystick::gamePadRequest, priorytetyzator, &MainPriorytetyzator::gamePadRequest);
    //QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsJoystick, gamepadNiebieski, &MainJoystick::getCommands);
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

