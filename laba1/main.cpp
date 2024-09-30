#include "headers/FinanceService.h"
#include "FinanceService.cpp"
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
