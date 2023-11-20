#include "reg_window.h"
#include "ui_reg.h"

Reg_window::Reg_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reg_window)
{
    ui->setupUi(this);
}

Reg_window::~Reg_window()
{
    delete ui;
}

void Reg_window::on_loginLine2_textEdited(const QString &arg1)
{
    Reg_window::m_userName = arg1;
}


void Reg_window::on_passLine2_textEdited(const QString &arg1)
{
    Reg_window::m_userPass = arg1;
}


void Reg_window::on_regButton2_clicked()
{
    QString username = getName();
    QString password = getPass();

    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this, "Input error", "Username or password field is empty.");
        return;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Registration failed", "Failed to register the user: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Registration successful", "The user has been registered successfully!");

        if (!QSqlDatabase::database().commit()) {
            qDebug() << "Error committing transaction: " << QSqlDatabase::database().lastError();
        } else {

            emit userRegistered(username, password);
            this->close();
        }
    }

}

QString Reg_window::getName()
{
    return m_userName;
}

QString Reg_window::getPass()
{
    return m_userPass;
}
