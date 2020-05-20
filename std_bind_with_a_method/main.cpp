#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <random>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>
#include "../common/common.h"

using namespace std::placeholders;  // for _1, _2, _3...

template<class T> class BankAccount;

template<class T> class BankAccount {
private:
    const T no_cash {};
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
    // Transfer of funds?
    BankAccount<T> (BankAccount<T>&& o) {
        std::cout << "move cash called for " << o.to_string() << std::endl;
        cash = o.cash;
        o.cash = no_cash;
        std::cout << "move cash result is  " << to_string() << std::endl;
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

void proxy_function (int cash, BankAccount<int> &b)
{
    b.deposit(cash);
}

int main(int, char**)
{
    try {
        DOC("create account1 and try to deposit into it");
        auto account1 = BankAccount<int>(0);

        DOC("invoke a bind to a method via proxy");
        auto f1 = std::bind(proxy_function, _1, account1);
        f1(100);

        //
        // std::ref is not normally needed unless it is something, like
        // say a bitfield that cannot have a direct reference and needs
        // to be wrapped by std::reference
        //
        DOC("invoke a bind to a method via proxy with std::ref");
        auto f2 = std::bind(proxy_function, _1, std::ref(account1));
        f2(100);

        DOC("invoke a bind to a method directly");
        auto f3 = std::bind(&BankAccount<int>::deposit, &account1, _1);
        f3(100);

        DOC("check the deposits succeeded");
        account1.check_balance(300);

        DOC("end");
    } catch (const std::string &e) {
        FAILED("account1 deposit failed!: " + e);
    }
}
