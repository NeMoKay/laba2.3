#pragma once
#include <QString>
#include <QLocale>


inline bool isRussianSystem() {
    static bool isRu = QLocale::system().name().startsWith("ru");
    return isRu;
}

#define APP_TITLE (isRussianSystem() ? "Разреженные матрицы" : "Sparse Matrices")
#define TAB_OPERATIONS (isRussianSystem() ? "Операции" : "Operations")

#define TEXT_SETTINGS (isRussianSystem() ? "Настройки операции" : "Operation Settings")
#define TEXT_DATA_TYPE (isRussianSystem() ? "Тип данных:" : "Data Type:")
#define TEXT_CONTAINER (isRussianSystem() ? "Контейнер:" : "Container:")
#define TEXT_OPERATION (isRussianSystem() ? "Операция:" : "Operation:")

const QString TYPE_INT = "int";
const QString TYPE_DOUBLE = "double";
const QString TYPE_COMPLEX = "Complex";

const QString CONT_ARRAY = "ArraySequence";
const QString CONT_LIST = "ListSequence";

#define OP_ADD (isRussianSystem() ? "Сложение (A + B)" : "Addition (A + B)")
#define OP_MULT (isRussianSystem() ? "Умножение (A * B)" : "Multiplication (A * B)")
#define OP_SCALAR (isRussianSystem() ? "Умножение на скаляр (A * scalar)" : "Scalar Multiplication (A * scalar)")
#define OP_NORM (isRussianSystem() ? "Норма матрицы (Norm(A))" : "Matrix Norm (Norm(A))")

#define TEXT_MATRIX_A (isRussianSystem() ? "Матрица A" : "Matrix A")
#define TEXT_MATRIX_B (isRussianSystem() ? "Матрица B" : "Matrix B")
#define TEXT_SCALAR (isRussianSystem() ? "Скаляр" : "Scalar")
#define TEXT_ROWS (isRussianSystem() ? "Строки:" : "Rows:")
#define TEXT_COLS (isRussianSystem() ? "Столбцы:" : "Cols:")

#define TEXT_RUN (isRussianSystem() ? "Вычислить" : "Calculate")
#define TEXT_GROUP_RESULT (isRussianSystem() ? "Результат" : "Result")
#define TEXT_RESULT_LBL (isRussianSystem() ? "Итог:" : "Total:")

#define ERR_TITLE (isRussianSystem() ? "Ошибка" : "Error")
#define ERR_INVALID_INPUT (isRussianSystem() ? "Некорректный ввод данных." : "Invalid input data.")
#define ERR_DIMS_MISMATCH (isRussianSystem() ? "Размеры матриц не совпадают или недопустимы для операции." : "Matrix dimensions mismatch or invalid for operation.")

const QString STYLE_LABEL_RESULT =
    "color: #a0c4ff; font-size: 13px; padding: 2px;";

const QString STYLE_MAIN = R"(
    QTabWidget::pane {
        border: 1px solid rgba(255,255,255,60);
        background: transparent;
    }
    QTabBar::tab {
        background: rgba(0,0,0,120);
        color: white;
        padding: 8px 20px;
        border: 1px solid rgba(255,255,255,60);
        border-radius: 4px 4px 0 0;
    }
    QTabBar::tab:selected {
        background: rgba(83,52,131,180);
    }
    QGroupBox {
        color: white;
        font-weight: bold;
        border: 1px solid rgba(255,255,255,70);
        border-radius: 8px;
        margin-top: 10px;
        padding-top: 10px;
        background: rgba(0,0,0,80);
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 12px;
        color: white;
    }
    QLabel { color: white; }
    QComboBox {
        background: rgba(0,0,0,200);
        color: white;
        border: 1px solid rgba(255,255,255,80);
        border-radius: 5px;
        padding: 5px;
    }
    QComboBox QAbstractItemView {
        background: rgba(20,20,20,240);
        color: white;
        selection-background-color: rgba(83,52,131,200);
    }
    QLineEdit, QSpinBox {
        background: rgba(0,0,0,140);
        color: white;
        border: 1px solid rgba(255,255,255,80);
        border-radius: 5px;
        padding: 5px;
    }
    QTextEdit {
        background: rgba(0,0,0,120);
        color: #a0c4ff;
        border: 1px solid rgba(255,255,255,60);
        border-radius: 5px;
    }
    QPushButton {
        background: rgba(60,60,60,180);
        color: white;
        border: 1px solid rgba(255,255,255,100);
        border-radius: 6px;
        padding: 8px 16px;
        font-size: 13px;
    }
    QPushButton:hover { background: rgba(100,100,100,200); }
    QPushButton:pressed { background: rgba(40,40,40,200); }
    QPushButton:disabled {
        background: rgba(40,40,40,120);
        color: rgba(255,255,255,60);
    }
    QTableWidget {
        background: rgba(0,0,0,120);
        color: white;
        gridline-color: rgba(255,255,255,40);
        border: 1px solid rgba(255,255,255,60);
        border-radius: 5px;
    }
    QHeaderView::section {
        background-color: rgba(0,0,0,180);
        color: white;
        border: 1px solid rgba(255,255,255,40);
        padding: 4px;
    }
    QTableWidget::item:selected {
        background-color: rgba(83,52,131,200);
    }
)";