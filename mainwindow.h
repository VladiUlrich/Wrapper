#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "node.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int levelNode (QString level);
    void tokenizer (QVector<Node> &);

private slots:
    void openFile();
    void generateXML();
    void saveAs();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
