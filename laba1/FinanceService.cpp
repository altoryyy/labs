#include "FinanceApp.h"
#include "FinanceRecord.cpp"

void FinanceService::createRecord(const std::string &description, double amount)
{
    records.push_back(std::make_unique<FinanceRecord>(description, amount));
}

void FinanceService::readRecords() const
{
    for (const auto &record : records)
    {
        record->display();
    }
}

void FinanceService::updateRecord(size_t index, const std::string &description, double amount)
{
    if (index < records.size())
    {
        records[index] = std::make_unique<FinanceRecord>(description, amount);
    }
    else
    {
        std::cerr << "Invalid index!" << std::endl;
    }
}

void FinanceService::deleteRecord(size_t index)
{
    if (index < records.size())
    {
        records.erase(records.begin() + index);
    }
    else
    {
        std::cerr << "Invalid index!" << std::endl;
    }
}

double FinanceService::calculateTotalBalance() const
{
    double total = 0;
    for (const auto &record : records)
    {
        total += record->getAmount();
    }
    return total;
}
