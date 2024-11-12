#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "FinanceService.h"
#include "Stack.h"
#include "CustomException.h"
#include <string>
#include <memory>

struct Action {
    enum class Type { AddIncome, AddExpense, DeleteRecord };
    Type type;
    std::string description;
    double amount;
    int recordId;
};

class ActionManager {
public:
    ActionManager(FinanceService &service) : financeService(service) {}

    void addIncome(const std::string &description, double amount) {

        int recordId = financeService.createIncome(description, amount);
        actionStack.push(Action{Action::Type::AddIncome, description, amount, recordId});
    }

    void addExpense(const std::string &description, double amount) {

        int recordId = financeService.createExpense(description, amount);
        actionStack.push(Action{Action::Type::AddExpense, description, amount, recordId});
    }

    void deleteRecord(int id) {
        auto record = financeService.getRecordById(id);
        actionStack.push(Action{Action::Type::DeleteRecord, record->getDescription(), record->getAmount(), id});
        financeService.deleteRecord(id);
    }

    void undo() {
        if (actionStack.isEmpty()) {
            throw CustomException("Нет действий для отмены.");
        }

        Action lastAction = actionStack.top();
        actionStack.pop();

        switch (lastAction.type) {
        case Action::Type::AddIncome:
        case Action::Type::AddExpense:
            financeService.deleteRecord(lastAction.recordId);
            break;
        case Action::Type::DeleteRecord:
            financeService.createExpense(lastAction.description, lastAction.amount);
            break;
        }
    }

private:
    FinanceService &financeService;
    Stack<Action> actionStack;
};

#endif
