#include "idatabase.h"
#include <QUuid>
#include <QMessageBox>

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    QString aFile="../../lab4.db";
    database.setDatabaseName(aFile);  //设置数据库名称

    if(!database.open()){ //打开数据库
        qDebug() << "failed to open database";
    }else
        qDebug() << "open database is ok" << database.connectionName();
}

bool IDatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this,database);  //数据表
    patientTabModel->setTable("patient");    //设置数据表
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //数据保存方式，OnManualSubmit,OnRowChange
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);  //排序
    if(!(patientTabModel->select()))  //查询数据
        return false;

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());  //在末尾添加一个记录
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount() - 1, 1); //创建最后一行的ModeIndex

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);  //获取当前记录
    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo,curRec);
    patientTabModel->setData(patientTabModel->index(curRecNo, patientTabModel->fieldIndex("CREATEDTIMESTAMP")),
                             QDateTime::currentDateTime().toString("yyyy-MM-dd"));

    return curIndex.row();
}

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();   //获取当前选择单元格索引
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}

bool IDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

QString IDatabase::userLogin(QString userName, QString passWord)
{
    QSqlQuery query;
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString password = query.value("password").toString();
        if(password == passWord){
            qDebug() << "login ok";
            return "loginOK";
        }
        else{
            qDebug() << "wrong password";
            QMessageBox::warning(nullptr,"..","密码错误");
            return "wrongPassword";
        }
    }
    else{
        qDebug() << "no such useer";
        QMessageBox::warning(nullptr,"..","用户名不存在");
        return "wrongUsername";
    }
}

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
