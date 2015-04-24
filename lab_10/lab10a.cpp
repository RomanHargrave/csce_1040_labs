#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

// trim strings
string trim(string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

class IAction {
    public:
        virtual ~IAction() {}
        virtual void apply() = 0; 
};

class ActionDeposit: public IAction {
    public:
        virtual void apply() {
            cout << "Deposit Selected" << endl;
        }
};

class ActionWithdraw: public IAction {
    public:
        virtual void apply() {
            cout << "Withdraw Selected" << endl;
        }
};

class ActionBalance: public IAction {
    public:
        virtual void apply() {
            cout << "Balance Selected" << endl;
        }
};

class ActionQuit: public IAction {
    public:
        virtual void apply() {
            cout << "Goodbye" << endl;
            exit(0);
        }
};

class Bank {
    private:
        map<string, IAction*> actions;
    public:
        void addAction(string actionKey, IAction* action) {
            actions.insert(pair<string, IAction*>(actionKey, action));
        }

        void run() {
            while (true) { // exit loop via `quit` action or sigterm
                string action;

                cout << "bank> ";
                cin >> action;
                trim(action);

                IAction* taken;
                {
                    map<string, IAction*>::iterator mapSearch = actions.find(action);
                    if(mapSearch != actions.end()) {
                        taken = mapSearch->second;
                    } else {
                        cout << "No suitable action found" << endl;
                        continue;
                    }
                }

                taken->apply();
            }
        }
};

int main (void) {

    Bank runtime;

    // Register actions
    runtime.addAction(string("deposit"),    new ActionDeposit());
    runtime.addAction(string("withdraw"),   new ActionWithdraw());
    runtime.addAction(string("balance"),    new ActionBalance());
    runtime.addAction(string("quit"),       new ActionQuit());

    // Pass over to Bank::run()
    
    runtime.run();
}
