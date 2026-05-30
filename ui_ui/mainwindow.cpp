#include "mainwindow.h"
#include "sparse_matrix_model.hpp"
#include <QInputDialog>
#include <QHeaderView>

static QString globalStyle() {
    return
        "QMainWindow { background-color: #0f0518; }" 
        "QLabel { color: #d4b3ff; font-weight: bold; font-size: 13px; }"
        "QGroupBox { color: #d4b3ff; font-weight: bold; border: 1px solid #5a2e8c; border-radius: 5px; margin-top: 1ex; padding-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px; }"
        "QSpinBox, QComboBox { background: #261138; color: #f0e6ff; border: 1px solid #5a2e8c; border-radius: 4px; padding: 3px; font-weight: bold; }"
        "QComboBox QAbstractItemView { background: #261138; color: #f0e6ff; selection-background-color: #5a2e8c; }"
        "QPushButton { background: #6121a8; color: white; border: none; border-radius: 6px; padding: 6px 15px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background: #7c2de8; }"
        "QPushButton:pressed { background: #4a1980; }"
        "QTableWidget { background: #1a0b26; color: #e6ccff; gridline-color: #431f6e; border: 1px solid #5a2e8c; selection-background-color: #4a1980; }"
        "QHeaderView::section { background: #261138; color: #d4b3ff; border: 1px solid #431f6e; font-weight: bold; }";
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Sparse Matrix (Violet Edition)");
    resize(1000, 600);
    setStyleSheet(globalStyle());
    setupUI();
    registerEndpoints();
    updateGrids(); 
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QGroupBox *groupParams = new QGroupBox("ПАРАМЕТРЫ МАТРИЦЫ", this);
    QVBoxLayout *paramsLayout = new QVBoxLayout(groupParams);
    
    QHBoxLayout *rowColLayout = new QHBoxLayout();
    labelRowsA = new QLabel("Row:", this);
    spinRowsA = new QSpinBox(this);
    spinRowsA->setRange(1, 20); spinRowsA->setValue(3);
    
    labelColsA = new QLabel("Col:", this);
    spinColsA = new QSpinBox(this);
    spinColsA->setRange(1, 20); spinColsA->setValue(3);
    
    rowColLayout->addWidget(labelRowsA);
    rowColLayout->addWidget(spinRowsA);
    rowColLayout->addSpacing(15);
    rowColLayout->addWidget(labelColsA);
    rowColLayout->addWidget(spinColsA);
    rowColLayout->addStretch();
    
    QHBoxLayout *typeLayout = new QHBoxLayout();
    labelType = new QLabel("Тип данных:", this);
    comboType = new QComboBox(this);
    comboType->addItems({"int", "double", "Complex"});
    
    typeLayout->addWidget(labelType);
    typeLayout->addWidget(comboType);
    typeLayout->addStretch();
    
    paramsLayout->addLayout(rowColLayout);
    paramsLayout->addLayout(typeLayout);
    
    QGroupBox *groupControl = new QGroupBox("УПРАВЛЕНИЕ", this);
    QVBoxLayout *controlLayout = new QVBoxLayout(groupControl);
    
    QHBoxLayout *containerLayout = new QHBoxLayout();
    labelContainer = new QLabel("Контейнер:", this);
    comboContainer = new QComboBox(this);
    comboContainer->addItems({"ArraySequence", "ListSequence"});
    containerLayout->addWidget(labelContainer);
    containerLayout->addWidget(comboContainer);
    containerLayout->addStretch();
    
    QHBoxLayout *opLayout = new QHBoxLayout();
    labelOp = new QLabel("Операция:", this);
    comboOp = new QComboBox(this);
    comboOp->addItems({"Норма", "Умножить на скаляр", "Сложить матрицы", "Умножить матрицы"});
    opLayout->addWidget(labelOp);
    opLayout->addWidget(comboOp);
    opLayout->addStretch();
    
    controlLayout->addLayout(containerLayout);
    controlLayout->addLayout(opLayout);

    topLayout->addWidget(groupParams);
    topLayout->addWidget(groupControl);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    QHBoxLayout *midLayout = new QHBoxLayout();
    btnCompute = new QPushButton("Вычислить", this);
    btnCompute->setFixedSize(140, 35);
    labelResult = new QLabel("", this);
    labelResult->setStyleSheet("color: #00ffcc; font-size: 16px;");
    
    midLayout->addWidget(btnCompute);
    midLayout->addWidget(labelResult);
    midLayout->addStretch();
    mainLayout->addLayout(midLayout);

    QHBoxLayout *matricesLayout = new QHBoxLayout();
    
    QVBoxLayout *layoutA = new QVBoxLayout();
    labelA = new QLabel("Матрица A:", this);
    tableA = new QTableWidget(this);
    layoutA->addWidget(labelA);
    layoutA->addWidget(tableA);
    matricesLayout->addLayout(layoutA);

    widgetB = new QWidget(this);
    QVBoxLayout *layoutB = new QVBoxLayout(widgetB);
    layoutB->setContentsMargins(0,0,0,0);
    labelB = new QLabel("Матрица B:", this);
    tableB = new QTableWidget(this);
    layoutB->addWidget(labelB);
    layoutB->addWidget(tableB);
    matricesLayout->addWidget(widgetB);

    widgetResult = new QWidget(this);
    QVBoxLayout *layoutRes = new QVBoxLayout(widgetResult);
    layoutRes->setContentsMargins(0,0,0,0);
    labelResultMatrix = new QLabel("Результат:", this);
    tableResult = new QTableWidget(this);
    layoutRes->addWidget(labelResultMatrix);
    layoutRes->addWidget(tableResult);
    matricesLayout->addWidget(widgetResult);

    mainLayout->addLayout(matricesLayout);

    widgetB->hide();
    widgetResult->hide();

    connect(spinRowsA, &QSpinBox::valueChanged, this, &MainWindow::updateGrids);
    connect(spinColsA, &QSpinBox::valueChanged, this, &MainWindow::updateGrids);
    connect(comboOp, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateGrids);
    connect(btnCompute, &QPushButton::clicked, this, &MainWindow::onCompute);
}

