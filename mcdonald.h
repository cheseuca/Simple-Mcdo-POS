
#ifndef MCDONALD_H
#define MCDONALD_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>


QT_BEGIN_NAMESPACE
namespace Ui { class Mcdonald; }
QT_END_NAMESPACE

class Mcdonald : public QMainWindow

{
    Q_OBJECT

public:
    Mcdonald(QWidget *parent = nullptr);
    ~Mcdonald();
    double price;
    double getProductSubtotal(const QString &productName, int quantity);


private slots:
    void on_bigmac_clicked();

    void on_compute_clicked();

    void on_newt_clicked();

    void on_qpounder_clicked();

    void on_dpounder_clicked();

    void on_esausage_clicked();

    void on_cburger_clicked();

    void on_hamdesal_clicked();

    void on_pancake_clicked();

    void on_softdrink_clicked();

    void on_fries_clicked();

    void on_sundae_clicked();

    void on_pay_clicked();


private:
    Ui::Mcdonald *ui;

    int bigmacqty = 0;
    int qpounderqty= 0;
    int dpounderqty = 0;
    int esausageqty = 0;
    int cburgerqty = 0;
    int hamdesalqty = 0;
    int pancakeqty = 0;
    int friesqty = 0;
    int softdrinkqty = 0;
    int sundaeqty = 0;

    double total = 0.0;
    double subtotal = 0.0;
    double tax= 0.12;
    double discount = 0.0;

};

#endif // MCDONALD_H
