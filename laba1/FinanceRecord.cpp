#include <iostream>
#include <vector>
#include <memory>
#include <string>

class FinanceRecord
{
public:
    FinanceRecord(const std::string &description, double amount)
        : description(description), amount(amount) {}

    void display() const
    {
        std::cout << "Description: " << description << ", Amount: " << amount << std::endl;
    }

    double getAmount() const
    {
        return amount;
    }

private:
    std::string description;
    double amount;
};