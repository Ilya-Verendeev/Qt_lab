#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QString &fileName,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings(this);
    ui->textEdit->setFont(settings->value("font", QFont()).value<QFont>());

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    this->setCentralWidget(ui->textEdit);

    connect(ui->Close, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->Exit, SIGNAL(triggered()),
            qApp, SLOT(closeAllWindows()));

    connect(ui->textEdit, SIGNAL(textChanged()),
            this, SLOT(documentModified()));

    connect(ui->actionAboutQt, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
    connect(ui->actionCut, SIGNAL(triggered()),
            ui->textEdit, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()),
            ui->textEdit, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()),
            ui->textEdit, SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered()),
            ui->textEdit, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()),
            ui->textEdit, SLOT(redo()));

    connect(ui->textEdit, SIGNAL(cutAvailable(bool)),
            ui->actionCut, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)),
            ui->actionCopy, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)),
            ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)),
            ui->actionRedo, SLOT(setEnabled(bool)));

    if (!fileName.isNull())
        loadFile(fileName);


    this->setWindowTitle(QString("%1[*]-%2")
            .arg(currentFile.isNull()?"новый документ":QFileInfo(currentFile).fileName())
            .arg(QApplication::applicationName()));

    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::documentModified()
{
    this->setWindowTitle(QString("%1[*]-%2")
            .arg(currentFile.isNull()?"новый документ":QFileInfo(currentFile).fileName())
            .arg(QApplication::applicationName()));
    this->setWindowModified(true);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();

}

bool MainWindow::maybeSave()
{
    //if (ui->textEdit->document()->isModified())
    if (isWindowModified())
    {
        QMessageBox::StandardButton tmp;
        tmp = QMessageBox::warning(this, "Документ изменен", "Документ был изменен, хотите ли вы сохранить его?\n"
                                      "Все изменения будут утеряны.", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
        if (tmp == QMessageBox::Yes)
        {
            if (currentFile.isEmpty()) {
                return on_SaveAs_triggered();
            } else {
                return saveFile(currentFile);
            }
        }
        else if (tmp == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName + ".txt");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ui->textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    currentFile = fileName;
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
    return true;
}

/*void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    ui->textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFile;
    if (currentFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}*/

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание!"),
                             tr("Не удалось открыть файл!"));
        setFileName(QString());

    }
    else
    {
        QTextStream in(&file);
        ui->textEdit->setPlainText(in.readAll());
        file.close();
        setFileName(fileName);
        this->setWindowModified(false);
    }
}

void MainWindow::setFileName(const QString &fileName)
{
    currentFile = fileName;
    this->setWindowTitle(QString("%1[*]-%2")
            .arg(currentFile.isNull()?"новый документ":QFileInfo(currentFile).fileName())
            .arg(QApplication::applicationName()));
}

void MainWindow::on_Create_triggered()
{
   MainWindow *window = new MainWindow();
   window->show();
   window->setWindowTitle(QString("%1[*]-%2")
           .arg(currentFile.isNull()?"новый документ":QFileInfo(currentFile).fileName())
           .arg(QApplication::applicationName()));
}

bool MainWindow::on_Save_triggered()
{
    if (currentFile.isEmpty()) {
        return on_SaveAs_triggered();
    } else {
        return saveFile(currentFile);
    }
}

bool MainWindow::on_SaveAs_triggered()
{
     QString fileName = QFileDialog::getSaveFileName(this);
     if (fileName.isEmpty())
        return false;

     return saveFile(fileName);
}

void MainWindow::on_actionSelectFont_triggered()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok, QFont("Arial", 14));
    if (ok) {
        ui->textEdit->setFont(font);
        settings->setValue("font", font);
    }

}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть документ", QDir::currentPath(), "Text documents (*.txt)");
    if (!fileName.isNull())
    {
        if (currentFile.isNull())
        {
            loadFile(fileName);
        }
        else
        {
            MainWindow *window = new MainWindow(fileName);
            window->show();
        }
    }

}

void MainWindow::on_actionAbout_triggered()
{
    QString appName(qApp->applicationName());
    QString appVersion(qApp->applicationVersion());
    QMessageBox::about(this, "О программе", appName + "\nVersion: " +
                       appVersion + "\n" + "Разработчик : \n\tВерендеев Илья");
}
