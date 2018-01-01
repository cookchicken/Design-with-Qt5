#include "insert.h"
#include "ui_insert.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
#include <QSqlRecord>
#include <QAbstractItemView>

Dialog_2::Dialog_2(const QString &factoryTable,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_2)
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

Dialog_2::~Dialog_2()
{
    delete ui;
}
void Dialog_2::to_array(QModelIndex index)
{
    int cmp_Count=0,move_Count=0;
    int R[100];
    QSqlRecord record = factoryModel->record(index.row());      //将所选行的数据添加至数组中
    for(int i=1;i<=100;i++)
    {
        R[i] = record.value(QString("num%1").arg(i)).toInt();
    }
    //设置标题
    ui->step->setColumnCount(100);
    QStringList headers;
    for(int i=1;i<=100;i++)
    {
        headers.append(QString("num%1").arg(i));
        QTableWidgetItem *item = new QTableWidgetItem (QString("%1").arg(R[i]));
        ui->step->setItem(0,i-1,item);
    }
    ui->step->setHorizontalHeaderLabels(headers);

    //插入排序
    int i,j;
    for(i=2;i<=100;i++)
    {
        ui->textEdit->append(QString("第%1趟结果:").arg(i-1));        // 显示趟数
        cmp_Count++;
        if(R[i]<R[i-1])     //进行99趟排序
        {
            R[0]=R[i];      //设置查找监视哨并保存待插入记录R[i]的值
            j=i-1;
            while(R[j]>R[0])        //将值大于R[i]的所有R[j]顺序后移一个记录位置
            {
                cmp_Count++;
                ui->textEdit->append(QString("元素%1%2%3%4").arg(R[j]).arg("与").arg(R[j+1]).arg("进行了交换\n"));     //显示详细步骤
                R[j+1]=R[j];
                j--;
                move_Count++;
            }
            R[j+1]=R[0];        //将R[0]插入到应插入的位置上

            // 显示第i步结果
            int RowCount;        //每执行一轮增加一行
            RowCount = ui->step->rowCount();
            ui->step->insertRow(RowCount);
            //插入元素
            for(int m=1;m<=100;m++)
            {
                ui->step->setItem(RowCount,m-1,new QTableWidgetItem(QString("%1").arg(R[m])));     //将执行一轮后新的排序输出至tablewidget
            }
        }
    }
    ui->textEdit->append(QString("此次运算执行了%1次比较，%2次交换").arg(cmp_Count).arg(move_Count));
}
void Dialog_2::on_pushButton_clicked()
{
    //清空列表
    int len = ui->step->rowCount();
    for(int i=len;i>=1;i--)
        ui->step->removeRow(i);
    //清空文本
    ui->textEdit->clear();
}
