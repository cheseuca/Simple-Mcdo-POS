#include "mcdonald.h"
#include "ui_mcdonald.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMainWindow>



Mcdonald::Mcdonald(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Mcdonald)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./mcdonalds_product_price.db"); //Need to be hard coded to work, use the actual location of the db file in your computer

    if (!db.open()){
        qDebug() << "Connection to database not established";
    } else {
        qDebug() << "Connection to database is established";
    }
}


Mcdonald::~Mcdonald()
{
    delete ui;
}


void Mcdonald::on_compute_clicked()
{
    double totaldue = ui->total->text().toDouble();
    double payment = ui->amount->text().toDouble();
    double changes = payment - totaldue;
    ui->change->setText(QString::number(changes));
}


void Mcdonald::on_newt_clicked()
{
    bigmacqty = 0;
    qpounderqty = 0;
    dpounderqty = 0;
    esausageqty = 0;
    cburgerqty = 0;
    hamdesalqty = 0;
    pancakeqty = 0;
    friesqty = 0;
    softdrinkqty = 0;
    sundaeqty = 0;
    total = 0;
    subtotal = 0;
    tax = 0.12;
    discount = 0;

    ui->senior->setChecked(false);
    ui->employee->setChecked(false);

    ui->sales->clear();
    ui->amount->clear();
    ui->change->clear();
    ui->stotal->clear();
    ui->tax->clear();
    ui->dcount->clear();
    ui->total->clear();

}

void Mcdonald::on_pay_clicked()
{
    double subtotal = total;
    double Tax = 0.12;
    double Discount = 0.0;

    // Check if senior discount checkbox is checked
    if(ui->senior->isChecked()){
        Tax = 0.0;  // No tax for seniors
        Discount = 0.12;  // 12% senior discount
    }

    // Check if employee discount checkbox is checked
    if(ui->employee->isChecked()){
        Discount = 0.03;  // 3% employee discount
    }

    //Get the subtotal of each product that were ordered
    subtotal += getProductSubtotal("BigMac Burger", bigmacqty);
    subtotal += getProductSubtotal("Quarter Pounder", qpounderqty);
    subtotal += getProductSubtotal("Double Pounder", dpounderqty);
    subtotal += getProductSubtotal("CheeseBurger", cburgerqty);
    subtotal += getProductSubtotal("Egg Sausage", esausageqty);
    subtotal += getProductSubtotal("Hamdesal", hamdesalqty);
    subtotal += getProductSubtotal("Pancake", pancakeqty);
    subtotal += getProductSubtotal("Softdrinks", softdrinkqty);
    subtotal += getProductSubtotal("French Fries", friesqty);
    subtotal += getProductSubtotal("Hot Fudge", sundaeqty);

    // Calculate discounts and taxes
    double discounts = subtotal * Discount;
    double taxes  = (subtotal - discounts) * Tax;

    // Calculate total due
    double alltotaldue = (subtotal + taxes) - discounts;


    ui->stotal->setText(QString::number(subtotal));
    ui->tax->setText(QString::number(taxes));
    ui->dcount->setText(QString::number(discounts));
    ui->total->setText(QString::number(alltotaldue));

    //Display the summary once the order is finalized
    QString summary = "Ordered Products:\n";
    if (bigmacqty > 0) {
        double bigmacSubtotal = getProductSubtotal("BigMac Burger", bigmacqty);
        summary += "Big Mac x " + QString::number(bigmacqty) + " = PHP " + QString::number(bigmacSubtotal, 'f', 2)+"\n";
    }
    if (qpounderqty > 0) {
        double quarterpoundSubtotal = getProductSubtotal("Quarter Pounder", qpounderqty);
        summary += "Quarter Pounder x " + QString::number(qpounderqty) + " = PHP " + QString::number(quarterpoundSubtotal, 'f', 2)+"\n";
    }
    if (dpounderqty > 0) {
        double doublepoundSubtotal = getProductSubtotal("Double Pounder", dpounderqty);
        summary += "Double Pounder x " + QString::number(dpounderqty) + " = PHP " + QString::number(doublepoundSubtotal, 'f', 2)+"\n";
    }
    if (cburgerqty > 0) {
        double cheesburgerSubtotal = getProductSubtotal("CheeseBurger", cburgerqty);
        summary += "CheeseBurger x " + QString::number(cburgerqty) + " = PHP " + QString::number(cheesburgerSubtotal, 'f', 2)+"\n";
    }
    if (esausageqty > 0) {
        double eggsausageSubtotal = getProductSubtotal("Egg Sausage", esausageqty);
        summary += "Egg Sausage x " + QString::number(esausageqty) + " = PHP " + QString::number(eggsausageSubtotal, 'f', 2)+"\n";
    }
    if (hamdesalqty > 0) {
        double hamdesalSubtotal = getProductSubtotal("Hamdesal", hamdesalqty);
        summary += "Hamdesal x " + QString::number(hamdesalqty) + " = PHP " + QString::number(hamdesalSubtotal, 'f', 2)+"\n";
    }
    if (pancakeqty > 0) {
        double pancakeSubtotal = getProductSubtotal("Pancake",pancakeqty);
        summary += "Pancake x " + QString::number(pancakeqty) + " = PHP " + QString::number(pancakeSubtotal, 'f', 2)+"\n";
    }
    if (softdrinkqty > 0) {
        double softdrinkSubtotal = getProductSubtotal("Softdrinks", softdrinkqty);
        summary += "Softdrink x " + QString::number(softdrinkqty) + " = PHP " + QString::number(softdrinkSubtotal, 'f', 2)+"\n";
    }
    if (friesqty > 0) {
        double friesSubtotal = getProductSubtotal("French Fries", friesqty);
        summary += "French Fries x " + QString::number(friesqty) + " = PHP " + QString::number(friesSubtotal, 'f', 2)+"\n";
    }
    if (sundaeqty > 0) {
        double sundaeSubtotal = getProductSubtotal("Hot Fudge", sundaeqty);
        summary += "Hot Fudge x " + QString::number(sundaeqty) + " = PHP " + QString::number(sundaeSubtotal, 'f', 2)+"\n";
    }

    summary += "\nSubtotal: PHP " + QString::number(subtotal, 'f', 2) + "\n";
    summary += "VAT Tax: PHP " + QString::number(taxes, 'f', 2) + "\n";
    summary += "Discount: PHP " + QString::number(discounts, 'f', 2) + "\n";
    summary += "Total Due: PHP " + QString::number(alltotaldue, 'f', 2) + "\n";

    ui->sales->setText(summary);
}

