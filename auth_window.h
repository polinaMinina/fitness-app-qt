#ifndef AUTH_WINDOW_H
#define AUTH_WINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <reg_window.h>

namespace Ui {
class Auth_window;
}

class Auth_window : public QDialog
{
    Q_OBJECT

public:
    explicit Auth_window(QWidget *parent = nullptr);
    ~Auth_window();
    QString getLogin();
    QString getPass();


private slots:
    void on_passLine_textEdited(const QString &arg1);
    void on_loginLine_textEdited(const QString &arg1);
    void on_loginButton_clicked();
    void on_regButton_clicked();
    void automaticLogin(const QString &username, const QString &password);

private:
    Ui::Auth_window *ui;
    QString m_username;
    QString m_userpass;
    QSqlDatabase myDB;
};

#endif // AUTH_WINDOW_H
