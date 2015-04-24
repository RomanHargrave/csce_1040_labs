// C++11 Please

#include <string>
#include <iostream>

using namespace std;

class Employee {

    private:

        string name;

    public:

        Employee(string name) {
            this->name = name;
        }

        string getName() {
            return name;
        }

        virtual string toString() {
            return "Employee()";
        }

};

class Manager : public Employee {

    public:

        Manager(string name)
            : Employee(name)
        {}

        string toString() {
            return "Manager( " + getName() + " )";
        }

};

class Worker : public Employee {

    public:

        Worker(string name) 
            : Employee(name)
        {}

        string toString() {
            return "Worker( " + getName() + " )";
        }
};

class Officer : public Employee {

    public:

        Officer(string name)
            : Employee(name)
        {}

        string toString() {
            return "Officer( " + getName() + " )";
        }
};

class Technician : public Employee {

    public:

        Technician(string name)
            : Employee(name)
        {}

        string toString() {
            return "Technician( " + getName() + " )";
        }

};

static void printEmployee(Employee& toPrint) {
    cout << toPrint.toString() << endl;
}

int main (void) {

    Employee    rafa ( "Rafa"  );
    Manager     mario( "Mario" );
    Worker      anton( "Anton" );
    Officer     luis ( "Luis"  );
    Technician  pablo( "Pablo" );

    printEmployee(rafa  );
    printEmployee(mario );
    printEmployee(anton );
    printEmployee(luis  );
    printEmployee(pablo );

    cout << "Ya he terminado." << endl;

    return 0;
}
