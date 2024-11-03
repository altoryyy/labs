    #include "MainWindow.h"
    #include "ui_MainWindow.h"
    #include "DatabaseService.h"
    #include "FinanceService.h"
    #include <QtCharts/QChartView>
    #include <QtCharts/QPieSeries>
    #include <QtCharts/QChart>

    //using namespace QtCharts;

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), financeService(0.0) {
        ui->setupUi(this);
        setFixedSize(757, 532);

        incomeListWidget = ui->incomeListWidget;
        expenseListWidget = ui->expenseListWidget;
        budgetSummaryLabel = new QLabel(this); // Initialize the budget summary label
        budgetSummaryLabel->setGeometry(280, 50, 200, 80);
        budgetSummaryLabel->setAlignment(Qt::AlignCenter);
        budgetSummaryLabel->setStyleSheet("font-size: 14px; color: gray;"); // Set style for the label

        connect(ui->setTargetBudgetButton, &QPushButton::clicked, this, &MainWindow::setTargetBudget);
        connect(ui->addIncomeButton, &QPushButton::clicked, this, &MainWindow::addIncome);
        connect(ui->addExpenseButton, &QPushButton::clicked, this, &MainWindow::addExpense);
        connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearAllRecords);
        connect(ui->updateRecordButton, &QPushButton::clicked, this, &MainWindow::updateRecord);

        // Connect the signal for expenses added
        connect(this, &MainWindow::expensesAdded, this, &MainWindow::createPieChart);

        loadRecords();
        createPieChart();
        updateBudgetSummary(); // Update budget summary when the window is created
    }

    MainWindow::~MainWindow() {
        DatabaseService::getInstance().closeDatabase();
        delete ui;
    }

    void MainWindow::clearAllRecords() {
        try {
            financeService.clearRecords();
            loadRecords();
            updateBudgetSummary(); // Update budget summary after clearing records
            QMessageBox::information(this, "Success", "All records have been cleared.");
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error", "An error occurred: " + QString::fromStdString(e.what()));
        }
    }

    void MainWindow::setTargetBudget() {
        bool ok;
        QInputDialog inputDialog(this);
        inputDialog.setWindowTitle("Set Target Budget");
        inputDialog.setLabelText("Enter target budget:");
        inputDialog.setDoubleRange(0, 1000000);
        // Указываем количество десятичных знаков непосредственно в getDouble()
        double amount = QInputDialog::getDouble(this, "Set Target Budget", "Enter target budget:", 0, 0, 1000000, 2, &ok);
        if (ok) {
            try {
                financeService.setTargetBudget(amount);
                updateBudgetSummary(); // Update budget summary after setting the budget
                QMessageBox::information(this, "Success", "Target budget set to " + QString::number(amount, 'f', 2));
            } catch (const std::exception &e) {
                QMessageBox::critical(this, "Error", "Failed to set target budget: " + QString::fromStdString(e.what()));
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
                    financeService.createIncome(description.toStdString(), amount);
                    loadRecords();
                    updateBudgetSummary(); // Update budget summary after adding income
                    QMessageBox::information(this, "Success", "Income added: " + description + " - " + QString::number(amount, 'f', 2));
                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Error", "Failed to add income: " + QString::fromStdString(e.what()));
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
                    financeService.createExpense(description.toStdString(), amount);
                    loadRecords();
                    updateBudgetSummary(); // Update budget summary after adding expense
                    emit expensesAdded(); // Emit signal to update the pie chart
                    QMessageBox::information(this, "Success", "Expense added: " + description + " - " + QString::number(amount, 'f', 2));
                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Error", "Failed to add expense: " + QString::fromStdString(e.what()));
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
                    // Здесь вызовите метод вашего сервиса для обновления записи
                    financeService.updateRecord(recordId, newDescription.toStdString(), newAmount);
                    loadRecords(); // Обновите список после изменения
                    updateBudgetSummary(); // Обновите сводку бюджета
                    QMessageBox::information(this, "Success", "Record updated.");
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
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Error", "Failed to load records: " + QString::fromStdString(e.what()));
        }
    }

    void MainWindow::updateBudgetSummary() {
        try {
            QString budgetSummary = financeService.getBudgetSummary(); // Получаем сводку бюджета
            budgetSummaryLabel->setText(budgetSummary); // Устанавливаем текст в метку
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить сводку бюджета: " + QString::fromStdString(e.what()));
        }
    }


    void MainWindow::createPieChart() {
        // Удаляем все виджеты из layout
        QLayoutItem *item;
        while ((item = ui->chartLayout->takeAt(0)) != nullptr) {
            delete item->widget(); // Удаляем виджет
            delete item; // Удаляем сам элемент layout
        }

        // Создание серии для круговой диаграммы
        QPieSeries *series = new QPieSeries();

        // Получаем записи расходов из базы данных
        auto records = financeService.getRecords();

        // Перебираем записи и добавляем их в диаграмму
        QStringList colors = {
            "#007BFF", // Основной цвет (синий)
            "#339FFF", // Светлый оттенок 1
            "#66B3FF", // Светлый оттенок 2
            "#99D6FF", // Светлый оттенок 3
            "#0066CC", // Темный оттенок 1
            "#005BB5", // Темный оттенок 2
            "#004999"  // Темный оттенок 3
        };

        for (const auto &record : records) {
            if (record->getType() == "Expense") {
                // Добавляем запись в серию и устанавливаем цвет
                auto slice = series->append(QString::fromStdString(record->getDescription()), record->getAmount());
                // Установка цвета для каждого сектора
                slice->setBrush(QBrush(QColor(colors[series->count() % colors.size()])));
                // Установка метки с названием и процентом
                slice->setLabel(QString("%1\n%2%").arg(QString::fromStdString(record->getDescription())).arg(QString::number(slice->percentage() * 100, 'f', 1)));
                slice->setLabelBrush(QBrush(Qt::white)); // Установка цвета меток на белый
            }
        }

        // Проверка, есть ли данные для отображения
        if (series->count() == 0) {
            QMessageBox::information(this, "Нет данных", "Нет доступных записей расходов для отображения.");
            return; // Выходим из функции
        }

        // Включаем отображение меток на секторах
        series->setLabelsVisible(true); // Показываем метки для секторов


        // Создание диаграммы
        QChart *chart = new QChart();
        chart->addSeries(series);

        // Убираем заголовок диаграммы
        chart->setTitle(""); // Убираем заголовок

        // Убираем легенду
        chart->legend()->setVisible(false); // Убираем легенду

        // Установка прозрачного фона для диаграммы
        chart->setBackgroundBrush(Qt::transparent); // Прозрачный фон для диаграммы
        chart->setPlotAreaBackgroundBrush(Qt::transparent); // Прозрачный фон для области диаграммы
        chart->setPlotAreaBackgroundPen(QPen(Qt::transparent)); // Убираем границу области диаграммы

        // Создание виджета для отображения диаграммы
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setBackgroundBrush(Qt::transparent); // Прозрачный фон для виджета

        // Установка диаграммы в нужный контейнер в вашем интерфейсе
        ui->chartLayout->addWidget(chartView);

        // Установка прозрачного фона для родительского виджета
        QWidget *parentWidget = ui->chartLayout->parentWidget();
        if (parentWidget) {
            parentWidget->setStyleSheet("background: transparent;");
        }

        ui->chartLayout->update();
    }
