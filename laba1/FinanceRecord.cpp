#include "headers/FinanceRecord.h"

FinanceRecord::FinanceRecord(const std::string &description, double amount, int id)
    : description(description), amount(amount), id(id) {}

void FinanceRecord::display() const
{
    std::cout << "ID: " << id << ", Description: " << description << ", Amount: " << amount << std::endl;
}

double FinanceRecord::getAmount() const
{
    return amount;
}

std::string FinanceRecord::getDescription() const
{
    return description;
}

int FinanceRecord::getId() const
{
    return id;
}

// Определение оператора ==
bool operator==(const FinanceRecord &lhs, const FinanceRecord &rhs)
{
    return lhs.description == rhs.description && lhs.amount == rhs.amount;
}

// Определение оператора вывода
std::ostream &operator<<(std::ostream &os, const FinanceRecord &record)
{
    os << "ID: " << record.id << ", Description: " << record.description << ", Amount: " << record.amount;
    return os;
}