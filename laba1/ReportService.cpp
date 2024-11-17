#include "ReportService.h"
#include "CustomException.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

ReportService::ReportService(const FinanceService& financeService)
    : financeService(financeService) {}

void ReportService::generateJsonReport(const std::string& reportType) const {
    std::vector<std::unique_ptr<FinanceEntry>> records = financeService.getRecords();

    std::string jsonContent = "{\n  \"reportType\": \"" + reportType + "\",\n  \"records\": [\n";

    bool hasRecords = false;

    if (reportType == "Бюджет") {
        QString budgetSummary = financeService.getBudgetSummary();
        jsonContent += "    {\n      \"summary\": \"" + budgetSummary.toStdString() + "\"\n    }\n";
        jsonContent += "  ]\n}";
        std::ofstream outFile("Бюджет_Отчет.json");
        if (outFile.is_open()) {
            outFile << jsonContent;
            outFile.close();
            std::cout << "Отчет сохранен в Бюджет_Отчет.json" << std::endl;
        } else {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
        return;
    }

    for (const auto& record : records) {
        if (record && ((reportType == "Доходы" && record->getType() == "Income") ||
                       (reportType == "Расходы" && record->getType() == "Expense"))) {
            jsonContent += "    {\n";
            jsonContent += "      \"id\": " + std::to_string(record->getId()) + ",\n";
            jsonContent += "      \"description\": \"" + record->getDescription() + "\",\n";
            jsonContent += "      \"amount\": " + std::to_string(record->getAmount()) + "\n";
            jsonContent += "    },\n";
            hasRecords = true;
        }
    }

    if (hasRecords) {
        jsonContent.pop_back();
        jsonContent.pop_back();
    }

    jsonContent += "  ]\n}";

    if (!hasRecords) {
        jsonContent = "{\n  \"reportType\": \"" + reportType + "\",\n  \"message\": \"Нет записей для данного типа.\"\n}";
    }

    std::ofstream outFile(reportType + "_Отчет.json");
    if (outFile.is_open()) {
        outFile << jsonContent;
        outFile.close();
        std::cout << "Отчет сохранен в " << reportType << "_Отчет.json" << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }
}
void ReportService::generateDescriptionReport(const std::string& description) const {
    std::vector<std::unique_ptr<FinanceEntry>> records = financeService.getRecords();

    std::string jsonContent = "{\n  \"description\": \"" + description + "\",\n  \"records\": [\n";

    bool hasRecords = false;

    for (const auto& record : records) {
        if (record && record->getDescription().find(description) != std::string::npos) {
            jsonContent += "    {\n";
            jsonContent += "      \"id\": " + std::to_string(record->getId()) + ",\n";
            jsonContent += "      \"description\": \"" + record->getDescription() + "\",\n";
            jsonContent += "      \"amount\": " + std::to_string(record->getAmount()) + "\n";
            jsonContent += "    },\n";
            hasRecords = true;
        }
    }

    if (hasRecords) {
        jsonContent.pop_back();
        jsonContent.pop_back();
    }

    jsonContent += "  ]\n}";

    if (!hasRecords) {
        jsonContent = "{\n  \"description\": \"" + description + "\",\n  \"message\": \"Нет записей для данного описания.\"\n}";
    }

    std::ofstream outFile("Description_Report.json");
    if (outFile.is_open()) {
        outFile << jsonContent;
        outFile.close();
        std::cout << "Отчет сохранен в Description_Report.json" << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }
}

