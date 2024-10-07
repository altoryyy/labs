#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include "FinanceRecord.h"

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

#endif