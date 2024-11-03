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

    ExpenseRecord(const ExpenseRecord& other)
        : FinanceEntry(other) {}

    ExpenseRecord(ExpenseRecord&& other) noexcept
        : FinanceEntry(std::move(other)) {}

    ExpenseRecord& operator=(const ExpenseRecord& other) {
        if (this != &other) {
            FinanceEntry::operator=(other);
        }
        return *this;
    }

    ExpenseRecord& operator=(ExpenseRecord&& other) noexcept {
        if (this != &other) {
            FinanceEntry::operator=(std::move(other));
        }
        return *this;
    }

    std::string getType() const override {
        return "Expense";
    }
};

#endif
