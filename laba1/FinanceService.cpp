#include "FinanceService.h"
#include "DatabaseService.h"
#include "Budget.h"
#include <format>
#include <stdexcept>
#include <functional>

FinanceService::FinanceService(double initialBudget)
    : budget(initialBudget), targetBudget(initialBudget) {
    if (!initializeDatabase()) {
        std::cerr << "Не удалось открыть базу данных!" << std::endl;
    }
    budget.loadBudgetFromDatabase(DatabaseService::getInstance().getDb());
}

bool FinanceService::initializeDatabase() {
    DatabaseService &dbService = DatabaseService::getInstance();
    return dbService.openDatabase("Records.db");
}

void FinanceService::setTargetBudget(double newTargetBudget) {
    this->targetBudget = newTargetBudget;
    executeUpdate("UPDATE Budget SET TargetAmount = ?", newTargetBudget);
    std::cout << "Целевой бюджет установлен: " << this->targetBudget << std::endl;
}

void FinanceService::executeUpdate(const std::string &sql, double value) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return;
    }
    sqlite3_bind_double(stmt, 1, value);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Ошибка обновления: " << sqlite3_errmsg(dbService.getDb()) << std::endl;
    }
    sqlite3_finalize(stmt);
}

void FinanceService::createIncome(const std::string &description, double amount) {
    processTransaction(description, amount, "Income", [this](const std::string &desc, double amt) {
        budget.addIncome(desc, amt);
    });
}

void FinanceService::createExpense(const std::string &description, double amount) {
    processTransaction(description, amount, "Expense", [this](const std::string &desc, double amt) {
        budget.addExpense(desc, amt);
    });
}

void FinanceService::processTransaction(const std::string &description, double amount, const std::string &type, 
    const std::function<void(const std::string&, double)>& method) {
    const DatabaseService &dbService = DatabaseService::getInstance();
    try {
        method(description, amount);
        dbService.createRecord(description, amount, type);
        saveBudget();
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при создании " << type << ": " << e.what() << std::endl;
    }
}

void FinanceService::readRecords() const {
    executeReadQuery("SELECT ID, Description, Amount, Type FROM FinanceRecords;");
}

void FinanceService::executeReadQuery(const std::string &sql) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
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
        auto record = (type == "Income")
            ? std::make_unique<IncomeRecord>(description, amount, id)
            : std::make_unique<ExpenseRecord>(description, amount, id);
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
    return dbService.getRecordById(id);
}

void FinanceService::displayIncomeRecords() const {
    executeReadQuery("SELECT * FROM FinanceRecords WHERE Type = 'Income' ORDER BY Description;");
}

void FinanceService::displayExpenseRecords() const {
    executeReadQuery("SELECT * FROM FinanceRecords WHERE Type = 'Expense' ORDER BY Description;");
}

void FinanceService::saveBudget() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    budget.saveBudgetToDatabase(dbService.getDb());
}

double FinanceService::calculateTotalIncome() const {
    return calculateTotal("Income");
}

double FinanceService::calculateTotalExpenses() const {
    return calculateTotal("Expense");
}

double FinanceService::calculateTotal(const std::string &type) const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    double total = 0.0;
    std::string sql = "SELECT SUM(Amount) FROM FinanceRecords WHERE Type = ?;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);
    if (!stmt) {
        std::cerr << "Ошибка подготовки SQL-запроса!" << std::endl;
        return total;
    }
    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return total;
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
        auto record = (type == "Income")
            ? std::make_unique<IncomeRecord>(description, amount, id)
            : std::make_unique<ExpenseRecord>(description, amount, id);
        records.push_back(std::move(record));
    }
    sqlite3_finalize(stmt);
    return records;
}

void FinanceService::clearRecords() const {
    const DatabaseService &dbService = DatabaseService::getInstance();
    dbService.clearRecords();
}
