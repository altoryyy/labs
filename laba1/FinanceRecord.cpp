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
    return id; // Метод для получения ID
}

// Дружественная функция
std::ostream &operator<<(std::ostream &os, const FinanceRecord &record)
{
    os << "ID: " << record.id << ", Description: " << record.description << ", Amount: " << record.amount;
    return os;
}

// Перегрузка операторов
bool FinanceRecord::operator==(const FinanceRecord &other) const
{
    return description == other.description && amount == other.amount;
}

FinanceRecord FinanceRecord::operator+(const FinanceRecord &other) const
{
    return FinanceRecord(description + " & " + other.description, amount + other.amount, 1); // ID не имеет значения при сложении
}

FinanceRecord FinanceRecord::operator-(const FinanceRecord &other) const
{
    return FinanceRecord(description + " - " + other.description, amount - other.amount, 1); // ID не имеет значения при вычитании
}