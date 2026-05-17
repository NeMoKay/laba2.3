#include "httplib.h"
#include <nlohmann/json.hpp>
#include "ArraySequence.hpp"
#include "SparseMatrix.hpp"
#include "Complex.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <algorithm>

using json = nlohmann::json;


template <typename T>
struct AppState{
    std::unique_ptr<SparseMatrix<ArraySequence, T>> A = std::make_unique<SparseMatrix<ArraySequence, T>>(3, 3);
    std::unique_ptr<SparseMatrix<ArraySequence, T>> B = std::make_unique<SparseMatrix<ArraySequence, T>>(3, 3);
    std::unique_ptr<SparseMatrix<ArraySequence, T>> Res = nullptr;
};

AppState<int> state_int;
AppState<double> state_double;
AppState<Complex> state_complex;

std::string format_double(double d) {
    std::string s = std::to_string(d);
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    if (s.back() == '.') s.pop_back();
    if (s == "-0") return "0";
    return s;
}

template <typename T>
std::string format_val(const T& val) {
    if constexpr (std::is_same_v<T, Complex>) {
        if (val == Complex()) return "0";
        std::string re_str = format_double(val.re);
        std::string im_str = format_double(val.im);
        
        if (val.re == 0) return im_str + "i";
        if (val.im == 0) return re_str;
        return re_str + (val.im > 0 ? "+" : "") + im_str + "i";
    } else if constexpr (std::is_same_v<T, double>) {
        if (val == 0.0) return "0";
        return format_double(val);
    } else {
        if (val == 0) return "0";
        return std::to_string(val);
    }
}

template <typename T>
json matrixToJson(const SparseMatrix<ArraySequence, T>* mat) {
    json j;
    if (!mat) {
        j["rows"] = 0; j["cols"] = 0; j["grid"] = json::array();
        return j;
    }
    j["rows"] = mat->Get_rows();
    j["cols"] = mat->Get_cols();
    json grid = json::array();
    
    for (size_t i = 0; i < mat->Get_rows(); ++i) {
        json row = json::array();
        for (size_t k = 0; k < mat->Get_cols(); ++k) {
            row.push_back(format_val(mat->Get_elem(i, k)));
        }
        grid.push_back(row);
    }
    j["grid"] = grid;
    return j;
}

template <typename T>
json serialize_state(const AppState<T>& state) {
    json j;
    j["A"] = matrixToJson(state.A.get());
    j["B"] = matrixToJson(state.B.get());
    j["Res"] = matrixToJson(state.Res.get());
    return j;
}

std::string build_response(const std::string& dataType, const std::string& log) {
    json response;
    if (dataType == "int") response["state"] = serialize_state(state_int);
    else if (dataType == "double") response["state"] = serialize_state(state_double);
    else if (dataType == "complex") response["state"] = serialize_state(state_complex);
    response["log"] = log;
    return response.dump();
}

