#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QTableWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <map>
#include <functional>

class SparseMatrixModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void updateGrids();
    void onCompute();

private:
    QLabel *labelRowsA, *labelColsA;      
    QSpinBox *spinRowsA, *spinColsA;
    QLabel *labelType, *labelContainer, *labelOp;
    QComboBox *comboType, *comboContainer, *comboOp;
    QPushButton *btnCompute;
    QLabel *labelA, *labelB, *labelResultMatrix, *labelResult;        
    QTableWidget *tableA, *tableB, *tableResult;
    QWidget *widgetB;
    QWidget *widgetResult;

    using Endpoint = std::function<void(SparseMatrixModel*)>;
    std::map<QString, Endpoint> endpoints;

    void setupUI();
    void registerEndpoints();
    void syncTableSize(QTableWidget *tbl, int rows, int cols);
    bool validateTable(QTableWidget *tbl);
    void showModel(QTableWidget *tbl, SparseMatrixModel *model);

    void execNorm(SparseMatrixModel* modelA);
    void execScalar(SparseMatrixModel* modelA);
    void execAdd(SparseMatrixModel* modelA);
    void execMult(SparseMatrixModel* modelA);
};