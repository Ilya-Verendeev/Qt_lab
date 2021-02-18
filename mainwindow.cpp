#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->RunButton->setEnabled(false);
    ui->StopButton->setEnabled(false);
    ui->TimeLabel->setText("00:00:00");
    ui->finalInf->setText("Final Information: ...");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BrowseButton_clicked()
{
    processPath = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QString(),
                                                tr("Executables (*.exe)"));
    if (!processPath.isNull())
    {
        ui->RunButton->setEnabled(true);
        ui->PathLine->setText(processPath);
    }
}

void MainWindow::on_RunButton_clicked()
{
    QStringList arguments;
    QString text = ui->ArgumentLine->text();
    if (!text.isEmpty())
    {
        arguments << text.split(" ");
    }

    process = new QProcess;

    connect(process, &QProcess::stateChanged,
                  this, &MainWindow::stateChanged);

    if (arguments.count() != 0)
        process ->start(processPath, arguments);
    else
        process ->start(processPath);
}

void MainWindow::on_StopButton_clicked()
{
    process->terminate();
}

void MainWindow::readStdOut()
{
    ui->OutTextEdit->moveCursor(QTextCursor::End);
    ui->OutTextEdit->insertPlainText(process->readAllStandardOutput());
}

void MainWindow::readStdErr()
{
    ui->ErrTextEdit->moveCursor(QTextCursor::End);
    ui->ErrTextEdit->insertPlainText(process->readAllStandardError());

}
void MainWindow::stateChanged()
{
    if (process->state() == QProcess::Running)
    {
        ui->BrowseButton->setEnabled(false);
        ui->RunButton->setEnabled(false);
        ui->StopButton->setEnabled(true);
        ui->TimeLabel->setText("00:00:00");
        ui->OutTextEdit->setPlainText("");
        ui->ErrTextEdit->setPlainText("");

        connect(process, &QProcess::readyReadStandardOutput, this,
                &MainWindow::readStdOut);

        connect(process, &QProcess::readyReadStandardError, this,
                &MainWindow::readStdErr);

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this, &MainWindow::ExitApp);

        time = QTime(0, 0, 0);
        timer = new QTimer();
        connect(timer, &QTimer::timeout, this, &MainWindow::slotTimerAlarm);
        timer->start(1000);

        wasStarted = true;
    }
}

void MainWindow::ExitApp()
{
    ui->finalInf->setText("Final infornation:\nExit code: " + QString::number(process->exitStatus()) +
                                  ". Duration: " + ui->TimeLabel->text());
    timer->stop();
    delete timer;

    ui->BrowseButton->setEnabled(true);
    ui->StopButton->setEnabled(false);

    wasStarted = false;
}

void MainWindow::slotTimerAlarm()
{
    time = time.addSecs(1);
    ui->TimeLabel->setText(time.toString("hh:mm:ss"));
}
