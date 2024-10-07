#ifndef FINANCERECORD_H
#define FINANCERECORD_H

#include <iostream>
#include <string>

class FinanceRecord
{
    std::string description;
    double amount;
    int id; // Поле для хранения ID

public:
    FinanceRecord(const std::string &description, double amount, int id); // Конструктор с тремя параметрами

    void display() const;
    double getAmount() const;
    std::string getDescription() const;
    int getId() const; // Новый геттер для ID

    // Дружественная функция
    friend std::ostream &operator<<(std::ostream &os, const FinanceRecord &record);

    // Перегрузка операторов
    bool operator==(const FinanceRecord &other) const;
    FinanceRecord operator+(const FinanceRecord &other) const;
    FinanceRecord operator-(const FinanceRecord &other) const;
};

#endif // FINANCERECORD_H