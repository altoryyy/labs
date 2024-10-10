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

    // Объявление дружественных функций для перегрузки операторов
    friend inline bool operator==(const FinanceRecord &lhs, const FinanceRecord &rhs);
    inline friend FinanceRecord operator+(const FinanceRecord &lhs, const FinanceRecord &rhs);
    inline friend FinanceRecord operator-(const FinanceRecord &lhs, const FinanceRecord &rhs);
    inline friend std::ostream &operator<<(std::ostream &os, const FinanceRecord &record);
};

#endif // FINANCERECORD_H