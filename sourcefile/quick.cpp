#include "quick.h"
#include "ui_quick.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
#include <QSqlRecord>
#include <QAbstractItemView>
#include <algorithm>

int cmp_Count=0;
int move_Count=0;

Dialog_3::Dialog_3(const QString &factoryTable,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_3)
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

Dialog_3::~Dialog_3()
{
    delete ui;
}

void Dialog_3::to_array(QModelIndex index)
{
    int R[100];
    QSqlRecord record = factoryModel->record(index.row());      //将所选行的数据添加至数组中
    for(int i=1;i<=100;i++)
    {
        R[i] = record.value(QString("num%1").arg(i)).toInt();
    }
    /*设置标题*/
    ui->step->setColumnCount(100);
    QStringList headers;
    for(int i=1;i<=100;i++)
    {
        headers.append(QString("num%1").arg(i));
        QTableWidgetItem *item = new QTableWidgetItem (QString("%1").arg(R[i]));
        ui->step->setItem(0,i-1,item);
    }
    ui->step->setHorizontalHeaderLabels(headers);

    /*快速排序core*/
    quicksort(R,1,100);
    ui->textEdit->append(QString("此次运算执行了%1次比较，%2次交换").arg(cmp_Count).arg(move_Count));
}

void Dialog_3::quicksort(int R[],int l,int r)
{
    if(l >= r)  return;
    int i=l, j = r;
    R[0]=R[l];    //暂存基准记录R[i]
    while(i != j)       //从表的两端交替向中间扫描
    {
        while(R[j] >= R[0] && i!=j) {j--;cmp_Count++;};    //从右向左扫描查找第一个小于R[0]的记录;
        while(R[i] <= R[0] && i!=j) {i++;cmp_Count++;};    //从左向右扫描查找第一个大于R[0]的记录;
        if(i == j)  break;      //已排序好结束循环
        ui->textEdit->append(QString("元素%1%2%3%4").arg(R[i]).arg("与").arg(R[j]).arg("进行了交换\n"));     //显示详细步骤
        qSwap(R[i],R[j]);        //交换两个数
        move_Count++;
    }
    qSwap(R[l],R[i]);
    move_Count++;
    /*显示操作位置*/
    ui->textEdit->append(QString("对第%1个元素左右进行排序:").arg(i));
    /*显示结果*/
    int RowCount;        //每执行一轮增加一行
    RowCount = ui->step->rowCount();
    ui->step->insertRow(RowCount);
    /*插入元素*/
    for(int m=1;m<=100;m++)
    {
        ui->step->setItem(RowCount,m-1,new QTableWidgetItem(QString("%1").arg(R[m])));     //将执行一轮后新的排序输出至tablewidget
    }

    /*递归调用*/
    quicksort(R,l,i-1);
    quicksort(R,i+1,r);
}

void Dialog_3::on_pushButton_clicked()
{
    //清空列表
    int len = ui->step->rowCount();
    for(int i=len;i>=1;i--)
        ui->step->removeRow(i);
    //清空文本
    ui->textEdit->clear();
}
