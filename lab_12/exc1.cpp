// C++11 Please!

#include <string>
#include <iostream>

using namespace std;

class Employee {

    protected:

        double salary;

    public:

        Employee(double pay) {
            this->salary = pay;
        }

        virtual double getPayment() {
            return salary;
        }

        string toString() {
            return "Employee(Pay = " + to_string(getPayment()) + " )";
        }

};

class Manager : public Employee {

    private:

        double wedge;

    public:

        Manager(double pay, double wedgeParam)
            : Employee(pay)
        {
            this->wedge = wedgeParam;
        }

        double getPayment() override {
            return salary * wedge;
        }

};


int main (void) {

    Employee employee( 1500 );
    Manager  manager ( 1500, 1.5 );

    cout << "Exercise about inheritance and polymorphism" << endl;

    cout << employee.toString() << endl;
    cout << manager.toString() << endl;

}
