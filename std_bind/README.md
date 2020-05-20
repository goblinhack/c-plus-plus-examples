How to use std::bind
====================

TODO
```C++
TODO
```
TODO
```C++
TODO
```
TODO

Here is the full example:
```C++
#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <random>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>

using namespace std::placeholders;  // for _1, _2, _3...

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
    friend int deposit (int cash, const BankAccount<int> &&account) {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, const BankAccount<int> &account) {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, BankAccount<int> &account) {
        account.deposit(cash);
        return account.cash;
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

void deposit_ammount (int cash, BankAccount<int> &b)
{
    b.deposit(cash);
}

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);

        // create a bind to a static function that invokes a method
        //
        // std::ref is not normally needed unless it is something, like
        // say a bitfield that cannot have a direct reference and needs
        // to be wrapped by std::reference
        //
        auto f1 = std::bind(deposit_ammount, _1, std::ref(account1));
        f1(100);

        // create a bind to a method directly
        auto f2 = std::bind(&BankAccount<int>::deposit, &account1,_1);
        f2(100);

        // check the deposits succeeded
        account1.check_balance(200);

        std::cout << "SUCCESS: account1 deposit succeeded!" << std::endl;
        std::cout << account1.to_string() << std::endl;

        // end
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_bind
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffeee1991c8, cash $0)

# create a bind to a static function that invokes a method
deposit cash called BankAccount(0x7ffeee1991c8, cash $100)

# create a bind to a method directly
deposit cash called BankAccount(0x7ffeee1991c8, cash $200)

# check the deposits succeeded
# SUCCESS: account1 deposit succeeded!
BankAccount(0x7ffeee1991c8, cash $200)

# end
delete account BankAccount(0x7ffeee1991c8, cash $200)
</pre>
