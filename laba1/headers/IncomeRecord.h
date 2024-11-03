#ifndef INCOMERECORD_H
#define INCOMERECORD_H

#include "FinanceEntry.h"
#include <stdexcept>

class IncomeRecord : public FinanceEntry {
public:
    IncomeRecord(const std::string &description, double amount, int id)
        : FinanceEntry(description, amount, id) {
        if (description.empty()) {
            throw std::invalid_argument("Описание не может быть пустым.");
        }
        if (amount < 0) {
            throw std::invalid_argument("Сумма не может быть отрицательной.");
        }
    }

    IncomeRecord(const IncomeRecord& other)
        : FinanceEntry(other) {}

    IncomeRecord(IncomeRecord&& other) noexcept
        : FinanceEntry(std::move(other)) {}

    IncomeRecord& operator=(const IncomeRecord& other) {
        if (this != &other) {
            FinanceEntry::operator=(other);
        }
        return *this;
    }

    IncomeRecord& operator=(IncomeRecord&& other) noexcept {
        if (this != &other) {
            FinanceEntry::operator=(std::move(other));
        }
        return *this;
    }

    std::string getType() const override {
        return "Income";
    }
};

#endif // INCOMERECORD_H
