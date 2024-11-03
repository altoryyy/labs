#include "FinanceService.h"
#include "DatabaseService.h"
#include "Budget.h"
#include <format>
#include <stdexcept>

FinanceService::FinanceService(double initialBudget)
    : budget(initialBudget), targetBudget(initialBudget) {
    DatabaseService &dbService = DatabaseService::getInstance();
    if (!dbService.openDatabase("Records.db")) {
        std::cerr << "Не удалось открыть базу данных!" << std::endl;
    }
    budget.loadBudgetFromDatabase(dbService.getDb());
}

void FinanceService::setTargetBudget(double newTargetBudget) { // Renamed parameter
    this->targetBudget = newTargetBudget;
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "UPDATE Budget SET TargetAmount = ?;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    sqlite3_bind_double(stmt, 1, newTargetBudget); // Use new parameter name
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Ошибка обновления бюджета: " << sqlite3_errmsg(dbService.getDb()) << std::endl;
    } else {
        std::cout << "Целевой бюджет установлен: " << this->targetBudget << std::endl;
    }
    sqlite3_finalize(stmt);
}

void FinanceService::createIncome(const std::string &description, double amount) {
    const DatabaseService &dbService = DatabaseService::getInstance();
    try {
        budget.addIncome(description, amount);
        dbService.createRecord(description, amount, "Income");
        saveBudget();
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при создании дохода: " << e.what() << std::endl;
    }
}

void FinanceService::readRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT ID, Description, Amount, Type FROM FinanceRecords;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::unique_ptr<FinanceEntry> record;
        if (type == "Income") {
            record = std::make_unique<IncomeRecord>(description, amount, id);
        } else if (type == "Expense") {
            record = std::make_unique<ExpenseRecord>(description, amount, id);
        } else {
            std::cerr << "Неизвестный тип записи: " << type << std::endl;
            continue;
        }
        record->display();
    }
    sqlite3_finalize(stmt);
}

void FinanceService::updateRecord(int id, const std::string &description, double amount) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    std::string sql = "UPDATE FinanceRecords SET Description = ?, Amount = ? WHERE ID = ?;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int(stmt, 3, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Ошибка обновления записи: " << sqlite3_errmsg(dbService.getDb()) << std::endl;
    }
    sqlite3_finalize(stmt);
}

void FinanceService::deleteRecord(int id) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    std::string sql = std::format("DELETE FROM FinanceRecords WHERE ID = {}; ", id);
    dbService.executeSQL(sql);
}

std::unique_ptr<FinanceEntry> FinanceService::getRecordById(int id) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    auto record = dbService.getRecordById(id);
    if (!record) {
        std::cerr << "Запись с ID " << id << " не найдена!" << std::endl;
        return nullptr;
    }
    return record;
}

void FinanceService::createExpense(const std::string &description, double amount) {
    const DatabaseService &dbService = DatabaseService::getInstance();
    try {
        budget.addExpense(description, amount);
        dbService.createRecord(description, amount, "Expense");
        saveBudget();
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при создании расхода: " << e.what() << std::endl;
    }
}

void FinanceService::displayIncomeRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT * FROM FinanceRecords WHERE Type = 'Income' ORDER BY Description;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    std::cout << "Записи доходов (отсортированные по описанию):" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::cout << "ID: " << id
                  << ", Описание: " << description
                  << ", Сумма: " << amount
                  << ", Тип: " << type << std::endl;
    }
    sqlite3_finalize(stmt);
}

void FinanceService::displayExpenseRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT * FROM FinanceRecords WHERE Type = 'Expense' ORDER BY Description;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    std::cout << "Записи расходов (отсортированные по описанию):" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::cout << "ID: " << id
                  << ", Описание: " << description
                  << ", Сумма: " << amount
                  << ", Тип: " << type << std::endl;
    }
    sqlite3_finalize(stmt);
}

void FinanceService::saveBudget() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    budget.saveBudgetToDatabase(dbService.getDb());
}

double FinanceService::calculateTotalIncome() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    double totalIncome = 0.0;
    const char *sql = "SELECT SUM(Amount) FROM FinanceRecords WHERE Type = 'Income';";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return totalIncome;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        totalIncome = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return totalIncome;
}

double FinanceService::calculateTotalExpenses() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    double totalExpenses = 0.0;
    const char *sql = "SELECT SUM(Amount) FROM FinanceRecords WHERE Type = 'Expense';";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return totalExpenses;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        totalExpenses = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return totalExpenses;
}

QString FinanceService::getBudgetSummary() const {
    double targetAmount = budget.getTargetAmount();
    double totalIncome = calculateTotalIncome();
    double totalExpenses = calculateTotalExpenses();
    double balance = targetAmount + totalIncome - totalExpenses;
    return QString("Целевой бюджет: %1\nОбщие доходы: %2\nОбщие расходы: %3\nОстаток бюджета: %4")
        .arg(targetAmount)
        .arg(totalIncome)
        .arg(totalExpenses)
        .arg(balance);
}

std::vector<std::unique_ptr<FinanceEntry>> FinanceService::getRecords() const {
    std::vector<std::unique_ptr<FinanceEntry>> records;
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT ID, Description, Amount, Type FROM FinanceRecords;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return records;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        std::unique_ptr<FinanceEntry> record;
        if (type == "Income") {
            record = std::make_unique<IncomeRecord>(description, amount, id);
        } else {
            record = std::make_unique<ExpenseRecord>(description, amount, id);
        }
        records.push_back(std::move(record));
    }
    sqlite3_finalize(stmt);
    return records;
}

void FinanceService::clearRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    dbService.clearRecords();
}
