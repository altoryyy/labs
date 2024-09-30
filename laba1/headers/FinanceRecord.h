#ifndef FINANCERECORD_H
#define FINANCERECORD_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class FinanceRecord
{
public:
    FinanceRecord(const std::string &description, double amount);
    void display() const;
    double getAmount() const;

private:
    std::string description;
    double amount;
};

#endif