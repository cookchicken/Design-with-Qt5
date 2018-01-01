#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_linkButton_clicked()
{
    conndlg = new ConnDlg;
    conndlg->show();
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

void MainWindow::on_bubble_clicked()
{
    dialog = new Dialog("factory");
    dialog->show();
}

void MainWindow::on_insert_clicked()
{
    dialog_2 = new Dialog_2("factory");
    dialog_2->show();
}

void MainWindow::on_quick_clicked()
{
    dialog_3 = new Dialog_3("factory");
    dialog_3->show();
}


void MainWindow::on_heap_clicked()
{
    dialog_4 = new Dialog_4("factory");
    dialog_4->show();
}
