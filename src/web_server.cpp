#include "httplib.h"
#include <nlohmann/json.hpp>
#include "SparseMatrix.hpp"
#include "ArraySequence.hpp"
#include "Complex.hpp"
#include <iostream>
#include <string>
#include <sstream>

using json = nlohmann::json;

class IMatrixHandler {
public:
    virtual ~IMatrixHandler() = default;
    virtual json GetState() = 0;
    virtual void Clear() = 0;
    virtual void Create(const std::string& target, size_t rows, size_t cols) = 0;
    virtual void Set(const std::string& target, size_t r, size_t c, double re, double im) = 0;
    virtual void Add() = 0;
    virtual void Mult() = 0;
    virtual std::string Norm(const std::string& target) = 0;
    virtual void MultScalar(const std::string& target, double re, double im) = 0;
};

template <typename T>
class MatrixHandler : public IMatrixHandler {
private:
    SparseMatrix<ArraySequence, T>* A = nullptr;
    SparseMatrix<ArraySequence, T>* B = nullptr;
    SparseMatrix<ArraySequence, T>* Res = nullptr;

    T MakeValue(double re, double im);
    std::string ToString(const T& val);

    json MatrixToJson(SparseMatrix<ArraySequence, T>* mat) {
        if (!mat) {
            return {{"rows", 0}, {"cols", 0}, {"grid", json::array()}};
        }
        size_t r = mat->Get_rows();
        size_t c = mat->Get_cols();
        json grid = json::array();
        for (size_t i = 0; i < r; ++i) {
            json row = json::array();
            for (size_t j = 0; j < c; ++j) {
                row.push_back(ToString(mat->Get_elem(i, j)));
            }
            grid.push_back(row);
        }
        return {{"rows", r}, {"cols", c}, {"grid", grid}};
    }

    SparseMatrix<ArraySequence, T>** GetTarget(const std::string& target) {
        if (target == "A") return &A;
        if (target == "B") return &B;
        if (target == "Res") return &Res;
        throw std::invalid_argument("Target not found");
    }

public:
    ~MatrixHandler() override {
        delete A;
        delete B;
        delete Res;
    }

    json GetState() override {
        return {
            {"A", MatrixToJson(A)},
            {"B", MatrixToJson(B)},
            {"Res", MatrixToJson(Res)}
        };
    }

    void Clear() override {
        delete A; A = nullptr;
        delete B; B = nullptr;
        delete Res; Res = nullptr;
    }

    void Create(const std::string& target, size_t rows, size_t cols) override {
        SparseMatrix<ArraySequence, T>** mat = GetTarget(target);
        delete *mat;
        *mat = new SparseMatrix<ArraySequence, T>(rows, cols);
    }

    void Set(const std::string& target, size_t r, size_t c, double re, double im) override {
        SparseMatrix<ArraySequence, T>** mat = GetTarget(target);
        if (!*mat) {
            throw std::runtime_error("Empty target");
        }

        auto elements = (*mat)->Get_Elements();
        T val = MakeValue(re, im);

        bool found = false;
        for (size_t i = 0; i < elements.GetLength(); ++i) {
            if (elements.Get(i).i == r && elements.Get(i).j == c) {
                auto el = elements.Get(i);
                el.elem = val;
                elements.Set(i, el);
                found = true;
                break;
            }
        }
        if (!found) {
            elements.Append(Matrix_elem<T>(val, r, c));
        }

        SparseMatrix<ArraySequence, T>* new_mat = new SparseMatrix<ArraySequence, T>(elements, (*mat)->Get_rows(), (*mat)->Get_cols());
        delete *mat;
        *mat = new_mat;
    }

    void Add() override {
        if (!A || !B) {
            throw std::runtime_error("A and B must exist");
        }
        delete Res;
        Res = new SparseMatrix<ArraySequence, T>((*A) + (*B));
    }

    void Mult() override {
        if (!A || !B) {
            throw std::runtime_error("A and B must exist");
        }
        delete Res;
        Res = new SparseMatrix<ArraySequence, T>((*A) * (*B));
    }

    std::string Norm(const std::string& target) override {
        SparseMatrix<ArraySequence, T>** mat = GetTarget(target);
        if (!*mat) {
            throw std::runtime_error("Empty target");
        }
        return ToString((*mat)->Get_Norm());
    }

