#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include "FinanceRecord.h"
#include <format>

class DatabaseService
{
private:
    sqlite3 *db = nullptr;

public:
    DatabaseService();
    ~DatabaseService();

    DatabaseService(const DatabaseService &) = delete;
    DatabaseService &operator=(const DatabaseService &) = delete;

    DatabaseService(DatabaseService &&other) noexcept;
    DatabaseService &operator=(DatabaseService &&other) noexcept;

    void openDatabase(const std::string &dbName);
    void closeDatabase();
    void clearRecords();

    FinanceRecord getRecordById(int id) const;

    void executeSQL(const std::string &sql);
    sqlite3_stmt *prepareStatement(const std::string &sql) const;
};

#endif