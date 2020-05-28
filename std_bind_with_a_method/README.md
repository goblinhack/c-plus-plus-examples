How to use std::bind with methods or proxy functions
====================================================

Sometimes we want to be able to bind to a method so we can call it directly,
or more likely pass that function around. There are a few ways to do this.
One is via a proxy function like:
```C++
    void proxy_function (int cash, BankAccount<int> &b)
    {
        b.deposit(cash);
    }

    auto f1 = std::bind(proxy_function, _1, account1);
    f1(100);
```
Or without the proxy:
```C++
    auto f2 = std::bind(&BankAccount<int>::deposit, &account1, _1);
    f2(100);
```
Here is the full example:
```C++
#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>

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
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);

        // invoke a bind to a method via proxy
        auto f1 = std::bind(proxy_function, _1, account1);
        f1(100);

        //
        // std::ref is not normally needed unless it is something, like
        // say a bitfield that cannot have a direct reference and needs
        // to be wrapped by std::reference
        //
        // invoke a bind to a method via proxy with std::ref
        auto f2 = std::bind(proxy_function, _1, std::ref(account1));
        f2(100);

        // invoke a bind to a method directly
        auto f3 = std::bind(&BankAccount<int>::deposit, &account1, _1);
        f3(100);

        // check the deposits succeeded
        account1.check_balance(300);

        // end
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_bind_with_a_method
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee0417f28, cash $0)

# invoke a bind to a method via proxy
copy cash constructor called for BankAccount(0x7ffee0417f28, cash $0)
copy cash constructor result is  BankAccount(0x7ffee0417f20, cash $0)
deposit cash called BankAccount(0x7ffee0417f20, cash $100)

# invoke a bind to a method via proxy with std::ref
deposit cash called BankAccount(0x7ffee0417f28, cash $100)

# invoke a bind to a method directly
deposit cash called BankAccount(0x7ffee0417f28, cash $200)

# check the deposits succeeded
delete account BankAccount(0x7ffee0417f20, cash $100)
delete account BankAccount(0x7ffee0417f28, cash $200)
# FAILED: account1 deposit failed!: account has different funds BankAccount(0x7ffee0417f28, cash $200) than expected 300
</pre>
