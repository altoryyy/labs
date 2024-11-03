#ifndef EXPENSERECORD_H
#define EXPENSERECORD_H

#include "FinanceEntry.h"
#include <stdexcept>

class ExpenseRecord : public FinanceEntry {
public:
    ExpenseRecord(const std::string &description, double amount, int id)
        : FinanceEntry(description, amount, id) {
        if (description.empty()) {
            throw std::invalid_argument("Описание не может быть пустым.");
        }
        if (amount < 0) {
            throw std::invalid_argument("Сумма не может быть отрицательной.");
        }
    }

    ExpenseRecord(const ExpenseRecord& other) = default;
    ExpenseRecord(ExpenseRecord&& other) noexcept = default;

    ExpenseRecord& operator=(const ExpenseRecord& other) = default;
    ExpenseRecord& operator=(ExpenseRecord&& other) noexcept = default;

    std::string getType() const override {
        return "Expense";
    }
};

#endif
