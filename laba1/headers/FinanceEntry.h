#ifndef FINANCEENTRY_H
#define FINANCEENTRY_H

#include <string>
#include <iostream>

class FinanceEntry
{
private:
    std::string description;
    double amount;
    int id;

public:
    FinanceEntry(const std::string &description, double amount, int id)
        : description(description), amount(amount), id(id) {}

    virtual ~FinanceEntry() = default;

    virtual std::string getType() const = 0;

    virtual void display() const
    {
        std::cout << "ID: " << id << ", Description: " << description << ", Amount: " << amount << ", Type: " << getType() << std::endl;
    }

    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    int getId() const { return id; }
};

#endif
