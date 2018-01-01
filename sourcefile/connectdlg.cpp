#include "connectdlg.h"
#include "ui_connectdlg.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
ConnDlg::ConnDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    QStringList drivers = QSqlDatabase::drivers();    //查找数据库驱动，以QStringList的形式返回所有可用驱动名
    ui.comboDriver->addItems(drivers);      //将这些驱动名加入ui界面的组合框
    connect(ui.comboDriver,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(driverChanged(const QString &))); //关联组合框信号
    ui.status_label->setText(tr("准备链接数据库!"));       //设置当前程序运行状态
}

void ConnDlg::driverChanged(const QString &text)
{
    if(text == "QSQLITE")       //QSQLITE数据库对应的sqlite数据库是一种进程内的本地数据库，不需要数据库名，用户名，密码，主机名端口等特性
    {
        ui.editDatabase->setEnabled(false);
        ui.editUsername->setEnabled(false);
        ui.editPassword->setEnabled(false);
        ui.editHostname->setEnabled(false);
        ui.portSpinBox->setEnabled(false);
    }
    else
    {
        ui.editDatabase->setEnabled(true);
        ui.editUsername->setEnabled(true);
        ui.editPassword->setEnabled(true);
        ui.editHostname->setEnabled(true);
        ui.portSpinBox->setEnabled(true);
    }
}

QString ConnDlg::driverName() const
{
    return ui.comboDriver->currentText();
}

QString ConnDlg::databaseName() const
{
    return ui.editDatabase->text();
}

QString ConnDlg::userName() const
{
    return ui.editUsername->text();
}

QString ConnDlg::password() const
{
    return ui.editPassword->text();
}

QString ConnDlg::hostName() const
{
    return ui.editHostname->text();
}

int ConnDlg::port() const
{
    return ui.portSpinBox->value();
}

void ConnDlg::on_okButton_clicked()
{
    if(ui.comboDriver->currentText().isEmpty())     //检查用户是否选择了数据库驱动
    {
        ui.status_label->setText(tr("请选择一个数据库驱动!"));
        ui.comboDriver->setFocus();
    }
    else if(ui.comboDriver->currentText() == "QSQLITE")     //若是QSQLITE驱动，则创建一个内存数据库
    {
        addSqliteConnection();
        creatDB();
        accept();       //创建数据库表，如已存在则无需执行
    }
    else        //创建一个其他类型的数据库的连接
    {
        QSqlError err = addConnection(driverName(), databaseName(),hostName(),userName(),password(),port());
        if(err.type() != QSqlError::NoError)        //显示错误信息
            ui.status_label->setText(err.text());
        else
            ui.status_label->setText(tr("连接数据库成功"));
        accept();       //创建数据库表，如已存在则无需执行
    }
}
void ConnDlg::on_cancelButton_clicked()
{
    close();
}

QSqlError ConnDlg::addConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
{
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if(!db.open(user,passwd))
    {
        err = db.lastError();
    }
    return err;
}

void ConnDlg::addSqliteConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("databasefile");
    if(!db.open())
    {
        ui.status_label->setText(db.lastError().text());
        return;
    }
    ui.status_label->setText(tr("创建sqlite数据库成功!"));
}

void ConnDlg::creatDB()
{
    QSqlQuery query;
    query.exec("create table factory (id int primary key, num1 int)");      //创建一个表
    query.prepare("insert into factory values (?,?)");        //产生10组的第一个数
    long records = 10;
    for(int i=1;i<=records;i++){      //随机产生10个1~100的整数
        query.bindValue(0,i);
        query.bindValue(1,1+rand()%100);
        query.exec();
    }
    for(int j=2;j<=100;j++)     //产生10组的第2~100个数
    {
        query.exec(QString("alter table factory add num%1 int").arg(j));

        for(int i=1;i<=records;i++){      //随机产生10个1~100的整数
            query.prepare(QString("update factory set num%1 = ? where id = %2").arg(j).arg(i));
            query.bindValue(0,1+rand()%100);
            query.exec();
        }
    }
}
