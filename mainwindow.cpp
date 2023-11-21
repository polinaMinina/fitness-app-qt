#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "chart.h"
#include <QDebug>
#include "statistics.h"
#include "sqlite.h"
#include "Calendar.h"
#include "profiler.h"
#include "dumbellframe.h"
#include "clickablecross.h"
#include "darkpalette.h"
#include "lightpalette.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::Window | Qt::FramelessWindowHint),
    ui(new Ui::MainWindow), dx( 0 ), dy( 0 ), isDarkTheme(true)
{
    ui->setupUi(this);
    ui->textBrowserWeight->hide();
    ui->lbTitle->setText(windowTitle());
    ui->calendarLabel->setText(calendarMonth.toUpper());

    connect(ui->btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);
    connectToSqlDatabase();

    frame = createCalendarDialogFrame();
    calendar = Calendar();
    calendarMonth = calendar.getCurrentMonth();

    createCalendar(calendarMonth);
    highlightCurrentDay();

    QPushButton* btnLeft = new QPushButton();
    btnLeft->setMaximumSize(20,20);
    btnLeft->setText("<");

    QPushButton* btnRight = new QPushButton();
    btnRight->setMaximumSize(20,20);
    btnRight->setText(">");

    QLabel* pages = new QLabel();
    pages->setText("<u><b>1</b1></u> 2 3 4 5");

    QFont fontForPages;
    fontForPages.setPixelSize(15);
    pages->setFont(fontForPages);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    ui->centralwidget->setLayout(mainLayout);


    QHBoxLayout* dumButtons = new QHBoxLayout();
    dumButtons->addWidget(btnLeft);
    dumButtons->addWidget(pages, 10, Qt::AlignCenter);
    dumButtons->addWidget(btnRight);

    dumbellFrame* dumbell = new dumbellFrame();
    QFrame* dumbellFrame = dumbell->createDumbellFrameEmpty();
    connect(dumbell->crossImage, &clickableCross::clicked, this, &MainWindow::createDumbellFrame);

    QVBoxLayout* pageLayout = new QVBoxLayout();
    dumbellsLayout = new QGridLayout();
    pageLayout->addLayout(dumbellsLayout);
    pageLayout->addLayout(dumButtons);
    ui->page_2->setLayout(pageLayout);
    dumbellsLayout->setAlignment(Qt::AlignTop);
    dumbellsLayout->addWidget(dumbellFrame, dumbellFramesCounterRows, dumbellFramesCounterCols);

    connect(ui->changeThemeButton, &QPushButton::clicked, this, &MainWindow::changeTheme);

}


QFrame* MainWindow::createDumbellFrame(QMouseEvent* e){
    e->ignore();
    if (dumbellFramesCounterCols == 0 && dumbellFramesCounterRows == 0){
        g.first = pos().x();
        g.second = pos().y();
    }
    dumbellFrame* dumbell = new dumbellFrame();
    QFrame* dumbellFrame = dumbell->createDumbellFrame();

    if (dumbellFramesCounterCols != 3){
        ++dumbellFramesCounterCols;
    }
    else {
        dumbellFramesCounterCols = 0;
        ++dumbellFramesCounterRows;
    }
    if (dumbellFramesCounterCols == 0 && dumbellFramesCounterRows == 2){
        ui->page_2->hide();
        ui->page->show();
    }
    dumbellsLayout->addWidget(dumbellFrame, dumbellFramesCounterRows, dumbellFramesCounterCols);
    return dumbellFrame;
}


void MainWindow::highlightCurrentDay()
{
    int day = calendar.getCurrentDay();
    int row = day / 7;
    int col = day - 7 * row;
    if (col == 0){
        col = 7;
        --row;
    }
    QColor color(Qt::white);
    color.setAlpha(30);
    ui->tableWidget->item(row, col - 1)->setBackground(color);
}


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    if(e->buttons() & Qt::LeftButton) {
        QPoint globalCursorPos = e->globalPosition().toPoint();

        int newX = globalCursorPos.x() - dx;
        int newY = globalCursorPos.y() - dy;

        setGeometry(
            newX,
            newY,
            width(),
            height()
            );
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton) {
        QPoint globalCursorPos = e->globalPosition().toPoint();

        dx = globalCursorPos.x() - this->x();
        dy = globalCursorPos.y() - this->y();

        setCursor(Qt::ArrowCursor);
    }
}


