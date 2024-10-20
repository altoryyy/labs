#include "headers/FinanceService.h"
#include "DatabaseService.cpp"

FinanceService::FinanceService()
{
    dbService.openDatabase("Records.db");
}

void FinanceService::createRecord(const std::string &description, double amount)
{
    std::string sql = std::format("INSERT INTO FinanceRecords (Description, Amount) VALUES ('{}', {});", description, amount);
    dbService.executeSQL(sql);
}

void FinanceService::readRecords() const
{
    const char *sql = "SELECT ID, Description, Amount FROM FinanceRecords;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);

    while (stmt && sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        FinanceRecord record(description, amount, id);
        record.display();
    }

    sqlite3_finalize(stmt);
}

void FinanceService::updateRecord(int id, const std::string &description, double amount)
{
    std::string sql = std::format("UPDATE FinanceRecords SET Description = '{}', Amount = {} WHERE ID = {};",
                                  description, amount, id);
    dbService.executeSQL(sql);
}

void FinanceService::deleteRecord(int id)
{
    std::string sql = std::format("DELETE FROM FinanceRecords WHERE ID = {}; ", id);
    dbService.executeSQL(sql);
}

double FinanceService::calculateTotalBalance() const
{
    double total = 0;
    const char *sql = "SELECT SUM(Amount) FROM FinanceRecords;";
    sqlite3_stmt *stmt = dbService.prepareStatement(sql);

    if (stmt && sqlite3_step(stmt) == SQLITE_ROW)
    {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

void FinanceService::clearRecords()
{
    dbService.clearRecords();
}

FinanceRecord FinanceService::getRecordById(int id) const
{
    return dbService.getRecordById(id);
}