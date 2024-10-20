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
    sqlite3 *db;

public:
    DatabaseService();
    ~DatabaseService();

    void openDatabase(const std::string &dbName);
    void closeDatabase();
    void clearRecords();

    FinanceRecord getRecordById(int id) const;

public:
    void executeSQL(const std::string &sql);
    sqlite3_stmt *prepareStatement(const std::string &sql) const;
};

#endif