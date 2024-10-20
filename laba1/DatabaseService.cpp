#include "headers/DatabaseService.h"
#include "FinanceRecord.cpp"

DatabaseService::DatabaseService() {}

void DatabaseService::openDatabase(const std::string &dbName)
{
    if (sqlite3_open(dbName.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;
        executeSQL("CREATE TABLE IF NOT EXISTS FinanceRecords (ID INTEGER PRIMARY KEY AUTOINCREMENT, Description TEXT, Amount REAL);");
    }
}

void DatabaseService::closeDatabase()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

void DatabaseService::executeSQL(const std::string &sql)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

sqlite3_stmt *DatabaseService::prepareStatement(const std::string &sql) const
{
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return stmt;
}

void DatabaseService::clearRecords()
{
    executeSQL("DELETE FROM FinanceRecords;");
    executeSQL("DELETE FROM sqlite_sequence WHERE name='FinanceRecords';");
}

FinanceRecord DatabaseService::getRecordById(int id) const
{
    FinanceRecord record("", 0.0, id);
    const char *sql = "SELECT Description, Amount FROM FinanceRecords WHERE ID = ?;";
    sqlite3_stmt *stmt = prepareStatement(sql);

    sqlite3_bind_int(stmt, 1, id);

    if (stmt && sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        double amount = sqlite3_column_double(stmt, 1);
        record = FinanceRecord(description, amount, id);
    }
    else
    {
        std::cerr << "No record found with ID: " << id << std::endl;
    }

    sqlite3_finalize(stmt);
    return record;
}