void MainWindow::connectToSqlDatabase()
{
    this->sql = SQLite();
    sql.connectToSql();
}


void MainWindow::addToCalc(QLineEdit* line, QTextBrowser* textBrowser, int& calcValue)
{
    int value = line->text().toInt();
    if (value <= 0) {
        QMessageBox::warning(this, "Error", "You must type only positive integers");
        return;
    }

    calcValue += value;
    QString text = QString("Added %1 to the table. Total amount: %2.").arg(value).arg(calcValue);
    textBrowser->setText(text);

    qDebug() << text;
}


void MainWindow::addDataToSql(const QString& typeOfTrain, QTextBrowser* textBrowser, int& calcValue)
{
    if (!userAddedValue(calcValue)){
        QMessageBox::warning(this, "Error", "You must add value first!");
        return;
    }
    else if (calcValue == 0){
        QMessageBox::warning(this, "Error", "You must type only integer values");
        return;
    }
    textBrowser->setText(sql.insertDataToSql(typeOfTrain, calcValue));
    calcValue = 0;
}


bool MainWindow::userAddedValue(int& calcValue)
{
    return calcValue;
}


void MainWindow::showChart(QChart* chart, QChartView* chartView)
{
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}


void MainWindow::cleanTable(QTextBrowser* textBrowser, int& calcValue)
{
    calcValue = 0;
    QString text = QString("The table was successfully cleaned. Total amount: %1.").arg(calcValue);
    textBrowser->setText(text);
    qDebug() << text;
}


void MainWindow::on_Add_weight_clicked()
{
    addToCalc(ui->lineEdit_weight, ui->textBrowser_weight_2, calcValueWeight);
    if (calcValueWeight != 0)
        addDataToSql("weight", ui->textBrowser_weight_2, calcValueWeight);
}


void MainWindow::on_Add_pushups_clicked()
{
    addDataToSql("pushups", ui->textBrowser_pushups, calcValuePushups);
}


void MainWindow::on_Add_pullups_clicked()
{
    addDataToSql("pullups", ui->textBrowser_pullups, calcValuePullups);
}


void MainWindow::on_Add_squats_clicked()
{
    addDataToSql("squats", ui->textBrowser_squats, calcValueSquats);
}


void MainWindow::on_Add_dips_clicked()
{
    addDataToSql("dips", ui->textBrowser_dips, calcValueDips);
}


void MainWindow::on_show_weight_clicked()
{
    QChart* chart = createChart(sql, "weight", this->isDarkTheme);
    showChart(chart, ui->chartW);
}


void MainWindow::on_show_pushups_clicked()
{
    QChart* chart = createChart(sql, "pushups", this->isDarkTheme);
    showChart(chart, ui->chartW_2);
}


void MainWindow::on_show_pullups_clicked()
{
    QChart* chart = createChart(sql, "pullups", this->isDarkTheme);
    showChart(chart, ui->chartW_3);
}


void MainWindow::on_show_squats_clicked()
{
    QChart* chart = createChart(sql, "squats", this->isDarkTheme);
    showChart(chart, ui->chartW_4);
}


void MainWindow::on_show_dips_clicked()
{
    QChart* chart = createChart(sql, "dips", this->isDarkTheme);
    showChart(chart, ui->chartW_dips);
}


void MainWindow::on_AddPushupsToCalc_clicked()
{
    addToCalc(ui->lineEdit_pushups, ui->textBrowser_pushups, calcValuePushups);
}


void MainWindow::on_AddPullupsToCalc_clicked()
{
    addToCalc(ui->lineEdit_pullups, ui->textBrowser_pullups, calcValuePullups);
}


