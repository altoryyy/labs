#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Класс для записи финансов
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

// Класс для сервиса контроля финансов
class FinanceService
{
public:
    // Создание записи
    void createRecord(const std::string &description, double amount)
    {
        records.push_back(std::make_unique<FinanceRecord>(description, amount));
    }

    // Чтение всех записей
    void readRecords() const
    {
        for (const auto &record : records)
        {
            record->display();
        }
    }

    // Обновление записи
    void updateRecord(size_t index, const std::string &description, double amount)
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

    // Удаление записи
    void deleteRecord(size_t index)
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

    // "Интересный" метод: подсчет общего баланса
    double calculateTotalBalance() const
    {
        double total = 0;
        for (const auto &record : records)
        {
            total += record->getAmount();
        }
        return total;
    }

private:
    std::vector<std::unique_ptr<FinanceRecord>> records;
};
