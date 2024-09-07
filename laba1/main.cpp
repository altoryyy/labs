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
    // Create
    void addRecord(const std::string &description, double amount)
    {
        records.push_back(new FinanceRecord(description, amount));
    }

    // Read
    void displayRecords() const
    {
        for (size_t i = 0; i < records.size(); ++i)
        {
            std::cout << "Description: " << records[i]->description << ", Amount: " << records[i]->amount << std::endl;
        }
    }

    // Update
    void updateRecord(int index, const std::string &newDescription, double newAmount)
    {
        if (index >= 0 && index < records.size())
        {
            records[index]->description = newDescription;
            records[index]->amount = newAmount;
        }
        else
        {
            std::cout << "Invalid index!" << std::endl;
        }
    }

    // Delete
    void deleteRecord(int index)
    {
        if (index >= 0 && index < records.size())
        {
            delete records[index];
            records.erase(records.begin() + index);
        }
        else
        {
            std::cout << "Invalid index!" << std::endl;
        }
    }

    // Calculate total balance
    double calculateTotalBalance() const
    {
        double total = 0;
        for (size_t i = 0; i < records.size(); ++i)
        {
            total += records[i]->amount;
        }
        return total;
    }

    // Destructor to free dynamically allocated memory
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
        std::cout << "1. Add Record\n2. Display Records\n3. Update Record\n4. Delete Record\n5. Calculate Total Balance\n0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Enter description: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Enter amount: ";
            std::cin >> amount;
            manager.addRecord(description, amount);
            break;
        case 2:
            manager.displayRecords();
            break;
        case 3:
            std::cout << "Enter index to update: ";
            std::cin >> index;
            std::cout << "Enter new description: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Enter new amount: ";
            std::cin >> amount;
            manager.updateRecord(index, description, amount);
            break;
        case 4:
            std::cout << "Enter index to delete: ";
            std::cin >> index;
            manager.deleteRecord(index);
            break;
        case 5:
            std::cout << "Total Balance: " << manager.calculateTotalBalance() << std::endl;
            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}

