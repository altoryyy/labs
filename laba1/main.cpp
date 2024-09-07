#include <iostream>
#include <vector>
#include <string>

class FinanceRecord
{
public:
    std::string description;
    double amount;

    FinanceRecord(const std::string &desc, double amt) : description(desc), amount(amt) {}
};

class FinanceManager
{
private:
    std::vector<FinanceRecord *> records;

public:
    // С
    void addRecord(const std::string &description, double amount)
    {
        records.push_back(new FinanceRecord(description, amount));
    }

    // R
    void displayRecords() const
    {
        for (size_t i = 0; i < records.size(); ++i)
        {
            std::cout << "Описание: " << records[i]->description << ", Сумма: " << records[i]->amount << std::endl;
        }
    }

    // U
    void updateRecord(int index, const std::string &newDescription, double newAmount)
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

    // D
    void deleteRecord(int index)
    {
        if (index >= 0 && index < records.size())
        {
            delete records[index];
            records.erase(records.begin() + index);
        }
        else
        {
            std::cout << "Неправильный индекс!" << std::endl;
        }
    }

    // итоговый баланс
    double calculateTotalBalance() const
    {
        double total = 0;
        for (size_t i = 0; i < records.size(); ++i)
        {
            total += records[i]->amount;
        }
        return total;
    }

    // освобождение памяти
    ~FinanceManager()
    {
        for (size_t i = 0; i < records.size(); ++i)
        {
            delete records[i];
        }
    }
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
            std::cout << "Введите суммуt: ";
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
