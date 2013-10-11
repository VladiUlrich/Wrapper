#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <iostream>
#include <QTextStream>
#include <string>
#include "node.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveAs()));
    QObject::connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(generateXML()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Abrir txt
void MainWindow::openFile()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Texto (*.txt)"));
    if (dialog.exec()) {
        cout << dialog.selectedFiles().last().toStdString() << endl;
        QString fileName = dialog.selectedFiles().last();
        QFile file (fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
            ui->textNormal->setPlainText(file.readAll());
    }
}

//Generar xml
void MainWindow::generateXML()
{
    QVector<Node> nodes;
    QString salida = "";
    tokenizer(nodes);
    for (int i = 0; i < nodes.size(); i++) {
        Node n = nodes[i];

        for (int tabs = 0; tabs < n.getLevel(); tabs++)
            salida += "\t";
        if (n.getValue() == NULL) {
            salida += ("<"+n.getName()+">");
        }
        else {
            salida += "<"+n.getName()+">"+n.getValue()+"</"+n.getName()+">";
        }
        salida += "\n";
    }
    ui->textXML->setPlainText(salida);
}

int MainWindow::levelNode (QString line) {
    //Caracteres de la linea
    int level = 0;
    for (int j = 0; j < line.length(); j++) {
        QChar c = line[j];
        if (c == '\t') level++;
    }
    return level;
}

void MainWindow::tokenizer (QVector<Node> &nodes) {
    QString input = ui->textNormal->toPlainText();
    QStringList lines = input.split('\n');
    ui->textXML->clear();

    //Lineas del archivo
    for (int i = 0; i < lines.length(); i++) {
        Node n;
        QString line = lines[i];
        QStringList segmentedLine;
        bool hasValue = line.contains(':', Qt::CaseInsensitive);

        n.setLevel(levelNode(line));

        if (hasValue) {
            segmentedLine = line.split(':');
            segmentedLine[0].replace("\t", "");
            segmentedLine[1].remove(0,1);
            n.setName(segmentedLine[0]);
            n.setValue(segmentedLine[1]);
            n.setUsed(true);
        }
        else {
            n.setName(line.replace("\t", ""));
            n.setUsed(false);
        }

        nodes.push_back(n);
    }
}

//Guardar txt
void MainWindow::saveAs()
{
    QFileDialog dialog (this);
    QString fileName = dialog.getSaveFileName(this, tr("Guardar como..."), QString(), tr("TXT (*.txt)"));

    cout << fileName.toStdString() << endl;

    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".txt", Qt::CaseInsensitive))
        fileName += ".txt";

    QFile file (fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Error al abrir archivo modo escritura" << endl;
        return;
    }

    QTextStream stream (&file);
    stream << ui->textNormal->toPlainText();
}
