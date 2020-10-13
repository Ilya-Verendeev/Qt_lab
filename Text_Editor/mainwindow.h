#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QIcon>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QIODevice>
#include <QCoreApplication>
#include <QFileDialog>
#include <QTranslator>
#include <QLibraryInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &fileName=QString(), QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_About_programm_triggered();

    void on_actionSelectFont_triggered();

    void documentModified();

    void on_Create_triggered();

    bool on_SaveAs_triggered();

    bool on_Save_triggered();

    void on_actionOpen_triggered();

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QString currentFile;
    QSettings *settings;

    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);
    void setFileName(const QString &fileName);
};
#endif // MAINWINDOW_H
