// C++11 Please
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

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

class MenuTask {

    private:

        string name;
        string description;

    protected:

        MenuTask(string name, string description) {
            this->name        = name;
            this->description = description;
        }

    public:

        string getName() {
            return name;
        }

        string getDescription() {
            return description;
        }

        virtual void apply(vector<Employee*>&) = 0;

};

class TaskCreateEmployee: public MenuTask {

    private:

        Employee* createOfficer() {
            string name;
            float salary;
            string role;

            cout << "Officer Name: ";
            cin >> name;

            cout << "Officer Salary: ";
            cin >> salary;

            cout << "Officer Role: ";
            cin >> role;

            return new Officer( name, salary, role );
        }

        Employee* createManager() {
            string name;
            float salary;
            string degree;

            cout << "Manager Name: ";
            cin >> name;

            cout << "Manager Salary: ";
            cin >> salary;

            cout << "Manager Degree: ";
            cin >> degree;

            return new Manager( name, salary, degree );
        }

        Employee* createTechnician() {
            string name;
            float salary;
            string field;

            cout << "Technician Name: ";
            cin >> name;

            cout << "Technician Salary: ";
            cin >> salary;

            cout << "Technician Field: ";
            cin >> field;

            return new Technician( name, salary, field );
        }

    public:
    
        TaskCreateEmployee()
            : MenuTask("create", "create a new employee")
        {}

        void apply(vector<Employee*>& employees) override {
            cout << "Create an [o]fficer [m]anager or [t]echnician" << endl;

            Employee* newEmployee;
            {
                char action;
                cin >> action;
                switch (action) {
                    case 'o':
                        newEmployee = createOfficer();
                        break;
                    case 'm':
                        newEmployee = createManager();
                        break;
                    case 't':
                        newEmployee = createTechnician();
                        break;
                    default:
                        cout << "Not an action" << endl;
                        return;
                }
            }

            employees.push_back( newEmployee );
            cout << "Employee added" << endl;
        }

};

class TaskListEmployees: public MenuTask {

    private:

        void printEmployee(Employee* that) {
            cout << that->toString() << endl;
        }

    public:

        TaskListEmployees()
            : MenuTask("ls", "list employees")
        {}

        void apply(vector<Employee*>& employees) override {
            vector<Employee*>::iterator iterator = employees.begin();
            while(iterator != employees.end()) {
                printEmployee(*iterator);
                ++iterator;
            }
        }
};

class TaskSearchEmployees: public MenuTask {

    private: 

        struct QueryTester {
            string fragment;

            void operator() (Employee* employee) {
                if (employee->getName().find(fragment) != string::npos) {
                    cout << employee->toString() << endl;
                }
            }
        };

    public:

        TaskSearchEmployees()
            : MenuTask("find", "search employees by name")
        {}

        void apply(vector<Employee*>& employees) override {

            string fragment;
            cout << "Name Fragment: ";
            cin >> fragment;

            {
                QueryTester tester { .fragment = fragment };

                for_each (employees.begin(), employees.end(), tester);
            }

        }

};

class TaskExitApplication: public MenuTask {

    public:

        TaskExitApplication()
            : MenuTask("exit", "exit the application")
        {}

        void apply(vector<Employee*>& unused) override {
            cout << "Bye!" << endl;
            exit (EXIT_SUCCESS);
        }

};

class Menu {

    private:

        map<string, MenuTask*> tasks;

        void dumpTasks() {
            map<string, MenuTask*>::iterator iterator = tasks.begin();

            while (iterator != tasks.end()) {
                MenuTask* task = iterator->second;
                cout << "Command `" << task->getName() << "`: " << task->getDescription() << endl;
                ++iterator;
            }
        }

    public:

        void addTask(MenuTask* task) {
            tasks[task->getName()] = task;
        }

        void run(vector<Employee*>& employees) {
            while (!cin.eof()) {
                dumpTasks();
                cout << "> ";
                string action;
                cin >> action;

                {
                    MenuTask* taskPtr = tasks[action];
                    if (taskPtr) {
                        taskPtr->apply(employees);
                        cout << "`" << taskPtr->getName() << "` finished." << endl;
                    } else {
                        cout << "I don't have a task named '" << action << "'" << endl;
                        continue;
                    }
                }
            }           
        }

};

int main (void) {


    Menu menu;
    {   
        menu.addTask( new TaskCreateEmployee()  );
        menu.addTask( new TaskListEmployees()   );
        menu.addTask( new TaskSearchEmployees() );
        menu.addTask( new TaskExitApplication() );
    }

    vector<Employee*> employeeList;
    menu.run(employeeList);

    return 0;
}
