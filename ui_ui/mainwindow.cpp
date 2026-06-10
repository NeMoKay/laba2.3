#include "mainwindow.h"
#include "resources.hpp"
#include "SparseMatrix.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Complex.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QHeaderView>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(APP_TITLE);
    resize(1100, 800);
    setStyleSheet(mainStyle());

    tabs = new QTabWidget(this);
    tabs->setGeometry(0, 0, 1100, 800);

    tabOps = new QWidget();
    tabs->addTab(tabOps, TAB_OPERATIONS);

    setupOperationsTab();
}

MainWindow::~MainWindow()
{
}

QString MainWindow::mainStyle(){
    return STYLE_MAIN;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(30, 30, 30));
    QMainWindow::paintEvent(event);
}

void MainWindow::normalizeTable(QTableWidget* table){
    table->horizontalHeader()->setMinimumSectionSize(60);
    table->horizontalHeader()->setDefaultSectionSize(60);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table->verticalHeader()->setMinimumSectionSize(30);
    table->verticalHeader()->setDefaultSectionSize(30);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

void MainWindow::setupOperationsTab(){
    QVBoxLayout *lay = new QVBoxLayout(tabOps);
    lay->setSpacing(12);
    lay->setContentsMargins(16, 16, 16, 16);

    groupSettings = new QGroupBox(TEXT_SETTINGS);
    QFormLayout *setLay = new QFormLayout(groupSettings);
    
    comboDataType = new QComboBox();
    comboDataType->addItems({TYPE_INT, TYPE_DOUBLE, TYPE_COMPLEX});
    comboContainer = new QComboBox();
    comboContainer->addItems({CONT_ARRAY, CONT_LIST});
    comboOperation = new QComboBox();
    comboOperation->addItems({OP_ADD, OP_MULT, OP_SCALAR, OP_NORM});
    
    setLay->addRow(TEXT_DATA_TYPE, comboDataType);
    setLay->addRow(TEXT_CONTAINER, comboContainer);
    setLay->addRow(TEXT_OPERATION, comboOperation);
    lay->addWidget(groupSettings);

    QHBoxLayout *matricesLayout = new QHBoxLayout();

    groupMatrixA = new QGroupBox(TEXT_MATRIX_A);
    QVBoxLayout *aLay = new QVBoxLayout(groupMatrixA);
    QHBoxLayout *dimLayA = new QHBoxLayout();
    spinRowsA = new QSpinBox(); spinRowsA->setRange(1, 100); spinRowsA->setValue(3);
    spinColsA = new QSpinBox(); spinColsA->setRange(1, 100); spinColsA->setValue(3);
    dimLayA->addWidget(new QLabel(TEXT_ROWS));
    dimLayA->addWidget(spinRowsA);
    dimLayA->addWidget(new QLabel(TEXT_COLS));
    dimLayA->addWidget(spinColsA);
    dimLayA->addStretch();
    aLay->addLayout(dimLayA);
    tableA = new QTableWidget(3, 3);
    normalizeTable(tableA);
    aLay->addWidget(tableA);
    matricesLayout->addWidget(groupMatrixA);

    groupMatrixB = new QGroupBox(TEXT_MATRIX_B);
    QVBoxLayout *bLay = new QVBoxLayout(groupMatrixB);
    QHBoxLayout *dimLayB = new QHBoxLayout();
    spinRowsB = new QSpinBox(); spinRowsB->setRange(1, 100); spinRowsB->setValue(3);
    spinColsB = new QSpinBox(); spinColsB->setRange(1, 100); spinColsB->setValue(3);
    dimLayB->addWidget(new QLabel(TEXT_ROWS));
    dimLayB->addWidget(spinRowsB);
    dimLayB->addWidget(new QLabel(TEXT_COLS));
    dimLayB->addWidget(spinColsB);
    dimLayB->addStretch();
    bLay->addLayout(dimLayB);
    tableB = new QTableWidget(3, 3);
    normalizeTable(tableB);
    bLay->addWidget(tableB);
    matricesLayout->addWidget(groupMatrixB);

    lay->addLayout(matricesLayout);

    groupScalar = new QGroupBox(TEXT_SCALAR);
    QFormLayout *sLay = new QFormLayout(groupScalar);
    lineScalar = new QLineEdit();
    sLay->addRow(TEXT_SCALAR, lineScalar);
    groupScalar->setVisible(false);
    lay->addWidget(groupScalar);

    btnRun = new QPushButton(TEXT_RUN);
    lay->addWidget(btnRun);

    groupResult = new QGroupBox(TEXT_GROUP_RESULT);
    QVBoxLayout *resLay = new QVBoxLayout(groupResult);
    labelResultTitle = new QLabel(TEXT_RESULT_LBL);
    labelResultTitle->setStyleSheet(STYLE_LABEL_RESULT);
    
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(6);
    shadow->setColor(Qt::black);
    shadow->setOffset(1, 1);
    labelResultTitle->setGraphicsEffect(shadow);
    resLay->addWidget(labelResultTitle);
    
    textResult = new QTextEdit();
    textResult->setReadOnly(true);
    textResult->setVisible(false);
    resLay->addWidget(textResult);

    tableResult = new QTableWidget();
    tableResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    normalizeTable(tableResult);
    resLay->addWidget(tableResult);
    
    lay->addWidget(groupResult);

    connect(spinRowsA, &QSpinBox::valueChanged, this, [this](int r){ tableA->setRowCount(r); });
    connect(spinColsA, &QSpinBox::valueChanged, this, [this](int c){ tableA->setColumnCount(c); });
    connect(spinRowsB, &QSpinBox::valueChanged, this, [this](int r){ tableB->setRowCount(r); });
    connect(spinColsB, &QSpinBox::valueChanged, this, [this](int c){ tableB->setColumnCount(c); });

    connect(comboOperation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onOperationChanged);
    connect(btnRun, &QPushButton::clicked, this, &MainWindow::onRun);
}

void MainWindow::onOperationChanged(int index){
    groupMatrixB->setVisible(index == 0 || index == 1);
    groupScalar->setVisible(index == 2);
    tableResult->clearContents();
    tableResult->setRowCount(0);
    tableResult->setColumnCount(0);
    textResult->clear();
    textResult->setVisible(false);
    tableResult->setVisible(true);
}

template <typename T>
T parseValue(const QString& str){
    if constexpr (std::is_same_v<T, Complex>){
        QStringList parts = str.split(",");
        double re = parts.value(0).toDouble();
        double im = parts.value(1).toDouble();
        return Complex(re, im);
    } else if constexpr (std::is_same_v<T, int>){
        return str.toInt();
    } else{
        return str.toDouble();
    }
}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T> parseMatrixFromTable(QTableWidget* table){
    size_t r = table->rowCount();
    size_t c = table->columnCount();
    Container<Matrix_elem<T>> parsedElems;

    for (int i = 0; i < r; ++i){
        for (int j = 0; j < c; ++j){
            QTableWidgetItem* item = table->item(i, j);
            if (item && !item->text().trimmed().isEmpty()){
                T val = parseValue<T>(item->text().trimmed());
                if (val != T()){
                    parsedElems.Append(Matrix_elem<T>(val, i, j));
                }
            }
        }
    }
    return SparseMatrix<Container, T>(parsedElems, r, c);
}

template <template <typename> class Container, typename T>
void displayResult(QTableWidget* tableResult, const SparseMatrix<Container, T>& result){
    tableResult->setRowCount(result.Get_rows());
    tableResult->setColumnCount(result.Get_cols());
    tableResult->clearContents();

    auto elems = result.Get_Elements();
    for (size_t idx = 0; idx < elems.GetLength(); ++idx){
        auto item = elems.Get(idx);
        QString valStr;
        if constexpr (std::is_same_v<T, Complex>){
            std::stringstream ss;
            ss << item.elem;
            valStr = QString::fromStdString(ss.str());
        } else if constexpr (std::is_same_v<T, int>){
            valStr = QString::number(item.elem);
        } else{
            valStr = QString::number(item.elem, 'f', 4);
        }
        QTableWidgetItem* cell = new QTableWidgetItem(valStr);
        tableResult->setItem(item.i, item.j, cell);
    }
}

template <typename T, template <typename> class Container>
void MainWindow::executeMatrixOperation(){
    try{
        SparseMatrix<Container, T> matrixA = parseMatrixFromTable<Container, T>(tableA);
        int op = comboOperation->currentIndex();

        if (op == 0){
            SparseMatrix<Container, T> matrixB = parseMatrixFromTable<Container, T>(tableB);
            auto result = matrixA + matrixB;
            textResult->setVisible(false);
            tableResult->setVisible(true);
            displayResult(tableResult, result);
        } 
        else if (op == 1){
            SparseMatrix<Container, T> matrixB = parseMatrixFromTable<Container, T>(tableB);
            auto result = matrixA * matrixB;
            textResult->setVisible(false);
            tableResult->setVisible(true);
            displayResult(tableResult, result);
        } 
        else if (op == 2){
            T scalar = parseValue<T>(lineScalar->text());
            auto result = matrixA * scalar;
            textResult->setVisible(false);
            tableResult->setVisible(true);
            displayResult(tableResult, result);
        } 
        else if (op == 3){
            T norm = matrixA.Get_Norm();
            std::stringstream ss;
            ss << norm;
            tableResult->setVisible(false);
            textResult->setVisible(true);
            textResult->setPlainText(QString::fromStdString(ss.str()));
        }
    } catch (...){
        QMessageBox::warning(this, ERR_TITLE, ERR_INVALID_INPUT);
    }
}

template <typename T>
void MainWindow::dispatchContainer(){
    if (comboContainer->currentIndex() == 0){
        executeMatrixOperation<T, ArraySequence>();
    } else{
        executeMatrixOperation<T, ListSequence>();
    }
}

void MainWindow::onRun(){
    int typeIdx = comboDataType->currentIndex();
    if (typeIdx == 0) dispatchContainer<int>();
    else if (typeIdx == 1) dispatchContainer<double>();
    else if (typeIdx == 2) dispatchContainer<Complex>();
}