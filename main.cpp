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

    MainJoystick* joystick = new MainJoystick(1, "/dev/ttyUSB0");
    threader.runInThread(joystick, threadKey);

    MainWalidator* walidator = new MainWalidator;
    threader.runInThread(walidator);

    MainKomunikacja* komunikacja = new MainKomunikacja("192.168.0.101");
    threader.runInThread(komunikacja);


    /***** CONNECTIONS *****/

    /**
     * Wyświetlanie informacji w oknie aplikacji
     */
    QObject::connect(walidator, &MainWalidator::robotCommandUpdateCorrected, &window, &MainWindow::updateRobotCommands);

    /**
     * Łączenie danych między wątkami
     */
    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, priorytetyzator, &MainPriorytetyzator::gameState);
    QObject::connect(rozpoznawator, &MainRozpoznawator::gameState, walidator, &MainWalidator::gameState);
    QObject::connect(joystick, &MainJoystick::gamePadRequest, priorytetyzator, &MainPriorytetyzator::gamePadRequest);
    QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsColors, klocki, &MainKlocki::getCommands);
    QObject::connect(priorytetyzator, &MainPriorytetyzator::getCommandsJoystick, joystick, &MainJoystick::getCommands);
    QObject::connect(klocki, &MainKlocki::robotCommandUpdate, walidator, &MainWalidator::robotCommandUpdateRaw);
    QObject::connect(joystick, &MainJoystick::robotCommandUpdate, walidator, &MainWalidator::robotCommandUpdateRaw);
    QObject::connect(walidator, &MainWalidator::robotCommandUpdateCorrected, komunikacja, &MainKomunikacja::robotCommandUpdate);

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

