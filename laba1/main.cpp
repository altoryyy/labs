#include "headers/FinanceService.h"
#include "FinanceService.cpp"

int main() // cd "/Users/artemlopatin/уник/labs/laba1/src/" && g++ -std=c++20 main.cpp -o main -lsqlite3 && "/Users/artemlopatin/уник/lабs/laba1/src/"main
{
    FinanceService service;
    int choice;
    std::string description;
    double amount;
    int id1;
    int id2;

    std::cout << std::fixed << std::setprecision(2);

    do
    {
        std::cout << "1. Create Record\n"
                  << "2. Read Records\n"
                  << "3. Update Record\n"
                  << "4. Delete Record\n"
                  << "5. Calculate Total Balance\n"
                  << "6. Clear Records\n"
                  << "7. Compare Records\n"
                  << "8. Add Records\n"
                  << "9. Subtract Records\n"
                  << "0. Exit\n";
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
            service.createRecord(description, amount);
            break;
        case 2:
            service.readRecords();
            break;
        case 3:
            std::cout << "Enter ID of the record to update: ";
            std::cin >> id1;
            std::cout << "Enter new description: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Enter new amount: ";
            std::cin >> amount;
            service.updateRecord(id1, description, amount);
            break;
        case 4:
            std::cout << "Enter ID of the record to delete: ";
            std::cin >> id1;
            service.deleteRecord(id1);
            break;
        case 5:
            std::cout << "Total Balance: " << service.calculateTotalBalance() << std::endl;
            break;
        case 6:
            service.clearRecords();
            std::cout << "Все записи были удалены." << std::endl;
            break;
        case 7:
        {
            std::cout << "Enter ID of the first record to compare: ";
            std::cin >> id1;
            std::cout << "Enter ID of the second record to compare: ";
            std::cin >> id2;

            FinanceRecord record1 = service.getRecordById(id1);

            // Declare record2 within the if statement
            if (FinanceRecord record2 = service.getRecordById(id2); record1 == record2)
            {
                std::cout << "Records are equal." << std::endl;
            }
            else
            {
                std::cout << "Records are not equal." << std::endl;
            }
            break;
        }
        case 8:
        {
            std::cout << "Enter ID of the first record to add: ";
            std::cin >> id1;
            std::cout << "Enter ID of the second record to add: ";
            std::cin >> id2;

            FinanceRecord record1 = service.getRecordById(id1);
            FinanceRecord record2 = service.getRecordById(id2);
            FinanceRecord combinedRecord = record1 + record2;

            std::cout << "Combined Record: " << combinedRecord << std::endl;
            break;
        }
        case 9:
        {
            std::cout << "Enter ID of the first record to subtract: ";
            std::cin >> id1;
            std::cout << "Enter ID of the second record to subtract: ";
            std::cin >> id2;

            FinanceRecord record1 = service.getRecordById(id1);
            FinanceRecord record2 = service.getRecordById(id2);
            FinanceRecord subtractedRecord = record1 - record2;

            std::cout << "Subtracted Record: " << subtractedRecord << std::endl;
            break;
        }
        case 0:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cerr << "Invalid choice!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}