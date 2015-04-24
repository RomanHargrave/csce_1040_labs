#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

// trim strings
string trim(string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

class Account {
    private:
        int balance;

    public:
        Account() {
            balance = 0;
        }

        int getBalance() {
            return balance;
        }

        int deposit(int amount) {
            return (balance += amount);
        }

        int withdraw(int amount) {
            return (balance -= amount);
        }
};

class Bank {
    private:
        map<int, Account*> accounts;

    public:
        int addAccount(Account* toAdd) {
            int number = 0;
            if(!accounts.empty()) {
                number = accounts.size();
            }

            accounts.insert(pair<int, Account*>(number, toAdd));
            return number;
        }

        Account* getAccount(int num) {
            map<int, Account*>::iterator iter = accounts.find(num);
            if(iter == accounts.end()) {
                return NULL;
            } else {
                return iter->second;
            }
        }
};

class IAction {
    public:
        virtual ~IAction() {}
        virtual void apply(Bank& bank, stringstream& args) = 0; 
};

class ActionDeposit: public IAction {
    public:
        virtual void apply(Bank& bank, stringstream& args) {
            int accountNum;
            int amount;

            args >> accountNum;
            args >> amount;

            {
                Account* acct = bank.getAccount(accountNum);
                if(acct) {
                    cout << "Depositing " << amount << " in to " << accountNum << endl;
                    int balance = acct->deposit(amount);
                    cout << "Balance is now " << balance << endl;
                } else {
                    cout << "Could not find an account with that number" << endl;
                }
            }
        }
};

class ActionWithdraw: public IAction {
    public:
        virtual void apply(Bank& bank, stringstream& args) {
            int accountNum;
            int amount;

            args >> accountNum;
            args >> amount;

            {
                Account* acct = bank.getAccount(accountNum);
                if(acct) {
                    cout << "Withrdawing " << amount << " from " << accountNum << endl;
                    int balance = acct->withdraw(amount);
                    cout << "Balance is now " << balance << endl;
                } else {
                    cout << "Could not find an account with that number" << endl;
                }
            }
        }
};

class ActionBalance: public IAction {
    public:
        virtual void apply(Bank& bank, stringstream& args) {
            int accountNum;

            args >> accountNum;

            {
                Account* acct = bank.getAccount(accountNum);
                if(acct) {
                    int balance = acct->getBalance();
                    cout << "Balance is " << balance << endl;
                } else {
                    cout << "Could not find an account with the number" << accountNum << endl;
                }
            }
        }
};

class ActionTransfer: public IAction {
    public:
        virtual void apply(Bank& bank, stringstream& args) {
            int accountFrom;
            int accountTo;
            int ammount;

            args >> accountFrom;
            args >> accountTo;
            args >> ammount;

            {
                Account* from;
                Account* to;
                {
                    from = bank.getAccount(accountFrom);
                    if(!from) {
                        cout << "Could not find originating account" << endl;
                        return;
                    }

                    to = bank.getAccount(accountTo);
                    if(!to) {
                        cout << "Could not find destination account" << endl;
                        return;
                    }
                }

                if(ammount > from->getBalance()) {
                    cout << "Funds exceeded" << endl;
                    return;
                } else {
                    from->withdraw(ammount);
                    to->deposit(ammount);
                }
            }

        }   
};

class ActionQuit: public IAction {
    public:
        virtual void apply(Bank& bank, stringstream& args) {
            cout << "Goodbye" << endl;
            exit(0);
        }
};

class Dispatcher {
    private:
        map<string, IAction*> actions;
        Bank* bank;
    public:
        Dispatcher(Bank* theBank) {
            bank = theBank;
        }

        void addAction(string actionKey, IAction* action) {
            actions.insert(pair<string, IAction*>(actionKey, action));
        }

        void run() {
            while (true) { // exit loop via `quit` action or sigterm
                string args;

                cout << "bank> ";
                getline(cin, args);
                trim(args);

                stringstream argsArray;
                argsArray.str(args);

                IAction* taken;
                {
                    string action;
                    argsArray >> action;
                    map<string, IAction*>::iterator mapSearch = actions.find(action);
                    if(mapSearch != actions.end()) {
                        taken = mapSearch->second;
                    } else {
                        cout << "No suitable action found for " << action << endl;
                        continue;
                    }
                }

                taken->apply(*bank, argsArray);
            }
        }
};

int main (void) {

    Bank theBank;

    for(int i = 0; i < 10; ++i) {
        theBank.addAccount(new Account());
    }

    Dispatcher runtime(&theBank);

    // Register actions
    runtime.addAction(string("deposit"),    new ActionDeposit());
    runtime.addAction(string("withdraw"),   new ActionWithdraw());
    runtime.addAction(string("balance"),    new ActionBalance());
    runtime.addAction(string("quit"),       new ActionQuit());
    runtime.addAction(string("transfer"),   new ActionTransfer());

    // Pass over to Bank::run()
    runtime.run();

    return 0;
}
