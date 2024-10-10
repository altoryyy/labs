#ifndef FINANCERECORD_H
#define FINANCERECORD_H

#include <iostream>
#include <string>

class FinanceRecord
{
private:
    std::string description;
    double amount;
    int id;

public:
    FinanceRecord(const std::string &description, double amount, int id);

    void display() const;
    double getAmount() const;
    std::string getDescription() const;
    int getId() const;

    friend bool operator==(const FinanceRecord &lhs, const FinanceRecord &rhs)
    {
        return lhs.description == rhs.description && lhs.amount == rhs.amount;
    }

    friend FinanceRecord operator+(const FinanceRecord &lhs, const FinanceRecord &rhs)
    {
        return FinanceRecord(lhs.description + " & " + rhs.description, lhs.amount + rhs.amount, 1);
    }
    friend FinanceRecord operator-(const FinanceRecord &lhs, const FinanceRecord &rhs)
    {
        return FinanceRecord(lhs.description + " - " + rhs.description, lhs.amount - rhs.amount, 1);
    }
    friend std::ostream &operator<<(std::ostream &os, const FinanceRecord &record)
    {
        os << "ID: " << record.id << ", Description: " << record.description << ", Amount: " << record.amount;
        return os;
    }
};

#endif