void MainWindow::on_AddSquatsToCalc_clicked()
{
    addToCalc(ui->lineEdit_squats, ui->textBrowser_squats, calcValueSquats);
}


void MainWindow::on_AddDipsToCalc_clicked()
{
    addToCalc(ui->lineEdit_dips, ui->textBrowser_dips, calcValueDips);
}


void MainWindow::on_clear_squats_clicked()
{
    cleanTable(ui->textBrowser_squats, this->calcValueSquats);
}


void MainWindow::on_clear_pullups_clicked()
{
    cleanTable(ui->textBrowser_pullups, this->calcValuePullups);
}


void MainWindow::on_clear_pushups_clicked()
{
    cleanTable(ui->textBrowser_pushups, this->calcValuePushups);
}


void MainWindow::on_clean_dips_clicked()
{
    cleanTable(ui->textBrowser_dips, this->calcValueDips);
}


void MainWindow::on_pushButton_showStatics_clicked()
{
    auto table = sql.getWeightFromSql();
    int mean = meanStat(table);
    int SD = standartDeviation(table, mean);
    int dynamic = dynamicStat(table);

    QList<int> stableWeightTable = QList<int>();

    for (auto it = table.rbegin(); stableWeightTable.size() != 3; ++it){
        if (it == table.rend()) break;
        stableWeightTable.append(*it);
    }
    int stableWeight = meanStat(stableWeightTable);

    QMap<QString, int> PCF {{"Protein", stableWeight*1.5},
                           {"Carbo", stableWeight*3},
                           {"Fat", stableWeight}};

    QString text = QString("STATISTICS:\n"
                           "Mean: %1.\nStandart Deviation: %2.\n"
                           "Dynamic: %3.\nLast added weight: %8.\n"
                           "Stable weight: %4.\n\n"
                           "NUTRIENTS:\n"
                           "Protein: %5.\nCarbo: %6.\nFat: %7.\n ")
                       .arg(mean).arg(SD).arg(dynamic).arg(stableWeight)
                       .arg(PCF["Protein"]).arg(PCF["Carbo"]).arg(PCF["Fat"]).arg(table.last());

    if (ui->textBrowserWeight->isVisible())
        ui->textBrowserWeight->hide();
    else
        ui->textBrowserWeight->show();

    ui->textBrowserWeight->setText(text);
}


void MainWindow::on_btnHide_clicked()
{
    showMinimized();
}


void MainWindow::on_btnResize_clicked()
{
    if (isFullScreen())
        showNormal();
    else
        showFullScreen();
}


void MainWindow::createCalendar(const QString& month)
{
    QFont font;
    font.setBold(true);
    ui->calendarLabel->setText(month.toUpper());
    QSqlQuery query = sql.readCalendarFromSql(calendarMonth);

    QString text;
    int row = 0;
    int col = 0;

    while(query.next()){
        text = query.value(0).toString();
        row = query.value(1).toInt();
        col = query.value(2).toInt();

        QTableWidgetItem* item = new QTableWidgetItem();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(font);
        item->setText(QString("%1").arg(text));

        ui->tableWidget->setItem(row, col, item);
    }

    for(int i = 1, row = 0, col = 0; i < calendar.getMonthsMap()[calendarMonth] + 1; ++i){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(font);
        item->setText(QString("%1\n\n-").arg(i));

        QTableWidgetItem* checkItem = ui->tableWidget->item(row, col);

        if(!checkItem){
            ui->tableWidget->setItem(row, col, item);
        }
        if(col == 6){
            ++row;
            col = 0;
        } else {
            ++col;
        }
    }
    if(month == calendar.getCurrentMonth()){
        highlightCurrentDay();
    }
}