    void MultScalar(const std::string& target, double re, double im) override {
        SparseMatrix<ArraySequence, T>** mat = GetTarget(target);
        if (!*mat) {
            throw std::runtime_error("Empty target");
        }
        T scalar = MakeValue(re, im);
        SparseMatrix<ArraySequence, T>* new_mat = new SparseMatrix<ArraySequence, T>((*(*mat)) * scalar);
        delete Res;
        Res = new_mat;
    }
};

template <> int MatrixHandler<int>::MakeValue(double re, double im) { return static_cast<int>(re); }
template <> double MatrixHandler<double>::MakeValue(double re, double im) { return re; }
template <> Complex MatrixHandler<Complex>::MakeValue(double re, double im) { return Complex(re, im); }

template <> std::string MatrixHandler<int>::ToString(const int& val) { return std::to_string(val); }
template <> std::string MatrixHandler<double>::ToString(const double& val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}
template <> std::string MatrixHandler<Complex>::ToString(const Complex& val) {
    if (val.re == 0 && val.im == 0) return "0";
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

class AppController {
private:
    IMatrixHandler* h_int;
    IMatrixHandler* h_double;
    IMatrixHandler* h_complex;

public:
    AppController() {
        h_int = new MatrixHandler<int>();
        h_double = new MatrixHandler<double>();
        h_complex = new MatrixHandler<Complex>();
    }

    ~AppController() {
        delete h_int;
        delete h_double;
        delete h_complex;
    }

    IMatrixHandler* Get(const std::string& type) {
        if (type == "int") return h_int;
        if (type == "double") return h_double;
        if (type == "complex") return h_complex;
        throw std::invalid_argument("Type not mapped");
    }
};

int main() {
    AppController ctrl;
    httplib::Server svr;
    
    svr.set_mount_point("/", "./interface_html");

    auto build_res = [](IMatrixHandler* h, const std::string& log) {
        json j;
        try {
            j["state"] = h->GetState();
        } 
        catch(...) {
            j["state"] = json::object();
        }
        j["log"] = log;
        return j.dump();
    };

    auto build_err = [](const std::string& log) {
        json j;
        j["state"] = json::object();
        j["log"] = log;
        return j.dump();
    };

    svr.Get("/api/get_state", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string type = req.get_param_value("dataType");
            res.set_content(build_res(ctrl.Get(type), "Synced"), "application/json");
        } 
        catch(const std::exception& e) {
            res.set_content(build_err(e.what()), "application/json");
        }
    });

    svr.Get("/api/clear", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string type = req.get_param_value("dataType");
            ctrl.Get(type)->Clear();
            res.set_content(build_res(ctrl.Get(type), "Wiped data"), "application/json");
        } 
        catch(const std::exception& e) {
            res.set_content(build_err(e.what()), "application/json");
        }
    });

    auto handle_post = [&](const httplib::Request& req, httplib::Response& res, auto logic) {
        try {
            json j = json::parse(req.body);
            auto h = ctrl.Get(j.value("dataType", "int"));
            logic(h, j);
        } 
        catch(const std::exception& e) {
            res.set_content(build_err(e.what()), "application/json");
        }
    };

    svr.Post("/api/create", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            h->Create(j["target"], j["rows"], j["cols"]);
            res.set_content(build_res(h, "Matrix built"), "application/json");
        });
    });

    svr.Post("/api/set", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            h->Set(j["target"], j["r"], j["c"], j.value("val_re", 0.0), j.value("val_im", 0.0));
            res.set_content(build_res(h, "Element updated"), "application/json");
        });
    });

    svr.Post("/api/add", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            h->Add();
            res.set_content(build_res(h, "Sum calculated"), "application/json");
        });
    });

    svr.Post("/api/mult", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            h->Mult();
            res.set_content(build_res(h, "Product calculated"), "application/json");
        });
    });

    svr.Post("/api/norm", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            std::string val = h->Norm(j["target"]);
            res.set_content(build_res(h, "Norma ||" + j["target"].get<std::string>() + "|| = " + val), "application/json");
        });
    });

    svr.Post("/api/mult_scalar", [&](const httplib::Request& req, httplib::Response& res) {
        handle_post(req, res, [&](IMatrixHandler* h, const json& j) {
            h->MultScalar(j["target"], j.value("scalar_re", 0.0), j.value("scalar_im", 0.0));
            res.set_content(build_res(h, "Scaled successfully"), "application/json");
        });
    });

    std::cout << "Starting server at http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}