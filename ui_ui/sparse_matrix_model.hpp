#pragma once
#include <cstddef>
#include <QString>
#include <QTableWidget>
#include <type_traits>
#include <map>
#include <functional>
#include <stdexcept>
#include "SparseMatrix.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Complex.hpp"

inline Complex parseComplexStr(QString s) {
    s.replace(" ", "");
    s.replace(",", ".");
    if (s.isEmpty() || s == "0") return Complex(0, 0);
    bool ok;
    double val = s.toDouble(&ok);
    if (ok) return Complex(val, 0);

    bool hasI = s.endsWith("i") || s.endsWith("j");
    if (hasI) s.chop(1);

    int splitIdx = -1;
    for (int i = s.length() - 1; i > 0; --i) {
        if (s[i] == '+' || s[i] == '-') {
            if (i > 0 && (s[i-1] == 'e' || s[i-1] == 'E')) continue;
            splitIdx = i;
            break;
        }
    }

    if (splitIdx != -1) { 
        double re = s.left(splitIdx).toDouble();
        QString imStr = s.mid(splitIdx);
        if (imStr == "+" || imStr == "-") imStr += "1";
        double im = imStr.toDouble();
        return Complex(re, im);
    } else {
        if (s == "+" || s == "-") s += "1";
        else if (s.isEmpty()) s = "1";
        double val = s.toDouble();
        if (hasI) return Complex(0, val);
        return Complex(val, 0);
    }
}

inline QString formatValue(double v) { return QString::number(v); }
inline QString formatValue(int v) { return QString::number(v); }
inline QString formatValue(const Complex& c) {
    if (c.re == 0 && c.im == 0) return "0";
    if (c.re == 0) return QString::number(c.im) + "i";
    if (c.im == 0) return QString::number(c.re);
    QString res = QString::number(c.re);
    if (c.im > 0) res += "+";
    res += QString::number(c.im) + "i";
    return res;
}

class SparseMatrixModel {
public:
    virtual ~SparseMatrixModel() = default;
    virtual size_t GetRows() const = 0;
    virtual size_t GetCols() const = 0;
    virtual QString GetElementStr(size_t i, size_t j) const = 0;
    virtual QString NormStr() const = 0;

    virtual SparseMatrixModel* Add(const SparseMatrixModel& other) const = 0;
    virtual SparseMatrixModel* Multiply(const SparseMatrixModel& other) const = 0;
    virtual SparseMatrixModel* ScalarMultiplyStr(const QString& scalarStr) const = 0;
};

template <typename T, template <typename> class Container>
class SparseMatrixAdapter : public SparseMatrixModel {
    SparseMatrix<Container, T> matrix;
public:
    SparseMatrixAdapter(const SparseMatrix<Container, T>& m) : matrix(m) {}

    size_t GetRows() const override { return matrix.Get_rows(); }
    size_t GetCols() const override { return matrix.Get_cols(); }
    
    QString GetElementStr(size_t i, size_t j) const override { 
        return formatValue(matrix(i, j)); 
    }
    
    QString NormStr() const override { 
        return formatValue(matrix.Get_Norm()); 
    }

    SparseMatrixModel* Add(const SparseMatrixModel& other) const override {
        const auto* o = dynamic_cast<const SparseMatrixAdapter<T, Container>*>(&other);
        if (!o) throw std::invalid_argument("Type mismatch");
        return new SparseMatrixAdapter<T, Container>(matrix + o->matrix);
    }

    SparseMatrixModel* Multiply(const SparseMatrixModel& other) const override {
        const auto* o = dynamic_cast<const SparseMatrixAdapter<T, Container>*>(&other);
        if (!o) throw std::invalid_argument("Type mismatch");
        return new SparseMatrixAdapter<T, Container>(matrix * o->matrix);
    }

    SparseMatrixModel* ScalarMultiplyStr(const QString& scalarStr) const override {
        T scalar;
        if constexpr (std::is_same_v<T, Complex>) {
            scalar = parseComplexStr(scalarStr);
        } else {
            scalar = (T)scalarStr.toDouble();
        }
        return new SparseMatrixAdapter<T, Container>(matrix * scalar);
    }
};

template <typename T, template <typename> class Container>
SparseMatrixModel* buildSparseModel(int rows, int cols, QTableWidget* tbl) {
    Container<Matrix_elem<T>> data;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QTableWidgetItem *item = tbl->item(i, j);
            if (item && !item->text().trimmed().isEmpty()) {
                QString txt = item->text().trimmed();
                if (txt == "0") continue;

                T val;
                if constexpr (std::is_same_v<T, Complex>) {
                    val = parseComplexStr(txt);
                    if (val.re == 0 && val.im == 0) continue;
                } else {
                    val = (T)txt.toDouble();
                    if (val == 0) continue;
                }
                data.Append(Matrix_elem<T>(val, i, j));
            }
        }
    }
    SparseMatrix<Container, T> sm(data, rows, cols);
    return new SparseMatrixAdapter<T, Container>(sm);
}

class ModelFactory {
    using Creator = std::function<SparseMatrixModel*(int, int, QTableWidget*)>;
    std::map<QString, Creator> registry;

    ModelFactory() {
        registry["int_ArraySequence"] = buildSparseModel<int, ArraySequence>;
        registry["int_ListSequence"] = buildSparseModel<int, ListSequence>;
        registry["double_ArraySequence"] = buildSparseModel<double, ArraySequence>;
        registry["double_ListSequence"] = buildSparseModel<double, ListSequence>;
        registry["Complex_ArraySequence"] = buildSparseModel<Complex, ArraySequence>;
        registry["Complex_ListSequence"] = buildSparseModel<Complex, ListSequence>;
    }

public:
    static ModelFactory& instance() {
        static ModelFactory factory;
        return factory;
    }

    SparseMatrixModel* create(const QString& type, const QString& container, int rows, int cols, QTableWidget* tbl) {
        QString key = type + "_" + container;
        if (registry.count(key)) {
            return registry[key](rows, cols, tbl);
        }
        throw std::invalid_argument("Unknown configuration");
    }
};