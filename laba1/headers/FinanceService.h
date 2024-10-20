#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include <string>
#include "DatabaseService.h"
#include <iomanip>
#include "FinanceRecord.h"
#include "sqlite3.h"

class FinanceService
{
private:
    DatabaseService dbService;
    void executeSQL(const std::string &sql);

public:
    FinanceService();
    void createRecord(const std::string &description, double amount);
    void readRecords() const;
    void updateRecord(int id, const std::string &description, double amount);
    void deleteRecord(int id);
    double calculateTotalBalance() const;
    void clearRecords();
    FinanceRecord getRecordById(int id) const;
};

#endif