#ifndef PATIENEDITVIEW_H
#define PATIENEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class PatienEditView;
}

class PatienEditView : public QWidget
{
    Q_OBJECT

public:
    explicit PatienEditView(QWidget *parent = nullptr,int index = 0);
    ~PatienEditView();

private slots:
    void on_btSave_clicked();

    void on_btCancel_clicked();

private:
    Ui::PatienEditView *ui;
    QDataWidgetMapper *dataMapper;   //数据映射

signals:
    void goPreviousView();

};

#endif // PATIENEDITVIEW_H
