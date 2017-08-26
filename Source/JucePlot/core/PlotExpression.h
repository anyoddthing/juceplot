#pragma once

struct Expression
{
    struct ConstExpression;
    
    template <typename ConstT>
    Expression(ConstT value, typename std::enable_if<std::is_arithmetic<ConstT>::value>::type* = 0)
    : data_(std::make_shared<Model<ConstExpression>>(value))
    {
    }
    
    template <typename ExprT>
    Expression(ExprT expr, typename std::enable_if<!std::is_arithmetic<ExprT>::value>::type* = 0)
    : data_(std::make_shared<Model<ExprT>>(std::move(expr)))
    {
    }

    double operator[](double i) const
    {
        return (*data_)[i];
    }
    
private:
    
    struct Contract
    {
        virtual ~Contract() = default;
        virtual double operator[](double i) const = 0;
    };
    
    template <typename ExprT>
    struct Model : virtual Contract
    {
        Model(ExprT expr) : data_(std::move(expr)) { }
        
        double operator[](double i) const override
        {
            return data_[i];
        }
        
    private:
        ExprT data_;
    };
    
    std::shared_ptr<Contract> data_;
};

struct XExpression
{
    double operator[](double i) const
    {
        return i;
    }
};

struct ConstExpression
{
    ConstExpression(double val) : val_(val) { }
    
    double operator[](double i) const
    {
        return val_;
    }
    
private:
    double val_;
};

struct Function
{
    Function(double (*func)(double), Expression expr) : func_(func), expr_(expr) { }
    
    double operator[](double i) const
    {
        return func_(expr_[i]);
    }
    
private:
    std::function<double(double)> func_;
    Expression expr_;
};

template <typename OperationT>
struct Operation
{
    Operation(Expression lhs, Expression rhs)
    : lhs_(std::move(lhs)), rhs_(std::move(rhs))
    {}
    
    double operator[](double i) const
    {
        OperationT operation;
        return operation(lhs_[i], rhs_[i]);
    }
    
private:
    Expression lhs_;
    Expression rhs_;
};


const static Expression x = XExpression {};

static Expression operator*(Expression lhs, Expression rhs)
{
    return Operation<std::multiplies<double>>(lhs, rhs);
}

static Expression operator+(Expression lhs, Expression rhs)
{
    return Operation<std::plus<double>>(lhs, rhs);
}

static Expression sin(Expression expr)
{
    return plot::Function(std::sin, expr);
}



