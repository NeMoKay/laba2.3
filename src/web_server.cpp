#include "httplib.h"
#include <nlohmann/json.hpp>
#include "ArraySequence.hpp"
#include "SparseMatrix.hpp"
#include "Complex.hpp"

#include <iostream>
#include <string>
#include <memory>

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

std::string format_double(double d){
    std::string s = std::to_string(d);
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    if (s.back() == '.'){
        s.pop_back();
    }
    if (s == "-0"){
        return "0";
    }
    return s;
}

std::string format_val(int val){
    if (val == 0){
        return "0";
    }
    return std::to_string(val);
}

std::string format_val(double val){
    if (val == 0.0){
        return "0";
    }
    return format_double(val);
}

std::string format_val(Complex val){
    if (val == Complex()){
        return "0";
    }
    std::string re_str = format_double(val.re);
    std::string im_str = format_double(val.im);
    
    if (val.re == 0){
        return im_str + "i";
    }
    if (val.im == 0){
        return re_str;
    }
    
    std::string sign = "";
    if (val.im > 0){
        sign = "+";
    }
    
    return re_str + sign + im_str + "i";
}

template <typename T>
json matrixToJson(const SparseMatrix<ArraySequence, T>* mat){
    json j;
    if (mat == nullptr){
        j["rows"] = 0;
        j["cols"] = 0;
        j["grid"] = json::array();
        return j;
    }
    j["rows"] = mat->Get_rows();
    j["cols"] = mat->Get_cols();
    json grid = json::array();
    
    for (size_t i = 0; i < mat->Get_rows(); ++i){
        json row = json::array();
        for (size_t k = 0; k < mat->Get_cols(); ++k){
            row.push_back(format_val(mat->Get_elem(i, k)));
        }
        grid.push_back(row);
    }
    j["grid"] = grid;
    return j;
}

template <typename T>
json serialize_state(const AppState<T>& state){
    json j;
    j["A"] = matrixToJson(state.A.get());
    j["B"] = matrixToJson(state.B.get());
    j["Res"] = matrixToJson(state.Res.get());
    return j;
}

std::string build_response(const std::string& dataType, const std::string& log){
    json response;
    if (dataType == "int"){
        response["state"] = serialize_state(state_int);
    } 
    else if (dataType == "double"){
        response["state"] = serialize_state(state_double);
    } 
    else if (dataType == "complex"){
        response["state"] = serialize_state(state_complex);
    }
    response["log"] = log;
    return response.dump();
}

