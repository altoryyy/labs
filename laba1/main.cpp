/*#include "FinanceService.h"
#include <iostream>
#include <iomanip>
#include "ActionHandler.h"

int main()
{
    FinanceService service(0.0);
    int choice;
    std::string description;
    double amount;
    int id;

    std::cout << std::fixed << std::setprecision(2);

    do
    {
        std::cout << "\n1. Установить целевой бюджет\n"
                  << "2. Создать запись дохода\n"
                  << "3. Создать запись расхода\n"
                  << "4. Прочитать записи\n"
                  << "5. Обновить запись\n"
                  << "6. Удалить запись\n"
                  << "7. Показать сводку бюджета\n"
                  << "8. Показать записи доходов\n"
                  << "9. Показать записи расходов\n"
                  << "10. Очистить записи\n"
                  << "0. Выход\n";
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;

        ActionHandler<FinanceService> handler;

        switch (choice)
        {
        case 1:
            std::cout << "Введите целевой бюджет: ";
            std::cin >> amount;
            service.setTargetBudget(amount);
            break;

        case 2:
            std::cout << "Введите описание дохода: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Введите сумму дохода: ";
            std::cin >> amount;
            handler.execute(description, amount, [&](const std::string &desc, double amt)
                            { service.createIncome(desc, amt); });
            break;

        case 3:
            std::cout << "Введите описание расхода: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Введите сумму расхода: ";
            std::cin >> amount;
            handler.execute(description, amount, [&](const std::string &desc, double amt)
                            { service.createExpense(desc, amt); });
            break;

        case 4:
            service.readRecords();
            break;

        case 5:
            std::cout << "Введите ID записи для обновления: ";
            std::cin >> id;
            std::cout << "Введите новое описание: ";
            std::cin.ignore();
            std::getline(std::cin, description);
            std::cout << "Введите новую сумму: ";
            std::cin >> amount;
            service.updateRecord(id, description, amount);
            break;

        case 6:
            std::cout << "Введите ID записи для удаления: ";
            std::cin >> id;
            service.deleteRecord(id);
            break;

        case 7:
            service.displayBudgetSummary();
            break;

        case 8:
            service.displayIncomeRecords();
            break;

        case 9:
            service.displayExpenseRecords();
            break;

        case 11:
            service.clearRecords();
            std::cout << "Все записи очищены." << std::endl;
            break;

        case 0:
            std::cout << "Выход..." << std::endl;
            break;

        default:
            std::cerr << "Неверный выбор!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}*/

#include <QApplication>
#include "MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "Application started" << std::endl;

    MainWindow window;
    window.show();

    std::cout << "Main window shown" << std::endl;

    return app.exec();
}