QFrame* MainWindow::createCalendarDialogFrame()
{
    frame = new QFrame();
    frame->setWindowFlag(Qt::FramelessWindowHint);
    frame->setFrameShape(QFrame::StyledPanel);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setMaxLength(200);

    QFont font;
    font.setPixelSize(17);

    QLabel *label = new QLabel();
    label->setText("Type the name of the training");
    label->setFont(font);

    QPushButton* btn = new QPushButton();
    btn->setText("Submit");
    btn->setMaximumWidth(100);
    QPushButton* quitBtn = new QPushButton();
    quitBtn->setText("Close");
    quitBtn->setMaximumWidth(100);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *layout1 = new QHBoxLayout();

    layout1->addWidget(btn);
    layout1->addWidget(quitBtn);

    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->setAlignment(label, Qt::AlignCenter);
    layout->setAlignment(lineEdit, Qt::AlignCenter);
    layout->addLayout(layout1);

    frame->setLayout(layout);
    frame->resize(300,150);
    frame->setWindowTitle("Add training");

    connect(lineEdit, &QLineEdit::textChanged, this, &MainWindow::getTextFromFrame);
    connect(quitBtn, &QPushButton::clicked, frame, &QFrame::hide);
    connect(btn, &QPushButton::clicked, this, &MainWindow::updateCalendarItem);

    profiler.finishMeasuring();
    return frame;
}


void MainWindow::getTextFromFrame(QString str)
{
    frameText = str;
}


void MainWindow::updateCalendarItem()
{
    QString tableWidgetText = ui->tableWidget->item(rowAndCol.first, rowAndCol.second)->text();
    QString dayNumbers;

    QFont font;
    font.setBold(true);

    QTableWidgetItem* item = new QTableWidgetItem();

    for (auto& i : tableWidgetText){
        if(i.isDigit())
            dayNumbers += i;
    }
    if (frameText.length() > 15){
        dayNumbers += "\n" + frameText;
    }
    else {
        dayNumbers += "\n\n" + frameText;
    }
    qDebug() << dayNumbers;

    item->setText(dayNumbers);
    item->setFont(font);
    item->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(rowAndCol.first, rowAndCol.second, item);

    frame->hide();

    sql.updateSqlCalendar(calendarMonth, dayNumbers, rowAndCol.first, rowAndCol.second);
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    int days = calendar.getMonthsMap()[calendarMonth];
    if((row * 7 + column + 1) > days){
        return;
    }
    frame->hide();
    rowAndCol = {row, column};
    frame->show();
}


void MainWindow::on_toolButton_clicked()
{
    calendarMonth = calendar.getNextMonth(calendarMonth);
    ui->tableWidget->clear();
    createCalendar(calendarMonth);
    ui->tableWidget->update();
}


void MainWindow::on_toolButton_2_clicked()
{
    calendarMonth = calendar.getPreviousMonth(calendarMonth);
    ui->tableWidget->clear();
    createCalendar(calendarMonth);
    ui->tableWidget->update();
}


void MainWindow::on_dips_tabBarClicked(int index)
{
    QChart* chart;
    switch (index) {
    case 0:
        chart = createChart(sql, "weight", this->isDarkTheme);  // change
        showChart(chart, ui->chartW);
        break;
    case 1:
        chart = createChart(sql, "pushups", this->isDarkTheme);
        showChart(chart, ui->chartW_2);
        break;
    case 2:
        chart = createChart(sql, "pullups", this->isDarkTheme);
        showChart(chart, ui->chartW_3);
        break;
    case 3:
        chart = createChart(sql, "squats", this->isDarkTheme);
        showChart(chart, ui->chartW_4);
        break;
    case 4:
        chart = createChart(sql, "dips", this->isDarkTheme);
        showChart(chart, ui->chartW_dips);
        break;
    }
}

void MainWindow::changeTheme() {
//    static bool isDarkTheme = true;
    if (this->isDarkTheme) {
        lightPalette palette;
        auto lightPalette = palette.setLightPalette();
        qApp->setPalette(lightPalette);
    } else {
        darkPalette palette;
        auto darkPalette = palette.setDarkPalette();
        qApp->setPalette(darkPalette);
    }
    this->isDarkTheme = !(this->isDarkTheme);
//    refreshCharts();
}


MainWindow::~MainWindow()
{
    delete ui;
}
