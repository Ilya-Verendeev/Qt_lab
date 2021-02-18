#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QErrorMessage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BrowseButton_clicked();

    void on_RunButton_clicked();

    void on_StopButton_clicked();

    void stateChanged();

    void readStdOut();

    void readStdErr();

    void ExitApp();

    void slotTimerAlarm();

private:
    Ui::MainWindow *ui;

    QString processPath;
    QProcess *process;
    QTimer* timer;
    QTime time;
    bool wasStarted = false;
};
#endif // MAINWINDOW_H
