#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move
#include <utility>
#include <iostream>
#include <functional>
#include "../common/common.h"

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
    //
    // Transfer of funds
    //
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
    friend int deposit (int cash, const BankAccount<int> &&account)
    {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, const BankAccount<int> &account)
    {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, BankAccount<int> &account)
    {
        account.deposit(cash);
        return account.cash;
    }
};

template<typename T, typename Account>
int process_deposit(T cash, Account&& b) {
    return deposit(cash, std::forward<Account>(b));
}

int main(int, char**)
{
    try {
        DOC("create account1 and try to deposit into it");
        auto account1 = BankAccount<int>(0);
        process_deposit<int>(100, account1);
        std::cout << account1.to_string() << std::endl;
        OK("account1 deposit succeeded!");
    } catch (const std::string &e) {
        ERR("account1 deposit failed!: " + e);
    }

    try {
        DOC("create locked account2 and try to deposit into it; this should fail");
        const auto account2 = BankAccount<int>(0);
        process_deposit<int>(100, account2);
        std::cout << account2.to_string() << std::endl;
        OK("account2 deposit succeeded!");
    } catch (const std::string &e) {
        ERR("account2 deposit failed!: " + e);
    }

    try {
        DOC("create locked account3 and try to deposit into it; this should fail");
        auto account3 = BankAccount<int>(0);
        process_deposit<int>(100, std::move(account3));
        std::cout << account3.to_string() << std::endl;
        OK("account3 deposit succeeded!");
    } catch (const std::string &e) {
        ERR("account3 deposit failed!: " + e);
    }
}
