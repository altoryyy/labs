#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include "FinanceService.h"
#include <string>

class ReportService {
public:
    ReportService(const FinanceService& financeService);
    void generateJsonReport(const std::string& reportType) const;
    void generateDescriptionReport(const std::string& description) const;

private:
    const FinanceService& financeService;
    std::vector<FinanceEntry*> filterByDescription(const std::string& description) const;
};

#endif
