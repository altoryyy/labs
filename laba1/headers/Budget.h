#ifndef BUDGET_H
#define BUDGET_H

#include "IncomeRecord.h"
#include "ExpenseRecord.h"
#include <vector>
#include <sqlite3.h>

class Budget
{
private:
    double targetAmount;
    double totalIncome {0.0};
    double totalExpenses {0.0};
    std::vector<IncomeRecord> incomeRecords;
    std::vector<ExpenseRecord> expenseRecords;

public:
    Budget();
    explicit Budget(double target);

    void addIncome(const std::string &description, double amount);
    void addExpense(const std::string &description, double amount);
    double getTotalIncome() const;
    double getTotalExpenses() const;
    double getTargetAmount() const;
    double getRemainingBudget() const;
    void displayBudgetSummary() const;
    void displayIncomeRecords() const;
    void displayExpenseRecords() const;


    void saveBudgetToDatabase(sqlite3 *db) const;
    void loadBudgetFromDatabase(sqlite3 *db);

    ~Budget() = default;
};

#endif
