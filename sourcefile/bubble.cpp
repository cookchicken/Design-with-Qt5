#include "bubble.h"
#include "ui_bubble.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
#include <QSqlRecord>
#include <QAbstractItemView>

Dialog::Dialog(const QString &factoryTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    factoryModel = new QSqlTableModel(this);        //为数据表“factory”创建一个QSqlTableModel模型
    factoryModel->setTable(factoryTable);
    factoryModel->select();
    ui->source->setSelectionBehavior(QAbstractItemView::SelectRows);        //规定用户行为只能选取行
    ui->source->setEditTriggers(QAbstractItemView::NoEditTriggers);         //不能编辑
    ui->source->setModel(factoryModel);

    connect(ui->source,SIGNAL(clicked(QModelIndex )),this,SLOT(to_array(QModelIndex )));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::to_array(QModelIndex index)
{
    int cmp_Count=0,move_Count=0;
    int R[100];
    QSqlRecord record = factoryModel->record(index.row());      //将所选行的数据添加至数组中
    for(int i=1;i<=100;i++)
    {
        R[i] = record.value(QString("num%1").arg(i)).toInt();
    }

    // 添加原始数据
    ui->tableWidget->setColumnCount(100);
    ui->tableWidget->setRowCount(1);
    QStringList headers;
    for(int i=1;i<=100;i++)
    {
        headers.append(QString("num%1").arg(i));
        QTableWidgetItem *item = new QTableWidgetItem (QString("%1").arg(R[i]));
        ui->tableWidget->setItem(0,i-1,item);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    //冒泡排序core
    int i,j,swap;
    for(i=1;i<100;i++)
    {
        // 显示趟数
        ui->textEdit->append(QString("第%1趟结果:").arg(i));
        swap=0;
        for(j=1;j<=100-i;j++)
        {
            cmp_Count++;
            if(R[j]>R[j+1])
            {
                // add textEdit R[j] transfer with R[j+1]
                ui->textEdit->append(QString("元素%1%2%3%4").arg(R[j]).arg("与").arg(R[j+1]).arg("进行了交换\n"));     //显示详细步骤
                R[0]=R[j];
                R[j]=R[j+1];
                R[j+1]=R[0];
                swap = 1;
                move_Count++;
            }
        }
        // 显示第i步结果
        ui->tableWidget->insertRow(i);        //每执行一轮增加一行
        for(int m=1;m<=100;m++)
        {
            ui->tableWidget->setItem(i,m-1,new QTableWidgetItem(QString("%1").arg(R[m])));       //将执行一轮后新的排序输出至tablewidget
        }
        if(swap==0)
            break;
    }
    ui->textEdit->append(QString("此次运算执行了%1次比较，%2次交换").arg(cmp_Count).arg(move_Count));
}

void Dialog::on_pushButton_clicked()
{   
    //清空列表
    int len = ui->tableWidget->rowCount();
    for(int i=len;i>=1;i--)
        ui->tableWidget->removeRow(i);
    //清空文本
    ui->textEdit->clear();
 }
