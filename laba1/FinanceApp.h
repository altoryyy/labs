#ifndef FINANCEAPP_H
#define FINANCEAPP_H

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

class FinanceService
{
public:
    void createRecord(const std::string &description, double amount);
    void readRecords() const;
    void updateRecord(size_t index, const std::string &description, double amount);
    void deleteRecord(size_t index);
    double calculateTotalBalance() const;

private:
    std::vector<std::unique_ptr<FinanceRecord>> records;
};

#endif // FINANCEAPP_H
