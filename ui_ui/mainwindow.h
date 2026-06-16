#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QTableWidget>

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onOperationChanged(int index);
    void onRun();

private:
    void setupOperationsTab();
    void normalizeTable(QTableWidget* table);

    template <typename T, template <typename> class Container>
    void executeMatrixOperation();

    template <typename T>
    void dispatchContainer();

    QTabWidget *tabs;
    QWidget *tabOps;

    QGroupBox *groupSettings;
    QComboBox *comboDataType;
    QComboBox *comboContainer;
    QComboBox *comboOperation;

    QGroupBox *groupMatrixA;
    QSpinBox *spinRowsA;
    QSpinBox *spinColsA;
    QTableWidget *tableA;

    QGroupBox *groupMatrixB;
    QSpinBox *spinRowsB;
    QSpinBox *spinColsB;
    QTableWidget *tableB;

    QGroupBox *groupScalar;
    QLineEdit *lineScalar;

    QPushButton *btnRun;

    QGroupBox *groupResult;
    QLabel *labelResultTitle;
    QTextEdit *textResult;
    QTableWidget *tableResult;
};

#endif // MAINWINDOW_H