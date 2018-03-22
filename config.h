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


#define GLOBAL_CODEC   "UTF-8"
#ifndef Q_OS_WIN
#define DATABASE       "/Users/wangkeyi/Desktop/Qt_project/lengku/lengku/data/data.db"
#else
#define DATABASE       "F:/project/lengku/lengku/data/data.db"
#endif

#endif // CONFIG_H
