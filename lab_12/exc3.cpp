// C++11 Please

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Employee {

    private:

        string name;
        float  salary;

    public:

        Employee(string name, float salary) {
            this->name = name;
            this->salary = salary;
        }

        string getName() {
            return name;
        }

        float getSalary() {
            return salary;
        }

        virtual string toString() {
            return "Employee()";
        }

};

class Manager : public Employee {

    private:

        string degree;

    public:

        Manager(string name, float salary, string degree)
            : Employee(name, salary)
        {
            this->degree = degree;
        }

        string toString() {
            return "Manager(Name = " + getName() + ", Salary = " + to_string(getSalary())
                + ", Degree = " + degree + ")";
        }

};

class Worker : public Employee {

    private:

        string position;

    public:

        Worker(string name, float salary, string position) 
            : Employee(name, salary)
        {
            this->position = position;
        }

        string toString() {
            return "Worker(Name = " + getName() + ", Salary = " + to_string(getSalary()) 
                + ", Position = " + position + ")";
        }
};

class Officer : public Employee {

    private:

        string role;

    public:

        Officer(string name, float salary, string role)
            : Employee(name, salary)
        {
            this->role = role;
        }

        string toString() {
            return "Officer(Name = " + getName() + ", Salary = " + to_string(getSalary()) 
                + ", Role = " + role + ")";
        }
};

class Technician : public Employee {

    private:

        string field;

    public:

        Technician(string name, float salary, string field)
            : Employee(name, salary)
        {
            this->field = field;
        }

        string toString() {
            return "Technician(Name = " + getName() + ", Salary = " + to_string(getSalary()) 
                + ", Field = " + field + ")";
        }

};

static void printEmployee(Employee& toPrint) {
    cout << toPrint.toString() << endl;
}

int main (void) {

    vector<Employee*> employeeList;

    employeeList.push_back( new Manager    ( "Carla Garcia", 35000, "Economist"    ) );
    employeeList.push_back( new Manager    ( "Juan Pereze",  38000, "Engineer"     ) );
    employeeList.push_back( new Officer    ( "Pedro Egia",   18000, "Officer"      ) );
    employeeList.push_back( new Officer    ( "Luisa Penia",  15000, "Officer"      ) );
    employeeList.push_back( new Technician ( "Javier Ramos", 19500, "Welder"       ) );
    employeeList.push_back( new Technician ( "Amaia Bilbao", 12000, "Electrician"  ) );

    {
        vector<Employee*>::iterator employeeIterator = employeeList.begin();
        while (employeeIterator != employeeList.end()) {
            printEmployee(**employeeIterator);
            ++employeeIterator;
        }
    }

    return 0;
}
