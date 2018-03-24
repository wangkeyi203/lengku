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

#define GLOBAL_CODEC   "UTF-8"
#ifndef Q_OS_WIN
#define DATABASE       "/Users/wangkeyi/Desktop/Qt_project/lengku/lengku/data/data.db"
#else
#define DATABASE       "F:/project/lengku/lengku/data/data.db"
#endif

/*数据库表
 * admin
 * id (int) name(char) passwd(char)
 *
 * worker
 * id (int) name(char) work_num(char) card1~card8(char)
 *
 * list
 * id int,name varchar(30) not null,kind varchar(30),weight1 varchar(30),time1 varchar(30),
 * weight2 varchar(30),time2 varchar(30),percent varchar(30),date varchar(30)
 */

#endif // CONFIG_H
