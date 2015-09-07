#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Common-utils/mapelements.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void quittingApplication();
    void startColourConfiguration();
    void startBoardConfiguration();

public slots:
    void updateRobotCommands(RobotCommands robotCommands);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
