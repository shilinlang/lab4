#include "patieneditview.h"
#include "ui_patieneditview.h"
#include "idatabase.h"

PatienEditView::PatienEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::PatienEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabMode = IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID,tabMode->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName,tabMode->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDcard,tabMode->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinHeight,tabMode->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinWeight,tabMode->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile,tabMode->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDateEditDOB,tabMode->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbComboSex,tabMode->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreatedTimeStamp,tabMode->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(index);
}

PatienEditView::~PatienEditView()
{
    delete ui;
}

void PatienEditView::on_btSave_clicked()
{
    IDatabase::getInstance().submitPatientEdit();

    emit goPreviousView();
}


void PatienEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

