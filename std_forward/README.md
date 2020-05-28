How to use std::forward
=======================

std::forward's aim is to make it easier to pass along the exact type of function
prototype that is given. For example consider before std::forward you would have
to define all combinations of a template e.g.:
```C++
template <typename A, typename B< void f(A& a, B& b) { }
template <typename A, typename B< void f(const A& a, B& b) { }
template <typename A, typename B< void f(A& a, const B& b) { }
template <typename A, typename B< void f(A& a, B& b) { }
template <typename A, typename B< void f(const const A& a, const B& b) { }
```
This is tedious and does not scale. Now with std::forward we need to define only one template:
```C++
template<typename A, typename Account> void f(A&& a, B&& b) {
    f(std::forward<Account>(a), std::forward<Account>(b));
}
```
I struggled to find a useful non generic example of std::forward, but hit upon an
example of a bank account that we pass along the cash to be deposited as an argument.

So if we have a const version of an account we should expect when we pass it to our
deposit template<> that the const function is called; and this then throws an exception 
(the idea being this was a locked account!)

If we have a non const account then we should be able to modify the account.

As a final aside, std::forward is more or less eqivalent to static_cast<U&&>
althought there are apparently some corner cases where this is not true; but
I'm not aware of them, so please let me know :)

Here is the full example:
```C++
#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move
#include <utility>
#include <iostream>
#include <functional>

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

template<typename T, typename Account>
int process_deposit(T cash, Account&& b) {
    return deposit(cash, std::forward<Account>(b));
}

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);
        process_deposit<int>(100, account1);
        std::cout << account1.to_string() << std::endl;
        std::cout << "SUCCESS: account1 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }

    try {
        // create locked account2 and try to deposit into it; this should fail
        const auto account2 = BankAccount<int>(0);
        process_deposit<int>(100, account2);
        std::cout << account2.to_string() << std::endl;
        std::cout << "SUCCESS: account2 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account2 deposit failed!: " << e << std::endl;
    }

    try {
        // create locked account3 and try to deposit into it; this should fail
        auto account3 = BankAccount<int>(0);
        process_deposit<int>(100, std::move(account3));
        std::cout << account3.to_string() << std::endl;
        std::cout << "SUCCESS: account3 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account3 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_forward
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee558f680, cash $0)
deposit cash called BankAccount(0x7ffee558f680, cash $100)
BankAccount(0x7ffee558f680, cash $100)
# SUCCESS: account1 deposit succeeded!
delete account BankAccount(0x7ffee558f680, cash $100)

# create locked account2 and try to deposit into it; this should fail
new cash BankAccount(0x7ffee558f640, cash $0)
delete account BankAccount(0x7ffee558f640, cash $0)
# FAILED: account2 deposit failed!: tried to write to a locked (const) account

# create locked account3 and try to deposit into it; this should fail
new cash BankAccount(0x7ffee558f600, cash $0)
delete account BankAccount(0x7ffee558f600, cash $0)
# FAILED: account3 deposit failed!: tried to write to a locked (const) account
</pre>
