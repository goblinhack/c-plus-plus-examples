How to use std::bind with a class callback
==========================================

Sometimes we want to be able to invoke callbacks from our class. This can be
difficult in C++ as we need to have the context (i.e. the class object) somehow
associated with the function.

Thankfully, std::bind, can do this for us. Here we bind "account1" to one of
its methods.
```C++
    auto method = std::bind(&BankAccount<int>::deposit, &account1, _1);
    method(100);
```
This is identical to:
```C++
    account1.deposit(100);
```
So not too earth shattering. However, suppose we want to call a method in
our class and have it call another method within the same class (or potentially
another class) i.e. a callback.

Here we define the ability for "check_transaction" to accept a callack
function:
```C++
    using CheckTransactionCallback = std::function<void(T)>;
    int check_transaction (int cash, CheckTransactionCallback fn) {
        if (cash < 100) {
            throw std::string("transaction is too small for Mr Money Bags");
        } else {
            fn(cash);
        }
        return cash;
    }
```
And to invoke it:
```C++
    auto callback = std::bind(&BankAccount<int>::deposit, &account1, _1);
    account1.check_transaction(99, callback);
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

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);

        auto deposit_method = std::bind(&BankAccount<int>::deposit, &account1, _1);
        account1.check_transaction(100, deposit_method);
        account1.check_transaction(100, deposit_method);
        account1.check_balance(200);
        std::cout << "SUCCESS: account1 1st deposit succeeded!" << std::endl;

        //
        // This will fail as we catch the 'small' transaction
        //
        account1.check_transaction(99, deposit_method);
        std::cout << "SUCCESS: account1 2nd deposit succeeded!" << std::endl;

        // end
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_bind_with_a_class_callback
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee6ced540, cash $0)
deposit cash called BankAccount(0x7ffee6ced540, cash $100)
deposit cash called BankAccount(0x7ffee6ced540, cash $200)
# SUCCESS: account1 1st deposit succeeded!
delete account BankAccount(0x7ffee6ced540, cash $200)
# FAILED: account1 deposit failed!: transaction is too small for Mr Money Bags
</pre>