//Created a function to auto calculate the subtotal of each product
double Mcdonald::getProductSubtotal(const QString &productName, int quantity)
{
    QSqlQuery query;
    QString queryString = "SELECT PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_NAME = :name";
    query.prepare(queryString);
    query.bindValue(":name", productName);
    if (query.exec() && query.next()) {
        double price = query.value(0).toDouble();
        return price * quantity;
    } else {
        qDebug() << "Error retrieving price for product" << productName;
        return 0.0;
    }
}


void Mcdonald::on_bigmac_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 1");
            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                bigmacqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(bigmacqty)+"\n");

            }

}

void Mcdonald::on_qpounder_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 2");
            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                qpounderqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                   "\nProduct Quantity: " +QString::number(qpounderqty)+"\n");
            }
}

void Mcdonald::on_dpounder_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 3");
            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                dpounderqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(dpounderqty)+"\n");
            }
}

void Mcdonald::on_esausage_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 5");
            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                esausageqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(esausageqty)+"\n");
            }
}

void Mcdonald::on_cburger_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 4");
            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                cburgerqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(cburgerqty)+"\n");
           }
}

void Mcdonald::on_hamdesal_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 6");

            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                hamdesalqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(hamdesalqty)+"\n");
            }
}

void Mcdonald::on_pancake_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 7");

            while (query.next()) {

                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                pancakeqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(pancakeqty)+"\n");
            }
}

void Mcdonald::on_softdrink_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 8");

            while (query.next()) {

            QString pdname = query.value(0).toString();
            double pdprice = query.value(1).toDouble();
            softdrinkqty += 1;
            //Display the product and the quantity ordered
            ui->sales->append("Product Name: " + pdname
                          + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                          "\nProduct Quantity: " +QString::number(softdrinkqty)+"\n");
            }
}

void Mcdonald::on_fries_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 9");
            while (query.next()) {
                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                friesqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(friesqty)+"\n");
            }
}

void Mcdonald::on_sundae_clicked()
{
    //Obtain the product price in the database
    QSqlQuery query;
        query.exec("SELECT PRODUCT_NAME, PRODUCT_PRICE FROM mcdopos WHERE PRODUCT_ID = 10");
            while (query.next()) {
                QString pdname = query.value(0).toString();
                double pdprice = query.value(1).toDouble();
                sundaeqty += 1;
                //Display the product and the quantity ordered
                ui->sales->append("Product Name: " + pdname
                                  + "\nProduct Price: " + QString::number(pdprice,'f',2)+
                                  "\nProduct Quantity: " +QString::number(sundaeqty)+"\n");
            }
}

