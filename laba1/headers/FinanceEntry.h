#ifndef FINANCEENTRY_H
#define FINANCEENTRY_H

#include <string>
#include <iostream>

class FinanceEntry {
private:
    std::string description;
    double amount;
    int id;

public:
    FinanceEntry(const std::string &description, double amount, int id)
        : description(description), amount(amount), id(id) {}

    FinanceEntry(const FinanceEntry& other)
        : description(other.description), amount(other.amount), id(other.id) {}

    FinanceEntry(FinanceEntry&& other) noexcept
        : description(std::move(other.description)),
        amount(other.amount), id(other.id) {}

    FinanceEntry& operator=(const FinanceEntry& other) {
        if (this != &other) {
            description = other.description;
            amount = other.amount;
            id = other.id;
        }
        return *this;
    }

    FinanceEntry& operator=(FinanceEntry&& other) noexcept {
        if (this != &other) {
            description = std::move(other.description);
            amount = other.amount;
            id = other.id;
        }
        return *this;
    }

    virtual ~FinanceEntry() = default;

    virtual std::string getType() const = 0;

    virtual void display() const {
        std::cout << "ID: " << id << ", Description: " << description
                  << ", Amount: " << amount << ", Type: " << getType() << std::endl;
    }

    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    int getId() const { return id; }
};

#endif
