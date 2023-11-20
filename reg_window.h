#ifndef REG_WINDOW_H
#define REG_WINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class Reg_window;
}

class Reg_window : public QDialog
{
    Q_OBJECT

public:
    explicit Reg_window(QWidget *parent = nullptr);
    ~Reg_window();
    QString getName();
    QString getPass();
    bool checkPass();

signals:
    void userRegistered(const QString &username, const QString &password);

private slots:
    void on_loginLine2_textEdited(const QString &arg1);
    void on_passLine2_textEdited(const QString &arg1);
    void on_regButton2_clicked();

private:
    Ui::Reg_window *ui;
    QString m_userName;
    QString m_userPass;
};

#endif // REG_WINDOW_H
