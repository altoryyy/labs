#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <string_view>
#include <algorithm>

class FinanceRecord
{
public:
    std::string description;
    double amount;

    FinanceRecord(std::string_view desc, double amt) : description(desc), amount(amt) {}
};

class FinanceManager
{
private:
    std::vector<std::unique_ptr<FinanceRecord>> records;

public:
    // Конструктор по умолчанию
    FinanceManager() = default;

    // Копирующий конструктор
    FinanceManager(const FinanceManager &other)
    {
        std::ranges::for_each(other.records, this {
            records.push_back(std::make_unique<FinanceRecord>(*record));
        });
    }

    // Оператор присваивания копированием
    FinanceManager &operator=(const FinanceManager &other)
    {
        if (this != &other)
        {
            records.clear();
            std::ranges::for_each(other.records, this {
                records.push_back(std::make_unique<FinanceRecord>(*record));
            });
        }
        return *this;
    }

    // Конструктор перемещения
    FinanceManager(FinanceManager &&other) noexcept = default;

    // Оператор присваивания перемещением
    FinanceManager &operator=(FinanceManager &&other) noexcept = default;

    // Добавление записи
    void addRecord(std::string_view description, double amount)
    {
        records.push_back(std::make_unique<FinanceRecord>(description, amount));
    }

    // Отображение записей
    void displayRecords() const
    {
        std::ranges::for_each(records,  {
            std::cout << "Описание: " << record->description << ", Сумма: " << record->amount << std::endl;
        });
    }

    // Обновление записи
    void updateRecord(int index, std::string_view newDescription, double newAmount)
    {
        if (index >= 0 && index < records.size())
        {
            records[index]->description = newDescription;
            records[index]->amount = newAmount;
        }
        else
        {
            std::cout << "Неправильный индекс!" << std::endl;
        }
    }

    // Удаление записи
    void deleteRecord(int index)
    {
        if (index >= 0 && index < records.size())
        {
            records.erase(records.begin() + index);
        }
        else
        {
            std::cout << "Неправильный индекс!" << std::endl;
        }
    }

    // Итоговый баланс
    double calculateTotalBalance() const
    {
        double total = 0;
        std::ranges::for_each(records, &total {
            total += record->amount;
        });
        return total;
    }

    // Освобождение памяти
    ~FinanceManager() = default;
};

int main()
{
    FinanceManager manager;
    int choice;
    std::string description;
    double amount;
    int index;

    do
    {
        std::cout << "1. Добавить запись\n2. Список записей\n3. Обновить записи\n4. Удалить запись\n5. Показать баланс\n0. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Введите описание: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Введите сумму: ";
            std::cin >> amount;
            manager.addRecord(description, amount);
            break;
        case 2:
            manager.displayRecords();
            break;
        case 3:
            std::cout << "Введите индекс для обновления: ";
            std::cin >> index;
            std::cout << "Введите новое описание: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Введите новую сумму: ";
            std::cin >> amount;
            manager.updateRecord(index, description, amount);
            break;
        case 4:
            std::cout << "Введите индекс для удаления: ";
            std::cin >> index;
            manager.deleteRecord(index);
            break;
        case 5:
            std::cout << "Итого: " << manager.calculateTotalBalance() << std::endl;
            break;
        case 0:
            std::cout << "Выход..." << std::endl;
            break;
        default:
            std::cout << "Неправильный выбор!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}
