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

    IncomeRecord(const IncomeRecord& other) = default;
    IncomeRecord(IncomeRecord&& other) noexcept = default;

    IncomeRecord& operator=(const IncomeRecord& other) = default;
    IncomeRecord& operator=(IncomeRecord&& other) noexcept = default;

    std::string getType() const override {
        return "Income";
    }
};

#endif
