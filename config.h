#ifndef CONFIG_H
#define CONFIG_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCodec>
#include <qsqltablemodel>
#include <QModelIndex>
#include <QStandardItem>
#include <QMessageBox>
#include <QHash>
#include <QTcpServer>
#include <QSqlTableModel>
#include <QDateTime>
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTextEdit>
#include <QTcpServer>
#include <QTcpSocket>

#define GLOBAL_CODEC   "UTF-8"
#ifndef Q_OS_WIN
#define DATABASE       "/Users/wangkeyi/Desktop/Qt_project/lengku/lengku/data/data.db"
#else
#define DATABASE       "D:/lengku/data.db"
#endif

/*数据库表
 * admin
 * id (int) name(char) passwd(char)
 *
 * worker
 * id (int) name(char) flag (int) lisdid(int) card1~card7(vchar)
 *
 * list_date
 * id (int) name(vchar) weight1 (vchar) num1(vchar) weight2(vchar) num2(char) persent(vchar) date(vchar)
 *
 * list
 * id int,name(vchar) not null,kind (vchar),weight1 (vchar),time1 (vchar),
 * weight2 (vchar),time2 (vchar),percent (vchar),date (vchar)
 *
 *
 * kind
 * id int,name vchar ,price vchar
 *
 * kind_now
 * id int,kind int
 */

#endif // CONFIG_H
