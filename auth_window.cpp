#include "auth_window.h"
#include "ui_auth.h"

#define Path_to_DB "C:/Users/MSI Prestige/Documents/sqlite_db/fit_db.sqlite"

Auth_window::Auth_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Auth_window)
{
    ui->setupUi(this);



    if (!QFile::exists(Path_to_DB)) {
        qDebug() << "No database found at " << Path_to_DB;
        return;
    }
    myDB = QSqlDatabase::addDatabase("QSQLITE");
    myDB.setDatabaseName(Path_to_DB);
    if (!myDB.open()) {
        QMessageBox::critical(nullptr, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return;
    }

}

Auth_window::~Auth_window()
{
    delete ui;
    myDB.close();
}

void Auth_window::on_passLine_textEdited(const QString &arg1)
{
    Auth_window::m_userpass = arg1;
}


void Auth_window::on_loginLine_textEdited(const QString &arg1)
{
    Auth_window::m_username = arg1;
}


void Auth_window::on_loginButton_clicked()
{
    QSqlQuery query;
    QString username = getLogin();
    QString userpass = getPass();

    if (query.exec(QString("SELECT * FROM users WHERE username='%1' AND password='%2'")
                       .arg(username).arg(userpass))) {
        if (query.next()) {
            accept();
        } else {
            QMessageBox::warning(this, "Login failed", "Incorrect username or password.");
        }
    } else {
        QMessageBox::critical(this, "SQL Error", query.lastError().text());
    }
}


void Auth_window::on_regButton_clicked()
{
    Reg_window *reg_window = new Reg_window(this);
    connect(reg_window, &Reg_window::userRegistered, this, &Auth_window::automaticLogin);
    reg_window->setAttribute(Qt::WA_DeleteOnClose); // Set the window to be deleted on close
    reg_window->exec();
}

QString Auth_window::getLogin()
{
    return Auth_window::m_username;
}

QString Auth_window::getPass()
{
    return Auth_window::m_userpass;
}

void Auth_window::automaticLogin(const QString &username, const QString &password) {
    // Assuming you have ui->usernameEdit and ui->passwordEdit QLineEdit fields.
    m_username = username;
    m_userpass = password;

    on_loginButton_clicked();
}
