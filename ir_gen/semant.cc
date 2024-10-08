#include "semant.h"
#include "../include/SysY_tree.h"
#include "../include/ir.h"
#include "../include/type.h"

extern LLVMIR llvmIR;

SemantTable semant_table;
std::vector<std::string> error_msgs{};

void __Program::TypeCheck() {
    semant_table.symbol_table.enter_scope();
    auto comp_vector = *comp_list;
    for (auto comp : comp_vector) {
        comp->TypeCheck();
    }
}

void Exp::TypeCheck() {
    addexp->TypeCheck();

    attribute = addexp->attribute;
}

void AddExp_plus::TypeCheck() {
    addexp->TypeCheck();
    mulexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void AddExp_sub::TypeCheck() {
    addexp->TypeCheck();
    mulexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void MulExp_mul::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    TODO("BinaryExp Semant");
}

void MulExp_div::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    TODO("BinaryExp Semant");
}

void MulExp_mod::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    TODO("BinaryExp Semant");
}

void RelExp_leq::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void RelExp_lt::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void RelExp_geq::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void RelExp_gt::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void EqExp_eq::TypeCheck() {
    eqexp->TypeCheck();
    relexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void EqExp_neq::TypeCheck() {
    eqexp->TypeCheck();
    relexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void LAndExp_and::TypeCheck() {
    landexp->TypeCheck();
    eqexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void LOrExp_or::TypeCheck() {
    lorexp->TypeCheck();
    landexp->TypeCheck();

    TODO("BinaryExp Semant");
}

void ConstExp::TypeCheck() {
    addexp->TypeCheck();
    attribute = addexp->attribute;
    if (!attribute.V.ConstTag) {    // addexp is not const
        error_msgs.push_back("Expression is not const " + std::to_string(line_number) + "\n");
    }
}

void Lval::TypeCheck() { TODO("Lval Semant"); }

void FuncRParams::TypeCheck() { TODO("FuncRParams Semant"); }

void Func_call::TypeCheck() { TODO("FunctionCall Semant"); }

void UnaryExp_plus::TypeCheck() { TODO("UnaryExp Semant"); }

void UnaryExp_neg::TypeCheck() { TODO("UnaryExp Semant"); }

void UnaryExp_not::TypeCheck() { TODO("UnaryExp Semant"); }

void IntConst::TypeCheck() {
    attribute.T.type = Type::INT;
    attribute.V.ConstTag = true;
    attribute.V.val.IntVal = val;
}

void FloatConst::TypeCheck() {
    attribute.T.type = Type::FLOAT;
    attribute.V.ConstTag = true;
    attribute.V.val.FloatVal = val;
}

void StringConst::TypeCheck() { TODO("StringConst Semant"); }

void PrimaryExp_branch::TypeCheck() {
    exp->TypeCheck();
    attribute = exp->attribute;
}

void assign_stmt::TypeCheck() { TODO("AssignStmt Semant"); }

void expr_stmt::TypeCheck() {
    exp->TypeCheck();
    attribute = exp->attribute;
}

void block_stmt::TypeCheck() { b->TypeCheck(); }

void ifelse_stmt::TypeCheck() {
    Cond->TypeCheck();
    if (Cond->attribute.T.type == Type::VOID) {
        error_msgs.push_back("if cond type is invalid " + std::to_string(line_number) + "\n");
    }
    ifstmt->TypeCheck();
    elsestmt->TypeCheck();
}

void if_stmt::TypeCheck() {
    Cond->TypeCheck();
    if (Cond->attribute.T.type == Type::VOID) {
        error_msgs.push_back("if cond type is invalid " + std::to_string(line_number) + "\n");
    }
    ifstmt->TypeCheck();
}

void while_stmt::TypeCheck() { TODO("WhileStmt Semant"); }

void continue_stmt::TypeCheck() { TODO("ContinueStmt Semant"); }

void break_stmt::TypeCheck() { TODO("BreakStmt Semant"); }

void return_stmt::TypeCheck() {
    return_exp->TypeCheck();

    if (return_exp->attribute.T.type == Type::VOID) {
        error_msgs.push_back("return type is invalid in line " + std::to_string(line_number) + "\n");
    }
}

void return_stmt_void::TypeCheck() {}

void ConstInitVal::TypeCheck() { TODO("ConstInitVal Semant"); }

void ConstInitVal_exp::TypeCheck() { TODO("ConstInitValExp Semant"); }

void VarInitVal::TypeCheck() { TODO("VarInitVal Semant"); }

void VarInitVal_exp::TypeCheck() { TODO("VarInitValExp Semant"); }

void VarDef_no_init::TypeCheck() { TODO("VarDefNoInit Semant"); }

void VarDef::TypeCheck() { TODO("VarDef Semant"); }

void ConstDef::TypeCheck() { TODO("ConstDef Semant"); }

void VarDecl::TypeCheck() { TODO("VarDecl Semant"); }

void ConstDecl::TypeCheck() { TODO("ConstDecl Semant"); }

void BlockItem_Decl::TypeCheck() { decl->TypeCheck(); }

void BlockItem_Stmt::TypeCheck() { stmt->TypeCheck(); }

void __Block::TypeCheck() {
    semant_table.symbol_table.enter_scope();
    auto item_vector = *item_list;
    for (auto item : item_vector) {
        item->TypeCheck();
    }
    semant_table.symbol_table.exit_scope();
}

void __FuncFParam::TypeCheck() {
    VarAttribute val;
    val.ConstTag = false;
    val.type = type_decl;
    scope = 1;

    if (dims != nullptr) {
        auto dim_vector = *dims;

        // the fisrt dim of FuncFParam is empty
        // eg. int f(int A[][30][40])
        val.dims.push_back(-1);
        for (int i = 1; i < dim_vector.size(); ++i) {
            auto d = dim_vector[i];
            d->TypeCheck();
            if (d->attribute.V.ConstTag == false) {
                error_msgs.push_back("Array Dim must be const expression in line " + std::to_string(line_number) +
                                     "\n");
            }
            if (d->attribute.T.type == Type::FLOAT) {
                error_msgs.push_back("Array Dim can not be float in line " + std::to_string(line_number) + "\n");
            }
            val.dims.push_back(d->attribute.V.val.IntVal);
        }
        attribute.T.type = Type::PTR;
    } else {
        attribute.T.type = type_decl;
    }

    if (name != nullptr) {
        if (semant_table.symbol_table.lookup_scope(name) != -1) {
            error_msgs.push_back("multiple difinitions of formals in function " + name->get_string() + " in line " +
                                 std::to_string(line_number) + "\n");
        }
        semant_table.symbol_table.add_Symbol(name, val);
    }
}

void __FuncDef::TypeCheck() {
    semant_table.symbol_table.enter_scope();

    semant_table.FunctionTable[name] = this;

    auto formal_vector = *formals;
    for (auto formal : formal_vector) {
        formal->TypeCheck();
    }

    // block TypeCheck
    if (block != nullptr) {
        auto item_vector = *(block->item_list);
        for (auto item : item_vector) {
            item->TypeCheck();
        }
    }

    semant_table.symbol_table.exit_scope();
}

void CompUnit_Decl::TypeCheck() { TODO("CompUnitDecl Semant"); }

void CompUnit_FuncDef::TypeCheck() { func_def->TypeCheck(); }