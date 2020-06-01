#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <set>
#include "../common/common.h"

template<class T> class BankAccount;

template<class T> class BankAccount {
private:
    T cash {};
public:
    BankAccount<T> () {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    BankAccount<T> (T cash) : cash (cash) {
        std::cout << "new cash " << to_string() << std::endl;
    }
    BankAccount<T> (const BankAccount& o) {
        std::cout << "copy cash constructor called for " << o.to_string() << std::endl;
        cash = o.cash;
        std::cout << "copy cash constructor result is  " << to_string() << std::endl;
    }
    ~BankAccount<T> () {
        std::cout << "delete account " << to_string() << std::endl;
    }
    void deposit (const T& deposit) {
        cash += deposit;
        std::cout << "deposit cash called " << to_string() << std::endl;
    }
    using CheckTransactionCallback = std::function<void(T)>;
    int check_transaction (int cash, CheckTransactionCallback fn) {
        if (cash < 100) {
            throw std::string("transaction is too small for Mr Money Bags");
        } else {
            fn(cash);
        }
        return cash;
    }
    T balance (void) const {
        return cash;
    }
    bool check_balance (T expected) const {
        if (cash == expected) {
            return true;
        } else {
            throw std::string("account has different funds " +
                              to_string() + " than expected " +
                              std::to_string(expected));
        }
    }
    friend std::ostream& operator<<(std::ostream &os, const BankAccount<T>& o) {
        os << "$" << std::to_string(o.cash);
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "BankAccount(" + ss.str() + ", cash $" + std::to_string(cash) + ")";
    }
};

template<class T> class BankCustomer;

template<class T> class BankCustomer {
private:
    std::string name {};
    BankAccount<T> account;
public:
    BankCustomer (void) { }
    BankCustomer (const std::string& name) : name(name) { }
    BankCustomer (const std::string& name, const BankAccount<T>& account) :
                    name(name), account(account) { }
    std::string to_string (void) const {
        return "Customer(" + name + ", " + account.to_string() + ")";
    }
    friend std::ostream& operator<<(std::ostream &os, const BankCustomer<T>& o) {
        os << o.to_string();
        return os;
    }
    friend bool operator<(const class BankCustomer<T> & lhs,
                          const class BankCustomer<T> & rhs) {
        return lhs.account.balance() > rhs.account.balance();
    }
};

static void backward_sort (void) {
    DOC("Backward sorted multiset");
    std::multiset< std::string, std::greater<std::string> > m;

    m.insert("zaphod");
    m.insert("universe");
    m.insert("arthur");
    m.insert("marvin");
    m.insert("mice");
    m.insert("vogon");

    for (auto i : m) { std::cout << i << std::endl; }
}

static void forward_sort (void) {
    DOC("Forward sorted multiset");
    std::multiset< std::string, std::less<std::string> > m;

    m.insert("zaphod");
    m.insert("universe");
    m.insert("arthur");
    m.insert("marvin");
    m.insert("mice");
    m.insert("vogon");

    for (auto i : m) { std::cout << i << std::endl; }
}

static void default_sort (void) {
    DOC("Default sorted multiset");
    std::multiset< std::string > m;

    m.insert("zaphod");
    m.insert("universe");
    m.insert("arthur");
    m.insert("marvin");
    m.insert("mice");
    m.insert("vogon");

    for (auto i : m) { std::cout << i << std::endl; }
}

static void account_demo (void)
{
    DOC("Create a std::multiset of BankCustomer -> Account");
    using Account = BankAccount<int>;
    using Customer = BankCustomer<int>;
    using TheBank = std::multiset<Customer>;

    //
    // Notice, Zaphod has two accounts and the multiset allows both
    //
    TheBank customers;
    customers.insert(Customer("Arthur",  Account(100)));
    customers.insert(Customer("Zaphod",  Account(100000)));
    customers.insert(Customer("Zaphod",  Account(999999)));
    customers.insert(Customer("Marvin",  Account(0)));
    customers.insert(Customer("TheMice", Account(666)));
    customers.insert(Customer("Ford",    Account(10)));

    //
    // Two ways to print this. One a simple loop. the other a lambda:
    //
    DOC("All customers, sorted by wealth");
    for (const auto& b : customers) {
        std::cout << b << std::endl;
    }

    auto show_all_bank_accounts = ([](const TheBank &customers)
        {
            DOC("All customers, sorted by wealth (lambda version)");
            for (const auto& b : customers) {
                std::cout << b << std::endl;
            }
        } );
    show_all_bank_accounts(customers);
}

int main(int, char**) {
    backward_sort();
    forward_sort();
    default_sort();
    account_demo();
}
