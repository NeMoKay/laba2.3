#pragma once
#include <QString>
#include <QLocale>
#include "LocalizationEn.h"
#include "LocalizationRu.h"
#include "Style.h"

struct AppStrings{
    QString appTitle;
    QString tabOperations;
    QString textSettings;
    QString textDataType;
    QString textContainer;
    QString textOperation;
    QString opAdd;
    QString opMult;
    QString opScalar;
    QString opNorm;
    QString textMatrixA;
    QString textMatrixB;
    QString textScalar;
    QString textRows;
    QString textCols;
    QString textRun;
    QString textGroupResult;
    QString textResultLbl;
    QString errTitle;
    QString errInvalidInput;
    QString errDimsMismatch;
};

namespace Resources{
    const QString TYPE_INT = "int";
    const QString TYPE_DOUBLE = "double";
    const QString TYPE_COMPLEX = "Complex";

    const QString CONT_ARRAY = "ArraySequence";
    const QString CONT_LIST = "ListSequence";

    inline AppStrings get_names(){
        AppStrings set_string;
        QString Language = QLocale::system().name();

        if (Language.startsWith("ru")){
            set_string.appTitle = LocalizationRu::APP_TITLE;
            set_string.tabOperations = LocalizationRu::TAB_OPERATIONS;
            set_string.textSettings = LocalizationRu::TEXT_SETTINGS;
            set_string.textDataType = LocalizationRu::TEXT_DATA_TYPE;
            set_string.textContainer = LocalizationRu::TEXT_CONTAINER;
            set_string.textOperation = LocalizationRu::TEXT_OPERATION;
            set_string.opAdd = LocalizationRu::OP_ADD;
            set_string.opMult = LocalizationRu::OP_MULT;
            set_string.opScalar = LocalizationRu::OP_SCALAR;
            set_string.opNorm = LocalizationRu::OP_NORM;
            set_string.textMatrixA = LocalizationRu::TEXT_MATRIX_A;
            set_string.textMatrixB = LocalizationRu::TEXT_MATRIX_B;
            set_string.textScalar = LocalizationRu::TEXT_SCALAR;
            set_string.textRows = LocalizationRu::TEXT_ROWS;
            set_string.textCols = LocalizationRu::TEXT_COLS;
            set_string.textRun = LocalizationRu::TEXT_RUN;
            set_string.textGroupResult = LocalizationRu::TEXT_GROUP_RESULT;
            set_string.textResultLbl = LocalizationRu::TEXT_RESULT_LBL;
            set_string.errTitle = LocalizationRu::ERR_TITLE;
            set_string.errInvalidInput = LocalizationRu::ERR_INVALID_INPUT;
            set_string.errDimsMismatch = LocalizationRu::ERR_DIMS_MISMATCH;
        } 
        else{
            set_string.appTitle = LocalizationEn::APP_TITLE;
            set_string.tabOperations = LocalizationEn::TAB_OPERATIONS;
            set_string.textSettings = LocalizationEn::TEXT_SETTINGS;
            set_string.textDataType = LocalizationEn::TEXT_DATA_TYPE;
            set_string.textContainer = LocalizationEn::TEXT_CONTAINER;
            set_string.textOperation = LocalizationEn::TEXT_OPERATION;
            set_string.opAdd = LocalizationEn::OP_ADD;
            set_string.opMult = LocalizationEn::OP_MULT;
            set_string.opScalar = LocalizationEn::OP_SCALAR;
            set_string.opNorm = LocalizationEn::OP_NORM;
            set_string.textMatrixA = LocalizationEn::TEXT_MATRIX_A;
            set_string.textMatrixB = LocalizationEn::TEXT_MATRIX_B;
            set_string.textScalar = LocalizationEn::TEXT_SCALAR;
            set_string.textRows = LocalizationEn::TEXT_ROWS;
            set_string.textCols = LocalizationEn::TEXT_COLS;
            set_string.textRun = LocalizationEn::TEXT_RUN;
            set_string.textGroupResult = LocalizationEn::TEXT_GROUP_RESULT;
            set_string.textResultLbl = LocalizationEn::TEXT_RESULT_LBL;
            set_string.errTitle = LocalizationEn::ERR_TITLE;
            set_string.errInvalidInput = LocalizationEn::ERR_INVALID_INPUT;
            set_string.errDimsMismatch = LocalizationEn::ERR_DIMS_MISMATCH;
        }

        return set_string;
    }
}