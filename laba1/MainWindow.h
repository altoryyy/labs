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
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget *incomeListWidget;
    QListWidget *expenseListWidget;
    FinanceService financeService;
    QLabel *budgetSummaryLabel; // New label for budget summary
    void loadRecords();
    void updateBudgetSummary(); // New method to update the budget summary display

private slots:
    void setTargetBudget();
    void addIncome();
    void addExpense();
    void updateRecord();
    void clearAllRecords();
    void createPieChart();
signals:
    void expensesAdded(); // Добавьте этот сигнал в ваш класс
};

#endif
