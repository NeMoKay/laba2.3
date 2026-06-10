#pragma once
#include <QString>

//Ресурс с файлом перевода. папки под каждый язык, и опредляя систему.  

const QString APP_TITLE = "Разреженные матрицы";
const QString TAB_OPERATIONS = "Операции";

const QString TEXT_SETTINGS = "Настройки операции";
const QString TEXT_DATA_TYPE = "Тип данных:";
const QString TEXT_CONTAINER = "Контейнер:";
const QString TEXT_OPERATION = "Операция:";

const QString TYPE_INT = "int";
const QString TYPE_DOUBLE = "double";
const QString TYPE_COMPLEX = "Complex";

const QString CONT_ARRAY = "ArraySequence";
const QString CONT_LIST = "ListSequence";

const QString OP_ADD = "Сложение (A + B)";
const QString OP_MULT = "Умножение (A * B)";
const QString OP_SCALAR = "Умножение на скаляр (A * scalar)";
const QString OP_NORM = "Норма матрицы (Norm(A))";

const QString TEXT_MATRIX_A = "Матрица A";
const QString TEXT_MATRIX_B = "Матрица B";
const QString TEXT_SCALAR = "Скаляр";
const QString TEXT_ROWS = "Строки:";
const QString TEXT_COLS = "Столбцы:";

const QString TEXT_RUN = "Вычислить";
const QString TEXT_GROUP_RESULT = "Результат";
const QString TEXT_RESULT_LBL = "Итог:";

const QString ERR_TITLE = "Ошибка";
const QString ERR_INVALID_INPUT = "Некорректный ввод данных.";
const QString ERR_DIMS_MISMATCH = "Размеры матриц не совпадают или недопустимы для операции.";

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