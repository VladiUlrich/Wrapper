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
    tokenizer(nodes);

    for (int i = 0; i < nodes.size(); i++) {
        Node n = nodes[i];
        int dif1 = 0, dif2 = 0;

        if (i > 1 && i < nodes.size()-1) {
            dif1 = n.getLevel() - nodes[i-1].getLevel();
            dif2 = n.getLevel() - nodes[i+1].getLevel();
            if (dif1 < 0) dif1 *= -1;
            if (dif2 < 0) dif2 *= -1;
        }

        if (n.getValue() == NULL && (dif1 > 1 || dif2 > 1)) {
            if (i > 1) {
                int j = i;
                while (j > 0) {
                    j--;
                    if (!nodes[j].isUsed()) {
                        nodes[j].setUsed(true);
                        cout << "</" << nodes[j].getName().toStdString() << ">" << endl;
                        break;
                    }
                }
                /*if (n.getLevel() != nodes[i].getLevel()-1 || n.getLevel() != nodes[i].getLevel()+1) {
                    Node last;
                    for (int j = i-1; j >= 0; j--) {
                        if (nodes[j].getValue() == NULL) {
                            last = nodes[j];
                            break;
                        }
                    }
                    cout << "</" << last.getName().toStdString() << ">" << endl;
                }*/
            }
            cout << "<" << n.getName().toStdString() << ">" << endl;
        }
        else {
            cout << "<" << n.getName().toStdString() << ">"
                 << n.getValue().toStdString() << "</"
                 << n.getName().toStdString() << ">" << endl;
        }
    }
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

        n.setLevel(levelNode(line));
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
