#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DatabaseService.h"
#include "FinanceService.h"
#include "CustomException.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), financeService(0.0), actionManager(financeService) {
    ui->setupUi(this);
    setFixedSize(757, 532);

    incomeListWidget = ui->incomeListWidget;
    expenseListWidget = ui->expenseListWidget;
    budgetSummaryLabel = new QLabel(this);
    budgetSummaryLabel->setGeometry(280, 50, 200, 80);
    budgetSummaryLabel->setAlignment(Qt::AlignCenter);
    budgetSummaryLabel->setStyleSheet("font-size: 14px; color: gray;");

    connect(ui->setTargetBudgetButton, &QPushButton::clicked, this, &MainWindow::setTargetBudget);
    connect(ui->addIncomeButton, &QPushButton::clicked, this, &MainWindow::addIncome);
    connect(ui->addExpenseButton, &QPushButton::clicked, this, &MainWindow::addExpense);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearAllRecords);
    connect(ui->updateRecordButton, &QPushButton::clicked, this, &MainWindow::updateRecord);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::undoLastAction);
    connect(this, &MainWindow::expensesAdded, this, &MainWindow::createPieChart);

    loadRecords();
    createPieChart();
    updateBudgetSummary();
}

MainWindow::~MainWindow() {
    DatabaseService::getInstance().closeDatabase();
    delete ui;
}

