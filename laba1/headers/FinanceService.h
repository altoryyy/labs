#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include <string>
#include <iostream>
#include "DatabaseService.h"
#include "Budget.h"
#include "sqlite3.h"
#include "FinanceEntry.h"
#include <QString>
#include <format>

class FinanceService {
private:
    Budget budget;
    double targetBudget;

public:
    FinanceService(double initialBudget = 0.0);
    void setTargetBudget(double budget);
    void createRecord(const std::string &description, double amount, const std::string &type);
    void readRecords() const;
    void updateRecord(int id, const std::string &description, double amount);
    void deleteRecord(int id);
    double calculateTotalBalance() const;
    void clearRecords();
    std::unique_ptr<FinanceEntry> getRecordById(int id) const;

    void createIncome(const std::string &description, double amount);
    void createExpense(const std::string &description, double amount);
    void displayBudgetSummary() const;
    void displayIncomeRecords() const;
    void displayExpenseRecords() const;

    QString getBudgetSummary() const;
    std::vector<std::unique_ptr<FinanceEntry>> getRecords() const;

    double calculateTotalIncome() const;
    double calculateTotalExpenses() const;

    void loadBudget();
    void saveBudget();
};

#endif
