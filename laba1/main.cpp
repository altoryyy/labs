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

int main()
{
    FinanceService service;
    int choice;
    std::string description;
    double amount;
    size_t index;

    do
    {
        std::cout << "1. Create Record\n2. Read Records\n3. Update Record\n4. Delete Record\n5. Calculate Total Balance\n0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Enter description: ";
            std::cin >> description;
            std::cout << "Enter amount: ";
            std::cin >> amount;
            service.createRecord(description, amount);
            break;
        case 2:
            service.readRecords();
            break;
        case 3:
            std::cout << "Enter index: ";
            std::cin >> index;
            std::cout << "Enter new description: ";
            std::cin >> description;
            std::cout << "Enter new amount: ";
            std::cin >> amount;
            service.updateRecord(index, description, amount);
            break;
        case 4:
            std::cout << "Enter index: ";
            std::cin >> index;
            service.deleteRecord(index);
            break;
        case 5:
            std::cout << "Total Balance: " << service.calculateTotalBalance() << std::endl;
            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cerr << "Invalid choice!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}
