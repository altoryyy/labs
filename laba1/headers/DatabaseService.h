#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <sqlite3.h>
#include <string>
#include <memory>
#include "FinanceEntry.h"
#include "ExpenseRecord.h"
#include "IncomeRecord.h"

class DatabaseService {
private:
    sqlite3 *db = nullptr;
    bool isOpen = false;

    DatabaseService() = default;

public:
    DatabaseService(const DatabaseService &) = delete;
    DatabaseService &operator=(const DatabaseService &) = delete;

    DatabaseService(DatabaseService &&other) noexcept = default;
    DatabaseService &operator=(DatabaseService &&other) noexcept = default;

    static inline DatabaseService& getInstance() {
        return instance;
    }

    bool openDatabase(const std::string &dbName);
    void closeDatabase();
    void clearRecords() const;
    bool createRecord(const std::string &description, double amount, const std::string &type) const;
    std::unique_ptr<FinanceEntry> getRecordById(int id) const;
    void executeSQL(const std::string &sql) const;
    sqlite3_stmt *prepareStatement(const std::string &sql) const;

    sqlite3 *getDb() const { return db; }

    void createBudgetTable();
};

#endif // DATABASESERVICE_H
