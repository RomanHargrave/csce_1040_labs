// vim: let g:syntastic_cpp_compiler_options="-std=c++11" :
// vim: set foldmethod=expr foldexpression={{{,}}} :
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <cctype>
#include <cstdlib> // atof

using namespace std;

// {{{ 

class MathToken {

    public:
        virtual ~MathToken() {}
        virtual bool isNumber() = 0;
        virtual double apply(MathToken& left, MathToken& right) = 0; 
        virtual double getValue() = 0;
        virtual string toString() = 0;

};

class DecimalToken: public MathToken {

    private:
        double value;

    public:
        DecimalToken(double val) {
            value = val;
        }

        bool isNumber() override { return true; }

        // XXX Exception needed
        double apply(MathToken& left, MathToken& right) override { return 0; }

        double getValue() override { return value; }

        string toString() override {
            return to_string(value);
        }
};

class MultiplyToken: public MathToken {

    public:
        bool isNumber() override { return false; }

        double apply(MathToken& left, MathToken& right) override {
            return left.getValue() * right.getValue();       
        }

        double getValue() override { return 0; }

        string toString() override { return string("+"); }

};

class DivideToken: public MathToken {

    public:
        bool isNumber() override { return false; }

        double apply(MathToken& left, MathToken& right) override {
            if(right.getValue() == 0) return 0;
            return left.getValue() / right.getValue();
        }

        double getValue() override { return 0; }

        string toString() override { return string("/"); }

};

class AddToken: public MathToken {

    public:
        bool isNumber() override { return false; }

        double apply(MathToken& left, MathToken& right) override {
            return left.getValue() + right.getValue();
        }

        double getValue() override { return 0; }

        string toString() override { return string("+"); }

};

class SubtractToken: public MathToken { 

    public:
        bool isNumber() override { return false; }

        double apply(MathToken& left, MathToken& right) override {
            return left.getValue() - right.getValue(); 
        }

        double getValue() override { return 0; }

        string toString() override {  return string("-"); }

};

// }}} 

static bool strIsNumber(string str) {
    stringstream stream(str);
    bool decimalEncountered = false;

    {
        char validating;
        while(stream.get(validating)) {
            if(isdigit(validating)) {
                continue;
            } else if(validating == '.' && !decimalEncountered) {
                decimalEncountered = true;
                continue;
            } else {
                return false;
            }
        }
    }

    return true;
}

static double processTokenChain(queue<MathToken*>& chain) { 
    
    stack<double> swap;

    while (!chain.empty()) {
     
        stack<MathToken*> operands;

        // Handle the swap value as an operand
        if (!swap.empty()) {
            operands.push( new DecimalToken( swap.top()) );
            swap.pop();
        }

        MathToken* oper;

        // Read operands until an operator is reached
        while(!chain.empty()) {
            MathToken* tok = chain.front(); chain.pop();
            if (tok->isNumber()) {
                operands.push(tok);
            } else if(operands.size() > 1) {
                oper = tok;
                break;
            } else {
                cerr << "At least two operands must preceed an operator" << endl;
                cerr << "Working token was " << tok->toString() << endl;
                return 0;
            }
        }

        // Process operation
        {
            MathToken* lastValue = operands.top(); operands.pop();
            
            while (!operands.empty()) {
                lastValue = new DecimalToken( oper->apply(*lastValue, *operands.top()) );
                operands.pop();
            }

            swap.push( lastValue->getValue() );

        }

    }

    return swap.top();

}

int main ( void ) {

    while (!cin.eof()) {
        string line;
        getline( cin, line );
        queue<MathToken*> opChain;

        {
            stringstream line_ops(line);
            string token;
            while (!line_ops.eof()) {
                line_ops >> token;
                if(strIsNumber(token)) {
                    opChain.push( new DecimalToken( ::atof(token.c_str()) ) );
                } else {
                    switch(token.at(0)) {
                        case '*':
                            opChain.push( new MultiplyToken() );
                            break;
                        case '/':
                            opChain.push( new DivideToken() );
                            break;
                        case '+':
                            opChain.push( new AddToken() );
                            break;
                        case '-':
                            opChain.push( new SubtractToken() );
                            break;
                        default:
                            cerr << "Invalid token " << token << endl;
                    }
                }

            }

        }

        double result = processTokenChain(opChain);
        cout << result << endl;
    }

    return 0;
}
