#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_connectdlg.h"
class QSqlError;

namespace Ui {
class ConnDlg;
}

class ConnDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConnDlg(QWidget *parent = 0);
    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port = -1);
    void creatDB();
    void addSqliteConnection();
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void driverChanged(const QString &);
private:
    Ui::QSqlConnectionDialogUi ui;
};

#endif // CONNECTDLG_H