void MainWindow::clearAllRecords() {
    try {
        financeService.clearRecords();
        loadRecords();
        updateBudgetSummary();
        createPieChart();
        QMessageBox::information(this, "Success", "All records have been cleared.");
    } catch (const CustomException &e) {
        QMessageBox::critical(this, "Error", "An error occurred: " + QString::fromStdString(e.what()));
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", "An unexpected error occurred: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::setTargetBudget() {
    bool ok;
    double amount = QInputDialog::getDouble(this, "Установить целевой бюджет", "Введите целевой бюджет:", 0, 0, 1000000, 2, &ok);
    if (ok) {
        try {
            financeService.setTargetBudget(amount);
            financeService.loadBudget();
            updateBudgetSummary();
            QMessageBox::information(this, "Успех", "Целевой бюджет установлен на " + QString::number(amount, 'f', 2));
        } catch (const CustomException &e) {
            QMessageBox::critical(this, "Ошибка", "Не удалось установить целевой бюджет: " + QString::fromStdString(e.what()));
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Ошибка", "Неизвестная ошибка: " + QString::fromStdString(e.what()));
        }
    }
}

void MainWindow::addIncome() {
    bool ok;
    QInputDialog inputDialog(this);
    inputDialog.setWindowTitle("Income Description");
    inputDialog.setLabelText("Enter description:");
    inputDialog.setStyleSheet("background-color: white; color: black; QLineEdit { background-color: white; color: black; } QLabel { color: black; }");

    QString description = inputDialog.getText(this, "Income Description", "Enter description:", QLineEdit::Normal, QString(), &ok);
    if (ok && !description.isEmpty()) {
        double amount = QInputDialog::getDouble(this, "Income Amount", "Enter amount:", 0, 0, 100000, 2, &ok);
        if (ok) {
            try {
                actionManager.addIncome(description.toStdString(), amount);
                loadRecords();
                updateBudgetSummary();
                QMessageBox::information(this, "Success", "Income added: " + description + " - " + QString::number(amount, 'f', 2));
            } catch (const CustomException &e) {
                QMessageBox::critical(this, "Error", "Failed to add income: " + QString::fromStdString(e.what()));
            } catch (const std::exception &e) {
                QMessageBox::critical(this, "Error", "Unexpected error: " + QString::fromStdString(e.what()));
            }
        }
    }
}

void MainWindow::addExpense() {
    bool ok;
    QInputDialog inputDialog(this);
    inputDialog.setWindowTitle("Expense Description");
    inputDialog.setLabelText("Enter description:");
    inputDialog.setStyleSheet("background-color: white; color: black; QLineEdit { background-color: white; color: black; } QLabel { color: black; }");

    QString description = inputDialog.getText(this, "Expense Description", "Enter description:", QLineEdit::Normal, QString(), &ok);
    if (ok && !description.isEmpty()) {
        double amount = QInputDialog::getDouble(this, "Expense Amount", "Enter amount:", 0, 0, 100000, 2, &ok);
        if (ok) {
            try {
                actionManager.addExpense(description.toStdString(), amount);
                loadRecords();
                updateBudgetSummary();
                emit expensesAdded();
                QMessageBox::information(this, "Success", "Expense added: " + description + " - " + QString::number(amount, 'f', 2));
            } catch (const CustomException &e) {
                QMessageBox::critical(this, "Error", "Failed to add expense: " + QString::fromStdString(e.what()));
            } catch (const std::exception &e) {
                QMessageBox::critical(this, "Error", "Unexpected error: " + QString::fromStdString(e.what()));
            }
        }
    }
}

void MainWindow::updateRecord() {
    bool ok;
    QInputDialog inputDialog(this);
    inputDialog.setWindowTitle("Update Record");
    inputDialog.setLabelText("Enter record ID to update:");
    inputDialog.setStyleSheet("background-color: white; color: black; QLineEdit { background-color: white; color: black; } QLabel { color: black; }");

    int recordId = inputDialog.getInt(this, "Update Record", "Enter ID of the record to update:", 0, 0, 1000000, 1, &ok);
    if (ok) {
        QString newDescription = QInputDialog::getText(this, "New Description", "Enter new description:", QLineEdit::Normal, QString(), &ok);
        if (ok && !newDescription.isEmpty()) {
            double newAmount = QInputDialog::getDouble(this, "New Amount", "Enter new amount:", 0, 0, 1000000, 2, &ok);
            if (ok) {
                try {
                    financeService.updateRecord(recordId, newDescription.toStdString(), newAmount);
                    loadRecords();
                    updateBudgetSummary();
                    QMessageBox::information(this, "Success", "Record updated.");
                } catch (const CustomException &e) {
                    QMessageBox::critical(this, "Error", "Failed to update record: " + QString::fromStdString(e.what()));
                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Error", "Unexpected error: " + QString::fromStdString(e.what()));
                }
            }
        }
    }
}

void MainWindow::loadRecords() {
    incomeListWidget->clear();
    expenseListWidget->clear();

    try {
        auto records = financeService.getRecords();
        for (const auto &record : records) {
            QString recordText = QString("%1: %2 - %3")
            .arg(QString::number(record->getId()))
                .arg(QString::fromStdString(record->getDescription()))
                .arg(QString::number(record->getAmount(), 'f', 2));

            if (record->getType() == "Income") {
                auto *item = new QListWidgetItem(recordText);
                incomeListWidget->addItem(item);
            } else if (record->getType() == "Expense") {
                auto *item = new QListWidgetItem(recordText);
                expenseListWidget->addItem(item);
            }
        }
    } catch (const CustomException &e) {
        QMessageBox::warning(this, "Error", "Failed to load records: " + QString::fromStdString(e.what()));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", "Unexpected error: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::updateBudgetSummary() {
    try {
        QString budgetSummary = financeService.getBudgetSummary();
        budgetSummaryLabel->setText(budgetSummary);
    } catch (const CustomException &e) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить сводку бюджета: " + QString::fromStdString(e.what()));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Ошибка", "Неизвестная ошибка: " + QString::fromStdString(e.what()));
    }
}

void MainWindow::createPieChart() {
    QLayoutItem *item;
    while ((item = ui->chartLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QPieSeries *series = new QPieSeries();

    auto records = financeService.getRecords();
    QMap<QString, double> expenseMap;

    QStringList colors = {
        "#007BFF",
        "#339FFF",
        "#66B3FF",
        "#99D6FF",
        "#0066CC",
        "#005BB5",
        "#004999"
    };

    for (const auto &record : records) {
        if (record->getType() == "Expense") {
            QString description = QString::fromStdString(record->getDescription());
            double amount = record->getAmount();
            expenseMap[description] += amount;
        }
    }

    for (auto it = expenseMap.constBegin(); it != expenseMap.constEnd(); ++it) {
        auto slice = series->append(it.key(), it.value());
        slice->setBrush(QBrush(QColor(colors[series->count() % colors.size()])));
        slice->setLabel(QString("%1\n%2%").arg(it.key()).arg(QString::number(slice->percentage() * 100, 'f', 1)));
        slice->setLabelBrush(QBrush(Qt::white));
    }

    if (series->count() == 0) {
        QMessageBox::information(this, "Нет данных", "Нет доступных записей расходов для отображения.");
        return;
    }

    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->setTitle("");

    chart->legend()->setVisible(false);

    chart->setBackgroundBrush(Qt::transparent);
    chart->setPlotAreaBackgroundBrush(Qt::transparent);
    chart->setPlotAreaBackgroundPen(QPen(Qt::transparent));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(Qt::transparent);

    ui->chartLayout->addWidget(chartView);

    QWidget *parentWidget = ui->chartLayout->parentWidget();
    if (parentWidget) {
        parentWidget->setStyleSheet("background: transparent;");
    }

    ui->chartLayout->update();
}

void MainWindow::undoLastAction() {
    try {
        actionManager.undo();
        loadRecords();
        updateBudgetSummary();
        createPieChart();
        QMessageBox::information(this, "Undo", "Last action undone.");
    } catch (const CustomException &e) {
        QMessageBox::critical(this, "Error", "Failed to undo action: " + QString::fromStdString(e.what()));
    }
}
