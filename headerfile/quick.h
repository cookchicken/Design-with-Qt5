#ifndef QUICK_H
#define QUICK_H
extern int cmp_Count,move_Count;
#include <QDialog>
#include <QSqlTableModel>
#include <QModelIndex>

namespace Ui {
class Dialog_3;
}

class Dialog_3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_3(const QString &factoryTable,QWidget *parent = 0);
    ~Dialog_3();

private slots:
    void on_pushButton_clicked();
    void to_array(QModelIndex index);
    void quicksort(int R[],int l,int r);
private:
    Ui::Dialog_3 *ui;
    QSqlTableModel *factoryModel;
};

#endif // QUICK_H
