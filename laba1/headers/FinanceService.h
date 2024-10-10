#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include <string>
#include <iostream>
#include <sqlite3.h>
#include "FinanceRecord.h"
#include <iomanip>

class FinanceService
{
private:
    sqlite3 *db = nullptr;

public:
    FinanceService();
    ~FinanceService();

    FinanceService(const FinanceService &other);

    FinanceService(FinanceService &&other) noexcept;

    FinanceService &operator=(const FinanceService &other);

    FinanceService &operator=(FinanceService &&other) noexcept;

    void createRecord(const std::string &description, double amount);
    void readRecords() const;
    void updateRecord(int id, const std::string &description, double amount);
    void deleteRecord(int id);
    double calculateTotalBalance() const;
    void clearRecords();
    FinanceRecord getRecordById(int id) const;

    void openDatabase();
    void closeDatabase();
};

#endif