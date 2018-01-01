#ifndef INSERT_H
#define INSERT_H

#include <QDialog>
#include <QSqlTableModel>
#include <QModelIndex>

namespace Ui {
class Dialog_2;
}

class Dialog_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_2(const QString &factoryTable,QWidget *parent = 0);
    ~Dialog_2();

private slots:
    void on_pushButton_clicked();
    void to_array(QModelIndex index);
private:
    Ui::Dialog_2 *ui;
    QSqlTableModel *factoryModel;
};

#endif // INSERT_H
