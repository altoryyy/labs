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
#include "ActionManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QListWidget *incomeListWidget;
    QListWidget *expenseListWidget;
    FinanceService financeService;
    QLabel *budgetSummaryLabel;
    ActionManager actionManager;
    void loadRecords();
    void updateBudgetSummary();

private slots:
    void setTargetBudget();
    void addIncome();
    void addExpense();
    void updateRecord();
    void clearAllRecords();
    void createPieChart();
    void undoLastAction();
signals:
    void expensesAdded();
};

#endif
