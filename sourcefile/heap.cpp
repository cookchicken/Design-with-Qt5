#include "heap.h"
#include "ui_heap.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
#include <QSqlRecord>
#include <QAbstractItemView>



Dialog_4::Dialog_4(const QString &factoryTable,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_4)
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

Dialog_4::~Dialog_4()
{
    delete ui;
}

void Dialog_4::to_array(QModelIndex index)
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

    /*堆排序core*/
    HeapSort(R,100);
}

void Dialog_4::HeapAdjust(int R[], int s, int t)
{
    /*对R[s]~R[t]除R[s]外均满足堆的定义，即只对R[s]进行调整，使R[s]为根的完全二叉树成为一个堆*/
    int i,j;
    R[0]=R[s];      //R[s]暂存于R[0]
    i=s;
    for(j=2*i;j<=t;j=2*j)       //沿关键字较大的孩子向下调整，先假定为左孩子
    {
        cmp_Count++;
        if(j<t && R[j]<R[j+1])
            j=j+1;      //右孩子结点的关键字大，则沿右孩子向下调整
        if(R[0]>R[j])       //R[0]的关键字已大于R[j]的关键字值，即以满足堆的定义，故不再向下调整
            break;
        R[i]=R[j];      //将关键字大的孩子节点R[j]调整至双亲结点R[i]
        move_Count++;
        i=j;        //定位于孩子节点继续向下调整
    }
    R[i]=R[0];     // 找到满足堆定义的R[0]放置位置i，将R[s]调整于此
}

void Dialog_4::HeapSort(int R[], int n)
{
    int i;
    for(i=n/2;i>0;i--)
        HeapAdjust(R,i,n);
    for(i=n;i>1;i--)
    {
        ui->textEdit->append(QString("第%1趟结果:").arg(n-i+1));        // 显示趟数
        ui->textEdit->append(QString("元素%1%2%3%4").arg(R[0]).arg("与").arg(R[i]).arg("进行了交换\n"));     //显示详细步骤
        R[0]=R[1];
        R[1]=R[i];
        R[i]=R[0];

        /*显示第i步结果*/
        int RowCount;        //每执行一轮增加一行
        RowCount = ui->step->rowCount();
        ui->step->insertRow(RowCount);
        /*插入元素*/
        for(int m=1;m<=100;m++)
        {
            ui->step->setItem(RowCount,m-1,new QTableWidgetItem(QString("%1").arg(R[m])));     //将执行一轮后新的排序输出至tablewidget
        }

        HeapAdjust(R,1,i-1);
    }
    ui->textEdit->append(QString("此次运算执行了%1次比较，%2次交换").arg(cmp_Count).arg(move_Count));
}

void Dialog_4::on_pushButton_clicked()
{
    //清空列表
    int len = ui->step->rowCount();
    for(int i=len;i>=1;i--)
        ui->step->removeRow(i);
    //清空文本
    ui->textEdit->clear();
}
