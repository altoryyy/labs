#ifndef EXPENSERECORD_H
#define EXPENSERECORD_H

#include "FinanceEntry.h"
#include <stdexcept>

class ExpenseRecord : public FinanceEntry {
public:
    ExpenseRecord(const std::string &description, double amount, int id)
        : FinanceEntry(description, amount, id) {
        if (description.empty()) {
            throw std::invalid_argument("Description cannot be empty.");
        }
        if (amount < 0) {
            throw std::invalid_argument("Amount cannot be negative.");
        }
    }

    std::string getType() const override {
        return "Expense";
    }
};

#endif
