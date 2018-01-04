# Design with Qt5
## Resume of Qt5
![Qt5 icon](https://github.com/cookchicken/Design-with-Qt5/blob/master/img-folder/d439b6003af33a87a4043fc6c05c10385243b584.jpg)
</br>
Qt has everything you need to quickly and cost-effectively design, develop, test, deploy and maintain your software for any project. Focus on creating the best user experiences instead of coding what’s already been coded for you.
## Resume of topic
Creating 10 groups' data randomly and each group contains 100 elements</br>
Then compare the four Sorting algorithms' performance：Bubble sort,Insertion sort,Quick sort,Heap sort.
## Project's framework
1. MainWindow
2. Database connection
3. Sorting algorithms
## Detail
> Talk is more,show me the code
1. Mainwindow
>mianwindow.h

    class MainWindow : public QMainWindow
    {
      Q_OBJECT

    public:
      explicit MainWindow(QWidget *parent = 0);
      ~MainWindow();

    private slots:
      void on_bubble_clicked();
      void on_linkButton_clicked();

      void on_quitButton_clicked();

      void on_insert_clicked();

      void on_quick_clicked();

      void on_heap_clicked();

    private:
      Ui::MainWindow *ui;
      Dialog *dialog;
      Dialog_2 *dialog_2;
      Dialog_3 *dialog_3;
      Dialog_4 *dialog_4;
      ConnDlg *conndlg;

    };
>>In the .h file, we declared the functions and class:</br>void on_bubble_clicked()+Dialog *dialog to declare bubble sort.</br>void on_insert_clicked()+Dialog_2 *dialog_2 to declare insert sort.</br>void on_quick_clicked()+Dialog_3 *dialog_3 to declare quick sort.</br>void on_heap_clicked()+Dialog_4 *dialog_4 to declare heap sort.</br>void on_linkButton_clicked()+ConnDlg *conndlg to declare Database connection.</br>void on_quitButton_clicked() to quit. 

>mainwindow.cpp

     void MainWindow::on_bubble_clicked()
    {
        dialog = new Dialog("factory");
        dialog->show();
    }
    void MainWindow::on_insert_clicked()
    {
        dialog_2 = new Dialog_2("factory");
        dialog_2->show();
    }
    void MainWindow::on_quick_clicked()
    {
        dialog_3 = new Dialog_3("factory");
        dialog_3->show();
    }
    void MainWindow::on_heap_clicked()
    {
        dialog_4 = new Dialog_4("factory");
        dialog_4->show();
    }
    void MainWindow::on_linkButton_clicked()
    {
        conndlg = new ConnDlg;
        conndlg->show();
    }
    void MainWindow::on_quitButton_clicked()
    {
        close();
    }
>>In the .cpp file, we complete the call functions</br>Take bubble sort as example: First we declare the class as dialog, Then we call the show() function to display the bubble sort interface.
2. Database connection
> connectdlg.h

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
>connectdlg.cpp

    ConnDlg::ConnDlg(QWidget *parent) :
          QDialog(parent)
    {
          ui.setupUi(this);
          QStringList drivers = QSqlDatabase::drivers();    //(a)
          ui.comboDriver->addItems(drivers);                //(b)
          connect(ui.comboDriver,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(driverChanged(const QString &))); //(c)
          ui.status_label->setText(tr("准备链接数据库!"));    //(d)  
    }
>>(a)Search the Database drive and return all the able-drivers' name with QStringList type.</br>
  (b)add these drivers into ui's complex combo.</br>
  (c)Link the combo's SIGNAL currentIndexChanged(const QString &) and SLOT driverChanged(const QString &), in order to as Users select different drivers the SLOT will always be called.
  (d)setting the current status.
  
    void ConnDlg::driverChanged(const QString &text)
    {
        if(text == "QSQLITE")           //(a)
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
 >>(a)due to the QSQLITE SqlDriver's database——sqlite is a kind of Local DB, it not need those characters. So when user choose QSQLITE those characters will be forbid.
 
    void ConnDlg::on_okButton_clicked()
    {
        if(ui.comboDriver->currentText().isEmpty())               //(a)
        {
            ui.status_label->setText(tr("请选择一个数据库驱动!"));
            ui.comboDriver->setFocus();
        }
        else if(ui.comboDriver->currentText() == "QSQLITE")       //(b)
        {
            addSqliteConnection();
            creatDB();                                            //(c)
            accept();       
        }
        else       
        {
            QSqlError err = addConnection(driverName(), databaseName(),hostName(),userName(),password(),port());  //(d)
            if(err.type() != QSqlError::NoError)                  //(e)
                ui.status_label->setText(err.text());
            else                                                  //(f)
                ui.status_label->setText(tr("连接数据库成功"));
            accept();      
        }
    }
>>(a)check whether the user choose one DB driver</br>
  (b)if choose QSQLITE then call the addSqliteConnection() to create a memory-database</br>
  (c)when connect succussed, use SQL para to create DB-table and insert data.</br>
  (d)if choose other DB drivers then call addConnection() to create according connection.</br>
  (e)use QSqlError class to deal relate error.</br>
  (f)display succussful message on status bar.
  
    QSqlError ConnDlg::addConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
    {
        QSqlError err;
        QSqlDatabase db = QSqlDatabase::addDatabase(driver);
        db.setDatabaseName(dbName);
        db.setHostName(host);
        db.setPort(port);
        if(!db.open(user,passwd))         //(a)
        {
            err = db.lastError();
        }
        return err;
    }
>>record the last error and connect the default connection.

    void ConnDlg::creatDB()
    {
        QSqlQuery query;                                                    //(a)
        query.exec("create table factory (id int primary key, num1 int)");     //(b)
        query.prepare("insert into factory values (?,?)");        
        long records = 10;
        for(int i=1;i<=records;i++){      
            query.bindValue(0,i);
            query.bindValue(1,1+rand()%100);
            query.exec();
        }
        for(int j=2;j<=100;j++)     
        {
            query.exec(QString("alter table factory add num%1 int").arg(j));

            for(int i=1;i<=records;i++){      
                query.prepare(QString("update factory set num%1 = ? where id = %2").arg(j).arg(i));
                query.bindValue(0,1+rand()%100);
                query.exec();
            }
        }
    }
>>(a) create QSqlQuery object, once the connection built, you can use the SQL para supported by QSqlQuery.</br>
  (b) take SQL para as parameter of QSqlQuery::exec() to create a 10 Rows 100 Columns table.
3. Sorting algorithms
>bubble.cpp

    Dialog::Dialog(const QString &factoryTable, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog)
    {
        ui->setupUi(this);
        factoryModel = new QSqlTableModel(this);        //(a)
        factoryModel->setTable(factoryTable);
        factoryModel->select();
        ui->source->setSelectionBehavior(QAbstractItemView::SelectRows);        //(b)
        ui->source->setEditTriggers(QAbstractItemView::NoEditTriggers);         //(c)
        ui->source->setModel(factoryModel);

        connect(ui->source,SIGNAL(clicked(QModelIndex )),this,SLOT(to_array(QModelIndex )));
    }
>>(a) create QSqlTableModel model for table factory</br>
  (b) Row Only</br>
  (c) No Edit
  
    void Dialog::to_array(QModelIndex index)
    {
        int cmp_Count=0,move_Count=0;                         
        int R[100];
        QSqlRecord record = factoryModel->record(index.row());      //(a)
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
                cmp_Count++;                    //(b)
                if(R[j]>R[j+1])
                {
                    // add textEdit R[j] transfer with R[j+1]
                    ui->textEdit->append(QString("元素%1%2%3%4").arg(R[j]).arg("与").arg(R[j+1]).arg("进行了交换\n"));     //(c)
                    R[0]=R[j];
                    R[j]=R[j+1];
                    R[j+1]=R[0];
                    swap = 1;
                    move_Count++;           //(d)
                }
            }
            // 显示第i步结果
            ui->tableWidget->insertRow(i);        //(e)
            for(int m=1;m<=100;m++)
            {
                ui->tableWidget->setItem(i,m-1,new QTableWidgetItem(QString("%1").arg(R[m])));       //(f)
            }
            if(swap==0)
                break;
        }
        ui->textEdit->append(QString("此次运算执行了%1次比较，%2次交换").arg(cmp_Count).arg(move_Count));
    }
>>(a) extract the row-selected data to array</br>
  (b) compare Count +1</br>
  (c) display the exchange detail</br>
  (d) move Count +1</br>
  (e) every execution the Row +1</r>
  (f) output the every step sorting result to tablewidget </br>
> other 3 sort algorithms frame are familiar with the bubble sort, no more explanation here.</br>
  *Caution:</br>*
          *quick sort and heap sort need to declare the cmp_Count and move_Count as global variable.*
 ##Program screenshot
You can find all the interface screenshots in image file. XD..
