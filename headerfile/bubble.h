#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QModelIndex>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const QString &factoryTable, QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void to_array(QModelIndex index);
private:
    Ui::Dialog *ui;
    QSqlTableModel *factoryModel;
};

#endif // DIALOG_H
