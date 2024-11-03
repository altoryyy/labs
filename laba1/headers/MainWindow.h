#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "FinanceService.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include "DatabaseService.h"
#include "ui_MainWindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget *incomeListWidget;
    QListWidget *expenseListWidget;
    FinanceService financeService;
    QLabel *budgetSummaryLabel;
    void loadRecords();
    void updateBudgetSummary();

private slots:
    void setTargetBudget();
    void addIncome();
    void addExpense();
    void updateRecord();
    void clearAllRecords();
    void createPieChart();
signals:
    void expensesAdded();
};

#endif
