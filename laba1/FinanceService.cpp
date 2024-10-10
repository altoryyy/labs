#include "headers/FinanceService.h"
#include "FinanceRecord.cpp"
#include <format>

// Constructor
FinanceService::FinanceService()
{
    openDatabase();
}

// Destructor
FinanceService::~FinanceService()
{
    closeDatabase();
}

// Copy constructor
FinanceService::FinanceService(const FinanceService &other)
{
    if (other.db)
    {
        sqlite3_open("Records.db", &db); // Open new connection
        // Additional logic can be added here if needed
    }
}

// Move constructor
FinanceService::FinanceService(FinanceService &&other) noexcept : db(other.db)
{
    other.db = nullptr; // Leave other in a safe state
}

// Copy assignment operator
FinanceService &FinanceService::operator=(const FinanceService &other)
{
    if (this != &other)
    {
        closeDatabase(); // Clean up existing resource

        if (other.db)
        {
            sqlite3_open("Records.db", &db); // Open new connection
            // Additional copying logic can be added here if needed
        }
        else
        {
            db = nullptr; // Ensure db is null if other.db is null
        }
    }
    return *this;
}

// Move assignment operator
FinanceService &FinanceService::operator=(FinanceService &&other) noexcept
{
    if (this != &other)
    {
        closeDatabase();    // Clean up existing resource
        db = other.db;      // Transfer ownership
        other.db = nullptr; // Leave other in a safe state
    }
    return *this;
}

// Open database connection
void FinanceService::openDatabase()
{
    if (sqlite3_open("Records.db", &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;

        const char *sql = "CREATE TABLE IF NOT EXISTS FinanceRecords (ID INTEGER PRIMARY KEY AUTOINCREMENT, Description TEXT, Amount REAL);";
        char *errMsg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }
}

// Close database connection
void FinanceService::closeDatabase()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr; // Ensure db is set to nullptr after closing
    }
}

// Create a new record
void FinanceService::createRecord(const std::string &description, double amount)
{
    std::string sql = std::format("INSERT INTO FinanceRecords (Description, Amount) VALUES ('{}', {});", description, amount);
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Read all records
void FinanceService::readRecords() const
{
    const char *sql = "SELECT ID, Description, Amount FROM FinanceRecords;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        double amount = sqlite3_column_double(stmt, 2);
        FinanceRecord record(description, amount, id);
        record.display();
    }

    sqlite3_finalize(stmt);
}

// Update a specific record
void FinanceService::updateRecord(int id, const std::string &description, double amount)
{
    std::string sql = std::format("UPDATE FinanceRecords SET Description = '{}', Amount = {} WHERE ID = {};",
                                  description, amount, id);
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Delete a record by ID
void FinanceService::deleteRecord(int id)
{
    std::string sql = std::format("DELETE FROM FinanceRecords WHERE ID = {}; ", id);
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Calculate the total balance
double FinanceService::calculateTotalBalance() const
{
    double total = 0;
    const char *sql = "SELECT SUM(Amount) FROM FinanceRecords;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return total;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

// Clear all records from the database
void FinanceService::clearRecords()
{
    char *errMsg = nullptr;
    const char *sqlDelete = "DELETE FROM FinanceRecords;";
    const char *sqlReset = "DELETE FROM sqlite_sequence WHERE name='FinanceRecords';";

    if (sqlite3_exec(db, sqlDelete, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, sqlReset, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Get a record by ID
FinanceRecord FinanceService::getRecordById(int id) const
{
    FinanceRecord record("", 0.0, id);
    const char *sql = "SELECT Description, Amount FROM FinanceRecords WHERE ID = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return record;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
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