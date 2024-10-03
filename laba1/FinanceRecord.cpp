#include "headers/FinanceRecord.h"
sdadsa
FinanceRecord::FinanceRecord(const std::string &description, double amount)
    : description(description), amount(amount) {}

void FinanceRecord::display() const
{
    std::cout << "Description: " << description << ", Amount: " << amount << std::endl;
}

double FinanceRecord::getAmount() const
{
    return amount;
}
