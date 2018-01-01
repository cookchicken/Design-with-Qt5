#ifndef HEAP_H
#define HEAP_H
extern int cmp_Count,move_Count;
#include <QDialog>
#include <QSqlTableModel>
#include <QModelIndex>

namespace Ui {
class Dialog_4;
}

class Dialog_4 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_4(const QString &factoryTable,QWidget *parent = 0);
    ~Dialog_4();

private slots:
    void on_pushButton_clicked();
    void to_array(QModelIndex index);
    void HeapAdjust(int R[],int s,int t);
    void HeapSort(int R[],int n);
private:
    Ui::Dialog_4 *ui;
    QSqlTableModel *factoryModel;
};

#endif // HEAP_H