void MainWindow::registerEndpoints() {
    endpoints["Норма"] = [this](SparseMatrixModel* m) { execNorm(m); };
    endpoints["Умножить на скаляр"] = [this](SparseMatrixModel* m) { execScalar(m); };
    endpoints["Сложить матрицы"] = [this](SparseMatrixModel* m) { execAdd(m); };
    endpoints["Умножить матрицы"] = [this](SparseMatrixModel* m) { execMult(m); };
}

void MainWindow::syncTableSize(QTableWidget *tbl, int rows, int cols) {
    int oldRows = tbl->rowCount();
    int oldCols = tbl->columnCount();
    
    tbl->setRowCount(rows);
    tbl->setColumnCount(cols);
    tbl->horizontalHeader()->setDefaultSectionSize(55);
    tbl->verticalHeader()->setDefaultSectionSize(30);
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if (i >= oldRows || j >= oldCols || !tbl->item(i, j)) {
                QTableWidgetItem *item = new QTableWidgetItem("0");
                item->setTextAlignment(Qt::AlignCenter);
                tbl->setItem(i, j, item);
            }
        }
    }
}

void MainWindow::updateGrids() {
    QString op = comboOp->currentText();
    int rA = spinRowsA->value();
    int cA = spinColsA->value();
    
    syncTableSize(tableA, rA, cA);
    widgetResult->hide();
    labelResult->clear();

    if (op == "Сложить матрицы") {
        labelB->setText(QString("Матрица B (%1x%2):").arg(rA).arg(cA));
        widgetB->show();
        syncTableSize(tableB, rA, cA);
    } 
    else if (op == "Умножить матрицы") {
        labelB->setText(QString("Матрица B (%1x%2):").arg(cA).arg(rA));
        widgetB->show();
        syncTableSize(tableB, cA, rA);
    } 
    else {
        widgetB->hide();
    }
}

