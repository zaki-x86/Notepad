#include "notepad.h"
#include "./ui_notepad.h"

#include <QAction>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QFontDialog>
#include <QDebug>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    setWindowTitle("New Document");
    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &Notepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &Notepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &Notepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);
    connect(ui->actionAbout, &QAction::triggered, this, &Notepad::about);

// Disable menu actions for unavailable features
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::newDocument() {
    setWindowTitle("New Document");
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void Notepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::save(){
    if(currentFile.isEmpty()) {
        currentFile = QFileDialog::getSaveFileName(this, "Save current file");
    }

    QFile file(currentFile);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(currentFile);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::saveAs(){
    QString filename = QFileDialog::getSaveFileName(this, "Save current file");

    if(currentFile.isEmpty()) {
        currentFile = filename;
        setWindowTitle(currentFile);
    }

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::print(){
    qDebug() << "Pretend like we're printing this thing\n";
}

void Notepad::exit(){
    QCoreApplication::quit();
}

void Notepad::copy(){
#if QT_CONFIG(clipboard)
    ui->textEdit->copy();
#endif
}

void Notepad::cut(){
#if QT_CONFIG(clipboard)
    ui->textEdit->cut();
#endif
}

void Notepad::paste(){
#if QT_CONFIG(clipboard)
    ui->textEdit->paste();
#endif
}

void Notepad::undo(){
    ui->textEdit->undo();
}

void Notepad::redo(){
    ui->textEdit->redo();
}

void Notepad::selectFont(){
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::setFontBold(bool bold){
    bold? ui->textEdit->setFontWeight(QFont::Normal) :
          ui->textEdit->setFontWeight(QFont::Bold);
}

void Notepad::setFontUnderline(bool underline){
    ui->textEdit->setFontUnderline(underline);
}

void Notepad::setFontItalic(bool italic){
    ui->textEdit->setFontItalic(italic);
}

void Notepad::about(){
    QMessageBox::about(this, tr("About MDI"),
                    tr("The <b>Notepad</b> example demonstrates how to code a basic "
                       "text editor using QtWidgets"));
}
