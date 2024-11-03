#include "DatabaseService.h"
#include <iostream>
#include <filesystem>
#include <memory>

bool DatabaseService::openDatabase(const std::string &dbName) {
    std::cout << "Attempting to open database: " << dbName << std::endl;

    if (!std::filesystem::exists(dbName)) {
        std::cerr << "Database file does not exist at: " << dbName << std::endl;
        return false;
    }

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (isOpen) {
        std::cerr << "Database is already open." << std::endl;
        return true;
    }

    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    } else {
        isOpen = true;
        std::cout << "Opened database successfully" << std::endl;

        executeSQL("CREATE TABLE IF NOT EXISTS FinanceRecords (ID INTEGER PRIMARY KEY AUTOINCREMENT, Description TEXT, Amount REAL, Type TEXT);");
        executeSQL("CREATE TABLE IF NOT EXISTS Budget (ID INTEGER PRIMARY KEY AUTOINCREMENT, TargetAmount REAL NOT NULL);");

        return true;
    }
}

void DatabaseService::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        isOpen = false;
        std::cout << "Database closed." << std::endl;
    }
}

void DatabaseService::executeSQL(const std::string &sql) {
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "SQL executed successfully: " << sql << std::endl;
    }
}

sqlite3_stmt *DatabaseService::prepareStatement(const std::string &sql) const {
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return stmt;
}

void DatabaseService::clearRecords() {
    const char *deleteSql = "DELETE FROM FinanceRecords;";
    std::cout << "Executing SQL: " << deleteSql << std::endl;

    char *errMsg = nullptr;
    int result = sqlite3_exec(db, deleteSql, nullptr, nullptr, &errMsg);

    if (result != SQLITE_OK) {
        std::cerr << "Error clearing records: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "All records cleared successfully." << std::endl;

        const char *resetSql = "DELETE FROM sqlite_sequence WHERE name='FinanceRecords';";
        result = sqlite3_exec(db, resetSql, nullptr, nullptr, &errMsg);

        if (result != SQLITE_OK) {
            std::cerr << "Error resetting ID: " << (errMsg ? errMsg : "Unknown error") << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "ID reset successfully." << std::endl;
        }
    }
}

bool DatabaseService::createRecord(const std::string &description, double amount, const std::string &type) {
    if (!isOpen) {
        std::cerr << "Database is not open. Cannot create record." << std::endl;
        return false;
    }

    executeSQL("BEGIN TRANSACTION;");

    const char *sql = "INSERT INTO FinanceRecords (Description, Amount, Type) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt = prepareStatement(sql);

    if (!stmt) {
        std::cerr << "Failed to prepare statement for creating record." << std::endl;
        executeSQL("ROLLBACK;");
        return false;
    }

    sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_text(stmt, 3, type.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        std::cerr << "Error inserting record: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        executeSQL("ROLLBACK;");
        return false;
    }

    sqlite3_finalize(stmt);
    executeSQL("COMMIT;");
    return true;
}

std::unique_ptr<FinanceEntry> DatabaseService::getRecordById(int id) const {
    const char *sql = "SELECT Description, Amount, Type FROM FinanceRecords WHERE ID = ?;";
    sqlite3_stmt *stmt = prepareStatement(sql);
    std::unique_ptr<FinanceEntry> record = nullptr;

    if (!stmt) {
        std::cerr << "Failed to prepare statement." << std::endl;
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        double amount = sqlite3_column_double(stmt, 1);
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

        if (type == "Income") {
            record = std::make_unique<IncomeRecord>(description, amount, id);
        } else if (type == "Expense") {
            record = std::make_unique<ExpenseRecord>(description, amount, id);
        }
    } else {
        std::cerr << "No record found with ID: " << id << std::endl;
    }

    sqlite3_finalize(stmt);
    return record;
}