bool MainWindow::validateTable(QTableWidget *tbl) {
    bool isComplex = (comboType->currentText() == "Complex");
    for(int i = 0; i < tbl->rowCount(); i++) {
        for(int j = 0; j < tbl->columnCount(); j++) {
            QTableWidgetItem *item = tbl->item(i, j);
            if (!item || item->text().trimmed().isEmpty()) continue;
            
            bool ok = true;
            if (!isComplex) {
                if (comboType->currentText() == "double") item->text().toDouble(&ok);
                else item->text().toInt(&ok);
            }

            if (!ok) {
                item->setBackground(QColor(150, 0, 0)); 
                return false;
            } else {
                item->setBackground(QColor(38, 17, 56)); 
            }
        }
    }
    return true;
}

void MainWindow::showModel(QTableWidget *tbl, SparseMatrixModel *model) {
    if (!model) return;
    int r = (int)model->GetRows();
    int c = (int)model->GetCols();
    
    tbl->clearContents();
    tbl->setRowCount(r);
    tbl->setColumnCount(c);
    
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            QString valStr = model->GetElementStr(i, j);
            QTableWidgetItem *item = new QTableWidgetItem(valStr);
            item->setTextAlignment(Qt::AlignCenter);
            
            if (valStr != "0") {
                item->setBackground(QColor(84, 27, 138)); 
            } else {
                item->setBackground(QColor(26, 11, 38)); 
            }
            tbl->setItem(i, j, item);
        }
    }
    widgetResult->show();
}

void MainWindow::onCompute() {
    if(!validateTable(tableA)) {
        QMessageBox::warning(this, "Ошибка", "Матрица A содержит некорректные значения!");
        return;
    }

    try {
        SparseMatrixModel *modelA = ModelFactory::instance().create(
            comboType->currentText(), 
            comboContainer->currentText(), 
            spinRowsA->value(), 
            spinColsA->value(), 
            tableA
        );

        labelResult->clear();
        QString op = comboOp->currentText();

        if (endpoints.count(op)) {
            endpoints[op](modelA);
        } else {
            QMessageBox::warning(this, "Ошибка", "Неизвестная операция!");
        }

        delete modelA;
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка выполнения", e.what());
    }
}

void MainWindow::execNorm(SparseMatrixModel *modelA) {
    labelResult->setText("Норма: " + modelA->NormStr());
}

void MainWindow::execScalar(SparseMatrixModel *modelA) {
    bool ok;
    QString prompt = (comboType->currentText() == "Complex") ? "Введите комплексный скаляр (напр. 2+3i):" : "Введите скаляр:";
    QString scalarStr = QInputDialog::getText(this, "Умножение", prompt, QLineEdit::Normal, "2", &ok);
    
    if (ok) {
        SparseMatrixModel *res = modelA->ScalarMultiplyStr(scalarStr);
        showModel(tableResult, res);
        delete res;
        labelResult->setText("Умножено на " + scalarStr);
    }
}

void MainWindow::execAdd(SparseMatrixModel *modelA) {
    if (!validateTable(tableB)) {
        QMessageBox::warning(this, "Ошибка", "Матрица B содержит некорректные значения!");
        return;
    }
    
    SparseMatrixModel *modelB = ModelFactory::instance().create(
        comboType->currentText(), 
        comboContainer->currentText(), 
        spinRowsA->value(), 
        spinColsA->value(), 
        tableB
    );
    
    SparseMatrixModel *res = modelA->Add(*modelB);
    showModel(tableResult, res);
    delete modelB;
    delete res;
    labelResult->setText("Матрицы успешно сложены");
}

void MainWindow::execMult(SparseMatrixModel *modelA) {
    if (!validateTable(tableB)) {
        QMessageBox::warning(this, "Ошибка", "Матрица B содержит некорректные значения!");
        return;
    }
    
    int cA = spinColsA->value();
    int cB = tableB->columnCount(); 
    
    SparseMatrixModel *modelB = ModelFactory::instance().create(
        comboType->currentText(), 
        comboContainer->currentText(), 
        cA, 
        cB, 
        tableB
    );
    
    SparseMatrixModel *res = modelA->Multiply(*modelB);
    showModel(tableResult, res);
    delete modelB;
    delete res;
    labelResult->setText("Матрицы успешно перемножены");
}