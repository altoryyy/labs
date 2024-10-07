#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include <string>
#include <iostream>
#include <sqlite3.h>
#include "FinanceRecord.h"

class FinanceService
{
    sqlite3 *db;

public:
    FinanceService();
    ~FinanceService();

    void createRecord(const std::string &description, double amount);
    void readRecords() const;
    void updateRecord(int id, const std::string &description, double amount);
    void deleteRecord(int id);
    double calculateTotalBalance() const;
    void clearRecords();
    FinanceRecord getRecordById(int id) const; // Прототип нового метода

    inline void openDatabase();
    inline void closeDatabase();
};

#endif // FINANCESERVICE_H