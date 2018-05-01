#pragma once

struct Expression
{
    struct ConstExpression;
    
    template <typename ConstT>
    Expression(ConstT value, typename std::enable_if<std::is_arithmetic<ConstT>::value>::type* = 0)
    : _data(std::make_shared<Model<ConstExpression>>(value))
    {
    }
    
    template <typename ExprT>
    Expression(ExprT expr, typename std::enable_if<!std::is_arithmetic<ExprT>::value>::type* = 0)
    : _data(std::make_shared<Model<ExprT>>(std::move(expr)))
    {
    }

    double operator[](double i) const
    {
        return (*_data)[i];
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
        Model(ExprT expr) : _data(std::move(expr)) { }
        
        double operator[](double i) const override
        {
            return _data[i];
        }
        
    private:
        ExprT _data;
    };
    
    std::shared_ptr<Contract> _data;
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
    ConstExpression(double val) : _val(val) { }
    
    double operator[](double i) const
    {
        return _val;
    }
    
private:
    double _val;
};

struct Function
{
    Function(double (*func)(double), Expression expr) : _func(func), _expr(expr) { }
    
    double operator[](double i) const
    {
        return _func(_expr[i]);
    }
    
private:
    std::function<double(double)> _func;
    Expression _expr;
};

template <typename OperationT>
struct Operation
{
    Operation(Expression lhs, Expression rhs)
    : _lhs(std::move(lhs)), _rhs(std::move(rhs))
    {}
    
    double operator[](double i) const
    {
        OperationT operation;
        return operation(_lhs[i], _rhs[i]);
    }
    
private:
    Expression _lhs;
    Expression _rhs;
};

struct PlotSamples
{
    void pushBack(double x, double y)
    {
        pushBack(juce::Point<double>(x, y));
    }
    
    void pushBack(juce::Point<double> sample)
    {
        _samples.push_back(std::move(sample));
    }
    
    double operator[](double i) const
    {
        if (i < _samples[0].getX() || i > _samples.back().getX())
            return std::numeric_limits<double>::quiet_NaN();
        
        juce::Point<double> searchValue {i, 0};
        auto it = std::lower_bound(_samples.begin(), _samples.end(), searchValue,
            [](auto& element, auto& value) { return element.getX() < value.getX(); });

        if (it == _samples.begin()) return _samples[0].getY();

        auto& p1 = *it;
        auto& p0 = *(it -1);
        
        return (p0.getY() * (p1.getX() - i) + p1.getY() * (i - p0.getX())) / (p1.getX() - p0.getX());
    }
    
private:
    mutable double lastX_;
    mutable std::size_t _lastI;
    
    std::vector<juce::Point<double>> _samples;
};


const static Expression x = XExpression {};

[[maybe_unused]]
static Expression operator*(Expression lhs, Expression rhs)
{
    return Operation<std::multiplies<double>>(lhs, rhs);
}

[[maybe_unused]]
static Expression operator+(Expression lhs, Expression rhs)
{
    return Operation<std::plus<double>>(lhs, rhs);
}

[[maybe_unused]]
static Expression sin(Expression expr)
{
    return plot::Function(std::sin, expr);
}