int main(){
    httplib::Server svr;
    svr.set_mount_point("/", "./interface_html");

    svr.Get("/api/get_state", [](const httplib::Request& req, httplib::Response& res){
        std::string dt = req.get_param_value("dataType");
        if (dt.empty()){
            dt = "int";
        }
        res.set_content(build_response(dt, "System Ready"), "application/json");
    });

    svr.Get("/api/clear", [](const httplib::Request& req, httplib::Response& res){
        std::string dt = req.get_param_value("dataType");
        state_int = AppState<int>();
        state_double = AppState<double>();
        state_complex = AppState<Complex>();
        res.set_content(build_response(dt, "All Matrices Cleared"), "application/json");
    });

    svr.Post("/api/create", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "Success";
        std::string dt = "int";
        try{
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");
            size_t r = body.value("rows", 3);
            size_t c = body.value("cols", 3);
            
            if (dt == "int"){
                if (target == "A"){
                    state_int.A = std::make_unique<SparseMatrix<ArraySequence, int>>(r, c);
                } 
                else{
                    state_int.B = std::make_unique<SparseMatrix<ArraySequence, int>>(r, c);
                }
                state_int.Res = nullptr;
            } 
            else if (dt == "double"){
                if (target == "A"){
                    state_double.A = std::make_unique<SparseMatrix<ArraySequence, double>>(r, c);
                } 
                else{
                    state_double.B = std::make_unique<SparseMatrix<ArraySequence, double>>(r, c);
                }
                state_double.Res = nullptr;
            } 
            else if (dt == "complex"){
                if (target == "A"){
                    state_complex.A = std::make_unique<SparseMatrix<ArraySequence, Complex>>(r, c);
                } 
                else{
                    state_complex.B = std::make_unique<SparseMatrix<ArraySequence, Complex>>(r, c);
                }
                state_complex.Res = nullptr;
            }
            
            log = "Created Matrix " + target;
        } 
        catch (const std::exception& e){
            log = "Error: " + std::string(e.what());
        }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/set", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "Success";
        std::string dt = "int";
        try{
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");
            size_t tr = body.value("r", 0);
            size_t tc = body.value("c", 0);

            if (dt == "int"){
                int new_val = static_cast<int>(body.value("val_re", 0.0));
                size_t R = 0;
                size_t C = 0;
                if (target == "A"){
                    R = state_int.A->Get_rows();
                    C = state_int.A->Get_cols();
                }
                else{
                    R = state_int.B->Get_rows();
                    C = state_int.B->Get_cols();
                }

                ArraySequence<Matrix_elem<int>> new_seq;
                for (size_t r = 0; r < R; ++r){
                    for (size_t c = 0; c < C; ++c){
                        if (r == tr && c == tc){
                            if (new_val != 0){
                                new_seq.Append(Matrix_elem<int>(new_val, r, c));
                            }
                        } 
                        else{
                            int ext = 0;
                            if (target == "A"){
                                ext = state_int.A->Get_elem(r, c);
                            }
                            else{
                                ext = state_int.B->Get_elem(r, c);
                            }
                            if (ext != 0){
                                new_seq.Append(Matrix_elem<int>(ext, r, c));
                            }
                        }
                    }
                }
                if (target == "A"){
                    state_int.A = std::make_unique<SparseMatrix<ArraySequence, int>>(new_seq, R, C);
                } 
                else{
                    state_int.B = std::make_unique<SparseMatrix<ArraySequence, int>>(new_seq, R, C);
                }
                state_int.Res = nullptr;

            } 
            else if (dt == "double"){
                double new_val = static_cast<double>(body.value("val_re", 0.0));
                size_t R = 0;
                size_t C = 0;
                if (target == "A"){
                    R = state_double.A->Get_rows();
                    C = state_double.A->Get_cols();
                }
                else{
                    R = state_double.B->Get_rows();
                    C = state_double.B->Get_cols();
                }

                ArraySequence<Matrix_elem<double>> new_seq;
                for (size_t r = 0; r < R; ++r){
                    for (size_t c = 0; c < C; ++c){
                        if (r == tr && c == tc){
                            if (new_val != 0.0){
                                new_seq.Append(Matrix_elem<double>(new_val, r, c));
                            }
                        } 
                        else{
                            double ext = 0.0;
                            if (target == "A"){
                                ext = state_double.A->Get_elem(r, c);
                            }
                            else{
                                ext = state_double.B->Get_elem(r, c);
                            }
                            if (ext != 0.0){
                                new_seq.Append(Matrix_elem<double>(ext, r, c));
                            }
                        }
                    }
                }
                if (target == "A"){
                    state_double.A = std::make_unique<SparseMatrix<ArraySequence, double>>(new_seq, R, C);
                } 
                else{
                    state_double.B = std::make_unique<SparseMatrix<ArraySequence, double>>(new_seq, R, C);
                }
                state_double.Res = nullptr;

            } 
            else if (dt == "complex"){
                Complex new_val = Complex(body.value("val_re", 0.0), body.value("val_im", 0.0));
                size_t R = 0;
                size_t C = 0;
                if (target == "A"){
                    R = state_complex.A->Get_rows();
                    C = state_complex.A->Get_cols();
                }
                else{
                    R = state_complex.B->Get_rows();
                    C = state_complex.B->Get_cols();
                }

                ArraySequence<Matrix_elem<Complex>> new_seq;
                for (size_t r = 0; r < R; ++r){
                    for (size_t c = 0; c < C; ++c){
                        if (r == tr && c == tc){
                            if (new_val != Complex()){
                                new_seq.Append(Matrix_elem<Complex>(new_val, r, c));
                            }
                        } 
                        else{
                            Complex ext = Complex();
                            if (target == "A"){
                                ext = state_complex.A->Get_elem(r, c);
                            }
                            else{
                                ext = state_complex.B->Get_elem(r, c);
                            }
                            if (ext != Complex()){
                                new_seq.Append(Matrix_elem<Complex>(ext, r, c));
                            }
                        }
                    }
                }
                if (target == "A"){
                    state_complex.A = std::make_unique<SparseMatrix<ArraySequence, Complex>>(new_seq, R, C);
                } 
                else{
                    state_complex.B = std::make_unique<SparseMatrix<ArraySequence, Complex>>(new_seq, R, C);
                }
                state_complex.Res = nullptr;
            }
            
            log = "Updated cell in Matrix " + target;
        } 
        catch (const std::exception& e){
            log = "Error: " + std::string(e.what());
        }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/add", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "";
        std::string dt = "int";
        try{
            dt = json::parse(req.body).value("dataType", "int");
            if (dt == "int"){
                state_int.Res = std::make_unique<SparseMatrix<ArraySequence, int>>(*(state_int.A) + *(state_int.B));
            } 
            else if (dt == "double"){
                state_double.Res = std::make_unique<SparseMatrix<ArraySequence, double>>(*(state_double.A) + *(state_double.B));
            } 
            else if (dt == "complex"){
                state_complex.Res = std::make_unique<SparseMatrix<ArraySequence, Complex>>(*(state_complex.A) + *(state_complex.B));
            }
            log = "Addition Successful";
        } 
        catch (const Exception& e){
            log = "Math Error: " + std::string(e.what());
        } 
        catch (const std::exception& e){
            log = "System Error: " + std::string(e.what());
        }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/mult", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "";
        std::string dt = "int";
        try{
            dt = json::parse(req.body).value("dataType", "int");
            if (dt == "int"){
                state_int.Res = std::make_unique<SparseMatrix<ArraySequence, int>>(*(state_int.A) * *(state_int.B));
            } 
            else if (dt == "double"){
                state_double.Res = std::make_unique<SparseMatrix<ArraySequence, double>>(*(state_double.A) * *(state_double.B));
            } 
            else if (dt == "complex"){
                state_complex.Res = std::make_unique<SparseMatrix<ArraySequence, Complex>>(*(state_complex.A) * *(state_complex.B));
            }
            log = "Multiplication Successful";
        } 
        catch (const Exception& e){
            log = "Math Error: " + std::string(e.what());
        } 
        catch (const std::exception& e){
            log = "System Error: " + std::string(e.what());
        }
        res.set_content(build_response(dt, log), "application/json");
    });

    svr.Post("/api/norm", [](const httplib::Request& req, httplib::Response& res){
        std::string log = "";
        std::string dt = "int";
        try{
            auto body = json::parse(req.body);
            dt = body.value("dataType", "int");
            std::string target = body.value("target", "A");

            if (dt == "int"){
                int n = 0;
                if (target == "A"){
                    n = state_int.A->Get_Norm();
                }
                else{
                    n = state_int.B->Get_Norm();
                }
                log = "Norm ||" + target + "|| = " + format_val(n);
            } 
            else if (dt == "double"){
                double n = 0.0;
                if (target == "A"){
                    n = state_double.A->Get_Norm();
                }
                else{
                    n = state_double.B->Get_Norm();
                }
                log = "Norm ||" + target + "|| = " + format_val(n);
            } 
            else if (dt == "complex"){
                Complex n = Complex();
                if (target == "A"){
                    n = state_complex.A->Get_Norm();
                }
                else{
                    n = state_complex.B->Get_Norm();
                }
                log = "Norm ||" + target + "|| = " + format_val(n);
            }
        } 
        catch (const std::exception& e){
            log = "Error: " + std::string(e.what());
        }
        res.set_content(build_response(dt, log), "application/json");
    });

    std::cout << "\n[Sparse Matrix GUI] -> http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}