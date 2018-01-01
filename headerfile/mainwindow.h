#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "form.h"
#include "bubble.h"
#include "connectdlg.h"
#include "insert.h"
#include "quick.h"
#include "heap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bubble_clicked();
    void on_linkButton_clicked();

    void on_quitButton_clicked();

    void on_insert_clicked();

    void on_quick_clicked();

    void on_heap_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
    Dialog_2 *dialog_2;
    Dialog_3 *dialog_3;
    Dialog_4 *dialog_4;
    ConnDlg *conndlg;

};

#endif // MAINWINDOW_H
