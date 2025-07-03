#ifndef GLOBALS_H
#define GLOBALS_H
#include "registering.h"
#include "control2.h"
#include "login.h"
#include "search.h"
#include "control1.h"
#include "recommended.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QUuid>
#include <QMessageBox>
#include <QMap>
#include <QDate>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QtAlgorithms>
extern registering *regForm;
extern control2 *controling2;
extern Login *login1;
extern Search *Searching;
extern control1 *controling1;
extern QString user;
extern recommended *recommending;
#endif // GLOBALS_H