int main() {
    httplib::Server svr;
    svr.set_mount_point("/", "./interface_html");

    svr.Get("/api/get_state", [](const httplib::Request& req, httplib::Response& res){
        std::string dt = req.get_param_value("dataType");
        res.set_content(build_response(dt.empty() ? "int" : dt, "System Ready"), "application/json");
    });

    svr.Get("/api/clear", [](const httplib::Request& req, httplib::Response& res){
        std::string dt = req.get_param_value("dataType");
        state_int = AppState<int>();
        state_double = AppState<double>();
        state_complex = AppState<Complex>();
        res.set_content(build_response(dt, "All Matrices Cleared"), "application/json");
    });

    svr.Post("/api/create", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "Success", dt = "int";
        try {
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");
            size_t r = body.value("rows", 3);
            size_t c = body.value("cols", 3);
            
            auto create_mat = [&](auto& state_obj, auto dummy_type) {
                using T = decltype(dummy_type);
                if (target == "A") state_obj.A = std::make_unique<SparseMatrix<ArraySequence, T>>(r, c);
                else state_obj.B = std::make_unique<SparseMatrix<ArraySequence, T>>(r, c);
                state_obj.Res = nullptr; // Прячем результат при создании новой матрицы
            };

            if (dt == "int") create_mat(state_int, int{});
            else if (dt == "double") create_mat(state_double, double{});
            else if (dt == "complex") create_mat(state_complex, Complex{});
            
            log = "Created Matrix " + target;
        } catch(const std::exception& e) { log = "Error: " + std::string(e.what()); }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/set", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "Success", dt = "int";
        try {
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");
            size_t tr = body.value("r", 0);
            size_t tc = body.value("c", 0);

            auto update_cell = [&](auto& state_obj, auto dummy_type) {
                using T = decltype(dummy_type);
                auto& mat_ptr = (target == "A") ? state_obj.A : state_obj.B;
                
                T new_val;
                if constexpr (std::is_same_v<T, Complex>) new_val = Complex(body.value("val_re", 0.0), body.value("val_im", 0.0));
                else new_val = static_cast<T>(body.value("val_re", 0.0));

                size_t R = mat_ptr->Get_rows();
                size_t C = mat_ptr->Get_cols();
                ArraySequence<Matrix_elem<T>> new_seq;
                
                for (size_t r = 0; r < R; ++r) {
                    for (size_t c = 0; c < C; ++c) {
                        if (r == tr && c == tc) {
                            if (new_val != T()) new_seq.Append(Matrix_elem<T>(new_val, r, c));
                        } 
                        else {
                            T ext = mat_ptr->Get_elem(r, c);
                            if (ext != T()) new_seq.Append(Matrix_elem<T>(ext, r, c));
                        }
                    }
                }
                mat_ptr = std::make_unique<SparseMatrix<ArraySequence, T>>(new_seq, R, C);
                state_obj.Res = nullptr;
            };

            if (dt == "int") update_cell(state_int, int{});
            else if (dt == "double") update_cell(state_double, double{});
            else if (dt == "complex") update_cell(state_complex, Complex{});
            log = "Updated cell in Matrix " + target;
        } catch(const std::exception& e) { log = "Error: " + std::string(e.what()); }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/add", [](const httplib::Request& req, httplib::Response& res){
        std::string log, dt = "int";
        try {
            dt = json::parse(req.body).value("dataType", "int");
            auto add_op = [&](auto& st, auto dummy_type) {
                using T = decltype(dummy_type);
                st.Res = std::make_unique<SparseMatrix<ArraySequence, T>>(*(st.A) + *(st.B));
            };
            if (dt == "int") add_op(state_int, int{});
            else if (dt == "double") add_op(state_double, double{});
            else if (dt == "complex") add_op(state_complex, Complex{});
            log = "Addition Successful";
        } 
        catch (const Exception& e) { log = "Math Error: " + std::string(e.what()); }
        catch (const std::exception& e) { log = "System Error: " + std::string(e.what()); }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/mult", [](const httplib::Request& req, httplib::Response& res){
        std::string log, dt = "int";
        try {
            dt = json::parse(req.body).value("dataType", "int");
            auto mult_op = [&](auto& st, auto dummy_type) {
                using T = decltype(dummy_type);
                st.Res = std::make_unique<SparseMatrix<ArraySequence, T>>(*(st.A) * *(st.B));
            };
            if (dt == "int") mult_op(state_int, int{});
            else if (dt == "double") mult_op(state_double, double{});
            else if (dt == "complex") mult_op(state_complex, Complex{});
            log = "Multiplication Successful";
        } 
        catch (const Exception& e) { log = "Math Error: " + std::string(e.what()); }
        catch (const std::exception& e) { log = "System Error: " + std::string(e.what()); }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/norm", [](const httplib::Request& req, httplib::Response& res){
        std::string log, dt = "int";
        try {
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");

            auto calc_norm = [&](auto& st, auto dummy_type) {
                using T = decltype(dummy_type);
                auto& mat = (target == "A") ? st.A : st.B;
                T n = mat->Get_Norm();
                log = "Norm ||" + target + "|| = " + format_val(n);
            };
            if (dt == "int") calc_norm(state_int, int{});
            else if (dt == "double") calc_norm(state_double, double{});
            else if (dt == "complex") calc_norm(state_complex, Complex{});
        } catch(const std::exception& e) { log = "Error: " + std::string(e.what()); }
        res.set_content(build_response(dt, log), "application/json");
    });

    std::cout << "\n[Sparse Matrix GUI] -> http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}