#include "list_date.h"
#include "ui_list_date.h"

List_date::List_date(QSqlDatabase &db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List_date)
{
    ui->setupUi(this);
    this->db= db;
    tablemodel = new QSqlTableModel(this);
    tablemodel->setTable("list_date");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->select();
    tablemodel->setHeaderData(0,Qt::Orientation::Horizontal,"编号");
    tablemodel->setHeaderData(1,Qt::Orientation::Horizontal,"姓名");
    tablemodel->setHeaderData(2,Qt::Orientation::Horizontal,"毛料");
    tablemodel->setHeaderData(3,Qt::Orientation::Horizontal,"数量");
    tablemodel->setHeaderData(4,Qt::Orientation::Horizontal,"半成品");
    tablemodel->setHeaderData(5,Qt::Orientation::Horizontal,"数量");
    tablemodel->setHeaderData(6,Qt::Orientation::Horizontal,"出成");
    tablemodel->setHeaderData(7,Qt::Orientation::Horizontal,"日期");
    //tablemodel->setHeaderData(8,Qt::Orientation::Horizontal,"日期");
    ui->tableView->setModel(tablemodel);
}

List_date::~List_date()
{
    delete ui;
}
//#ifdef Q_OS_WIN
void List_date::on_export_xls_clicked()
{

    //QString fileName = QFileDialog::getSaveFileName(ui->tableView, "保存",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel 文件(*.xls *.xlsx)");

    QString filepath = QFileDialog::getSaveFileName(this, tr("保存到..."),QString(),tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.txt);;"));
    int row = ui->tableView->model()->rowCount();
    int col = ui->tableView->model()->columnCount();
    QList<QString> list;
        //添加列标题
    QString HeaderRow;
    for(int i=0;i<col;i++)
    {
       // HeaderRow.append(table->horizontalHeaderItem(i)->text()+"\t");
        HeaderRow.append(ui->tableView->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString() +"\t");
    }
    list.push_back(HeaderRow);
    qDebug()<<row;
    qDebug()<<col;
    for(int i=0;i<row;i++)
        {
            QString rowStr = "";
            for(int j=0;j<col;j++){
                qDebug()<< ui->tableView->model()->index(i,j).data().toString();
                rowStr += ui->tableView->model()->index(i,j).data().toString() +"\t";
                //rowStr += table->item(i,j)->text() + "\t";
            }
            list.push_back(rowStr);
        }
    QTextEdit textEdit;
        for(int i=0;i<list.size();i++)
        {
            textEdit.append(list.at(i));
        }

        QFile file(filepath);
        if(file.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("GBK");
            ts<<textEdit.document()->toPlainText();
            file.close();
        }
}
//#endif
