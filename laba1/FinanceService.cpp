#include "FinanceService.h"
#include "DatabaseService.h"
#include "Budget.h"
#include <format>
#include "Stack.h"
#include <stdexcept>
#include "CustomException.h"

template<typename T>
void bindParameter(sqlite3_stmt *stmt, int index, const T &value);

template<>
void bindParameter<std::string>(sqlite3_stmt *stmt, int index, const std::string &value) {
    sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC);
}

template<>
void bindParameter<double>(sqlite3_stmt *stmt, int index, const double &value) {
    sqlite3_bind_double(stmt, index, value);
}

template<>
void bindParameter<int>(sqlite3_stmt *stmt, int index, const int &value) {
    sqlite3_bind_int(stmt, index, value);
}

FinanceService::FinanceService(double initialBudget)
    : budget(initialBudget), targetBudget(initialBudget) {
    DatabaseService &dbService = DatabaseService::getInstance();
    if (!dbService.openDatabase("Records.db")) {
        throw CustomException("Не удалось открыть базу данных!");
    }
    budget.loadBudgetFromDatabase(dbService.getDb());
}

void FinanceService::setTargetBudget(double newTargetBudget) {
    this->targetBudget = newTargetBudget;
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "UPDATE Budget SET TargetAmount = ?;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        throw CustomException("Ошибка подготовки SQL-запроса для обновления бюджета.");
    }
    bindParameter(stmt, 1, newTargetBudget);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw CustomException("Ошибка обновления бюджета: " + std::string(sqlite3_errmsg(dbService.getDb())));
    }
    sqlite3_finalize(stmt);
}

int FinanceService::createIncome(const std::string &description, double amount) {
    const DatabaseService &dbService = DatabaseService::getInstance();
    if (amount <= 0) {
        throw CustomException("Сумма дохода должна быть положительной.");
    }
    try {
        budget.addIncome(description, amount);

        return dbService.createRecord(description, amount, "Income");
    } catch (const std::system_error &e) {
        throw CustomException("Ошибка при создании дохода: " + std::string(e.what()));
    }
}

void FinanceService::readRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT ID, Description, Amount, Type FROM FinanceRecords;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        throw CustomException("Ошибка подготовки SQL-запроса для чтения записей.");
    }

    Stack<std::unique_ptr<FinanceEntry>> recordStack;

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
            throw CustomException("Неизвестный тип записи: " + type);
        }

        recordStack.push(std::move(record));
    }

    while (!recordStack.isEmpty()) {
        auto record = std::move(recordStack.top());
        record->display();
        recordStack.pop();
    }

    sqlite3_finalize(stmt);
}

void FinanceService::updateRecord(int id, const std::string &description, double amount) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    std::string sql = "UPDATE FinanceRecords SET Description = ?, Amount = ? WHERE ID = ?;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        throw CustomException("Ошибка подготовки SQL-запроса для обновления записи.");
    }
    bindParameter(stmt, 1, description);
    bindParameter(stmt, 2, amount);
    bindParameter(stmt, 3, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw CustomException("Ошибка обновления записи: " + std::string(sqlite3_errmsg(dbService.getDb())));
    }
    sqlite3_finalize(stmt);
}

void FinanceService::deleteRecord(int id) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    std::string sql = std::format("DELETE FROM FinanceRecords WHERE ID = {}; ", id);

    try {
        dbService.executeSQL(sql);
    } catch (const std::exception &e) {
        throw CustomException("Ошибка удаления записи с ID " + std::to_string(id) + ": " + e.what());
    }
}

std::unique_ptr<FinanceEntry> FinanceService::getRecordById(int id) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    auto record = dbService.getRecordById(id);
    if (!record) {
        throw CustomException("Запись с ID " + std::to_string(id) + " не найдена!");
    }
    return record;
}

int FinanceService::createExpense(const std::string &description, double amount) {
    const DatabaseService &dbService = DatabaseService::getInstance();
    if (amount <= 0) {
        throw CustomException("Сумма расхода должна быть положительной.");
    }
    try {
        budget.addExpense(description, amount);
        return dbService.createRecord(description, amount, "Expense");
    } catch (const std::system_error &e) {
        throw CustomException("Ошибка при создании расхода: " + std::string(e.what()));
    }
}

void FinanceService::displayIncomeRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    const char *sql = "SELECT * FROM FinanceRecords WHERE Type = 'Income' ORDER BY Description;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        throw CustomException("Ошибка подготовки SQL-запроса для отображения доходов.");
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
        throw CustomException("Ошибка подготовки SQL-запроса для отображения расходов.");
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
        throw CustomException("Ошибка подготовки SQL-запроса для расчета общего дохода.");
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
        throw CustomException("Ошибка подготовки SQL-запроса для расчета общих расходов.");
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
        throw CustomException("Ошибка подготовки SQL-запроса для получения записей.");
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

void FinanceService::loadBudget() {
    DatabaseService& dbService = DatabaseService::getInstance();
    budget.loadBudgetFromDatabase(dbService.getDb());
}


int FinanceService::getLastRecordId() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    return dbService.getLastInsertedId();
}
