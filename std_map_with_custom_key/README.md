How to use std::map with a custom key
=====================================

std::map provides a key to value mapping, where the key is sorted.
Behind the scenes std::map could be implemented as a tree or hashtable,
but the point is you don't need to worry about that. All you need to
do is provide a means to sort the keys via the "<" operator.

Now, many classes come with this operator already, hence you can do:
```C++
    std::map< std::string, int > m;
```
But if you want to provide your own sorter, that is easy too:
```C++
    bool operator< (const AccountNumber& rhs) const {
        return (val < rhs.val);
    }
```
For our example, let's make use of "using" to make things more readable:
For adding to a map, you must insert:
```C++
    using Account = BankAccount<int>;
    using Bank = std::map< AccountNumber, Account >;
    Bank thebank;
```
So now we have our key as AccountNumber and Account being our value.
Let's add an account. Note that to make a key value pair you use
std::make_pair:
```C++
    AccountNumber account1(101);
    Account       balance1(10000);
    thebank.insert(std::make_pair(account1, balance1));
```
Alternatively you can insert via []
```C++
    AccountNumber account2(102);
    Account       balance2(20000);
    thebank[account2] = balance2;
```
And finally, emplace can be used which avoids the copy for large objects:
```C++
    AccountNumber account3(103);
    Account       balance3(30000);
    thebank.emplace(std::make_pair(account3, balance3));
```
To check if an account exists, use find(). The return value is an iterator,
so to check for failure just compare to the end() iterator:
```C++
    if (thebank.find(account1) == thebank.end()) {
        DOC("No");
    } else {
        DOC("Yes");
    }
```
Removing an account is via erase:
```C++
    thebank.erase(account2);
```
And finally to remove everything and rob the bank, do:
```C++
    thebank.clear();
```
Here is the full example:
```C++
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>

class AccountNumber {
private:
    int val {};
public:
    AccountNumber (void) { }
    AccountNumber (int val) : val(val) { }
    bool operator< (const AccountNumber& rhs) const {
        return (val < rhs.val); 
    }
    std::string to_string (void) const {
        return "AccountNumber(" + std::to_string(val) + ")";
    }
    friend std::ostream& operator<<(std::ostream &os, const AccountNumber& o) {
        os << o.to_string();
        return os;
    }
};

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

int main(int, char**)
{
    // Create a std::map of AccountNumber -> Account
    using Account = BankAccount<int>;
    using Bank = std::map< const AccountNumber, Account >;
    Bank thebank;

    //
    // We're going to be fancy here and create a lambda we can use
    // to print the accounts.
    //
    auto show_all_bank_accounts = ([](const Bank &b)
        {
            // Show all bank accounts
            for (auto const &acc : b) {
                std::cout << acc.first << " " << acc.second << std::endl;
            }
        } );

    // Create some accounts
    AccountNumber account1(101);
    Account       balance1(10000);

    AccountNumber account2(102);
    Account       balance2(20000);

    AccountNumber account3(104);
    Account       balance3(30000);

    AccountNumber account4(104);
    Account       balance4(30000);

    // Add an account with insert()
    thebank.insert(std::make_pair(account1, balance1));

    // Add an account with map[k] = v
    thebank[account2] = balance2;

    // Add an account with emplace()
    thebank.emplace(std::make_pair(account3, balance3));

    show_all_bank_accounts(thebank);

    // Does account1 exist?
    if (thebank.find(account1) == thebank.end()) {
        // No
    } else {
        // Yes
    }

    // Does account4 exist?
    if (thebank.find(account4) == thebank.end()) {
        // No. Add it quick!
        thebank.insert(std::make_pair(account4, balance4));
    } else {
        // Yes
    }
    show_all_bank_accounts(thebank);

    // Remove account2
    thebank.erase(account2);
    show_all_bank_accounts(thebank);

    // Modify account3
    thebank[account3].deposit(100);
    show_all_bank_accounts(thebank);

    // Rob the bank
    thebank.clear();

    // End
}
```
To build:
<pre>
cd std_map_with_custom_key
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a std::map of AccountNumber -> Account

# Create some accounts
new cash BankAccount(0x7ffeeb41bfc0, cash $10000)
new cash BankAccount(0x7ffeeb41bfb0, cash $20000)
new cash BankAccount(0x7ffeeb41bfa0, cash $30000)
new cash BankAccount(0x7ffeeb41bf90, cash $30000)

# Add an account with insert()
copy cash constructor called for BankAccount(0x7ffeeb41bfc0, cash $10000)
copy cash constructor result is  BankAccount(0x7ffeeb41bf8c, cash $10000)
copy cash constructor called for BankAccount(0x7ffeeb41bf8c, cash $10000)
copy cash constructor result is  BankAccount(0x7fbd8e4029d0, cash $10000)
delete account BankAccount(0x7ffeeb41bf8c, cash $10000)

# Add an account with map[k] = v
default constructor BankAccount(0x7fbd8e402a00, cash $0)

# Add an account with emplace()
copy cash constructor called for BankAccount(0x7ffeeb41bfa0, cash $30000)
copy cash constructor result is  BankAccount(0x7ffeeb41bf74, cash $30000)
copy cash constructor called for BankAccount(0x7ffeeb41bf74, cash $30000)
copy cash constructor result is  BankAccount(0x7fbd8e402a30, cash $30000)
delete account BankAccount(0x7ffeeb41bf74, cash $30000)

# Show all bank accounts
AccountNumber(101) $10000
AccountNumber(102) $20000
AccountNumber(104) $30000

# Does account1 exist?

# Yes

# Does account4 exist?

# Yes

# Show all bank accounts
AccountNumber(101) $10000
AccountNumber(102) $20000
AccountNumber(104) $30000

# Remove account2
delete account BankAccount(0x7fbd8e402a00, cash $20000)

# Show all bank accounts
AccountNumber(101) $10000
AccountNumber(104) $30000

# Modify account3
deposit cash called BankAccount(0x7fbd8e402a30, cash $30100)

# Show all bank accounts
AccountNumber(101) $10000
AccountNumber(104) $30100

# Rob the bank
delete account BankAccount(0x7fbd8e4029d0, cash $10000)
delete account BankAccount(0x7fbd8e402a30, cash $30100)

# End
delete account BankAccount(0x7ffeeb41bf90, cash $30000)
delete account BankAccount(0x7ffeeb41bfa0, cash $30000)
delete account BankAccount(0x7ffeeb41bfb0, cash $20000)
delete account BankAccount(0x7ffeeb41bfc0, cash $10000)
</pre>
