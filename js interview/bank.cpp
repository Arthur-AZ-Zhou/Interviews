#include <bits/stdc++.h>
#include <cmath>
using namespace std;

// Base Account class
class Account {
protected:
    string owner;
    double balance;

public:
    Account(string owner, double balance = 0) : owner(owner), balance(balance) {
        cout << "Made account under: " << owner << " with initial balance of: " << balance << endl; 
    }

    void deposit(double amount) {
        balance += amount;
    }
    
    virtual bool withdraw(double amount) = 0; // pure virtual

    bool transfer(Account& to, double amount) {
        if (withdraw(amount)) {
            to.deposit(amount);
            cout << "Src balance: " << balance << ", dest balance: " << to.getBalance() << endl;
            return true;
        }

        cout << "Source account too poor" << endl;
        return false;
    }

    double getBalance() const {
        return balance;
    }

    string getOwner() const {
        return owner;
    }
};

// SavingsAccount with interest
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(string owner, double balance = 0, double interestRate = 0.02) : Account(owner, balance) {
        cout << "Account type is savings with interestRate: " << interestRate << endl;
        this->interestRate = interestRate;
    }

    virtual bool withdraw(double amount) {
        balance -= amount;
        cout << "remaining balance: " << balance << endl;
        return true;
    }

    void applyInterestSimple(int numYears) {
        balance *= (1.0 + interestRate * numYears);
    }

    void applyInterestCompounding(int numYears) {
        balance *= pow(1.0 + interestRate, numYears); 
    }
};

// CheckingAccount with overdraft protection
class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(string owner, double balance = 0, double overdraftLimit = 100) : Account(owner, balance) {
        cout << "Account type is checking with overdraft of: " << overdraftLimit << endl;
        this->overdraftLimit = overdraftLimit;
    }

    virtual bool withdraw(double amount) {
        if (balance - amount < -overdraftLimit) {
            cout << "current balance is too low!" << endl;
            return false;
        }
        
        balance -= amount;
        cout << "remaining balance: " << balance << endl;
        return true;
    }
};

// ------------------- TEST (main) -------------------
int main() {
    SavingsAccount alice("Alice", 1000, 0.05);
    CheckingAccount bob("Bob", 500, 200);

    cout << "Before transfer:\n";
    cout << alice.getOwner() << ": " << alice.getBalance() << endl;
    cout << bob.getOwner() << ": " << bob.getBalance() << endl;

    // 🚨 Test your implementation here:
    alice.transfer(bob, 300);  // should succeed
    bob.transfer(alice, 800);  // should fail (overdraft limit exceeded)

    cout << "\nAfter transfer:\n";
    cout << alice.getOwner() << ": " << alice.getBalance() << endl;
    cout << bob.getOwner() << ": " << bob.getBalance() << endl;
}
