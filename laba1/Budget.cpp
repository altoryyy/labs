#include "Budget.h"
#include <iostream>
#include <string>

Budget::Budget() : targetAmount(0.0), totalIncome(0.0), totalExpenses(0.0) {}

Budget::Budget(double target) : targetAmount(target), totalIncome(0.0), totalExpenses(0.0) {}

void Budget::addIncome(const std::string &description, double amount)
{
    IncomeRecord income(description, amount, incomeRecords.size() + 1);
    incomeRecords.push_back(income);
    totalIncome += amount;
}

void Budget::addExpense(const std::string &description, double amount)
{
    ExpenseRecord expense(description, amount, expenseRecords.size() + 1);
    expenseRecords.push_back(expense);
    totalExpenses += amount;
}

double Budget::getTotalIncome() const
{
    return totalIncome;
}

double Budget::getTotalExpenses() const
{
    return totalExpenses;
}

double Budget::getRemainingBudget() const
{
    return targetAmount - totalExpenses;
}

double Budget::getTargetAmount() const
{
    return targetAmount;
}

void Budget::displayBudgetSummary() const
{
    std::cout << "Целевой бюджет: " << targetAmount << std::endl;
    std::cout << "Общие доходы: " << totalIncome << std::endl;
    std::cout << "Общие расходы: " << totalExpenses << std::endl;
    std::cout << "Остаток бюджета: " << getRemainingBudget() << std::endl;
}

void Budget::displayIncomeRecords() const
{
    std::cout << "Записи доходов:" << std::endl;
    for (const auto &record : incomeRecords)
    {
        record.display();
    }
}

void Budget::displayExpenseRecords() const
{
    std::cout << "Записи расходов:" << std::endl;
    for (const auto &record : expenseRecords)
    {
        record.display();
    }
}

void Budget::saveBudgetToDatabase(sqlite3 *db)
{
    std::string sql = "INSERT OR REPLACE INTO Budget (ID, TargetAmount) VALUES (1, " + std::to_string(targetAmount) + ");";
    char *errMsg = nullptr;

    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg);
    }
}

void Budget::loadBudgetFromDatabase(sqlite3 *db)
{
    const char *sql = "SELECT TargetAmount FROM Budget WHERE ID = 1;";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            targetAmount = sqlite3_column_double(stmt, 0);
        }
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
