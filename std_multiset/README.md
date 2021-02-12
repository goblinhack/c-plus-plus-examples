How to use std::multiset
========================

Multisets do away with the restriction of uniqueness for their values,
compared to std::set where a value can exist only a single time. This
sounds a lot like std::map, and it is, with the big exception that maps
can have separate keys and values. For sets we only have value.

Multisets will have more of a performance impact than std::set as they
do need to cater for duplicate values. Hence if you do not need this,
consider std::set.

Multisets will require a means of sorting with winning comparisons being
ordered closer to the 'start' of the set e.g.:
```C++
    std::multiset< std::string, std::greater<std::string> > m;

    m.insert("zaphod");
    m.insert("universe");
    m.insert("arthur");
    m.insert("marvin");
    m.insert("mice");
    m.insert("vogon");
```
which yields:
```C++
    zaphod
    vogon
    universe
    mice
    marvin
    arthur
```
In our example we will provide the comparison operator so we can sort
accounts by balance:
```C++
    friend bool operator<(const class BankCustomer<T> & lhs,
                          const class BankCustomer<T> & rhs) {
        return lhs.account.balance() > rhs.account.balance();
    }
```
So the biggest accounts will appear first!

Note also that we can insert the same value multiple times. Here, zaphod
sneakily has two bank accounts:
```C++
    using Account = BankAccount<int>;
    using Customer = BankCustomer<int>;
    using TheBank = std::multiset<Customer>;

    TheBank customers;
    customers.insert(Customer("Zaphod", Account(100000)));
    customers.insert(Customer("Zaphod", Account(999999)));
```
And we can then print all accounts easily:
```C++
    auto show_all_bank_accounts = ([](const TheBank &customers)
        {
            for (const auto& b : customers) {
                std::cout << b << std::endl;
            }
        } );

    show_all_bank_accounts(customers);
```
Now what if you want to find something in a multiset? This is a little
bit more tricky as we have to be able to handle multiple values. For this,
std::equal_range comes to the rescue. We give this search criteria and
get back a pair of iterators e.g.:
```C++
    using Iter = std::multiset<Customer>::iterator;
    std::pair<Iter,Iter> ret = customers.equal_range(Customer("Zaphod"));
    for (auto iter = ret.first; iter != ret.second; iter++) {
        std::cout << "Zaphod has been found! " << *iter << std::endl;
    }
```
Removing a customer is easy via erase:
```C++
    customers.erase(account2);
```
And finally to remove everything and get rid of all your customers!:
```C++
    customers.clear();
```
Here is the full example:
```C++
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <set>

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
    BankCustomer (void) { 
        std::cout << "default customer " << to_string() << std::endl;
    }
    BankCustomer (const std::string& name) : name(name) {
        std::cout << "new temporary customer " << to_string() << std::endl;
    }
    BankCustomer (const std::string& name, const BankAccount<T>& account) :
                    name(name), account(account) {
        std::cout << "new customer " << to_string() << std::endl;
    }
    ~BankCustomer () {
        std::cout << "delete customer " << to_string() << std::endl;
    }
    std::string to_string (void) const {
        return "Customer(" + name + ", " + account.to_string() + ")";
    }
    friend std::ostream& operator<<(std::ostream &os, const BankCustomer<T>& o) {
        os << o.to_string();
        return os;
    }
    friend bool operator<(const class BankCustomer<T> & lhs,
                          const class BankCustomer<T> & rhs) {
        return lhs.name > rhs.name;
    }
};

static void backward_sort (void) {
    // Backward sorted multiset
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
    // Forward sorted multiset
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
    // Default sorted multiset
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
    // Create a std::multiset of BankCustomer -> Account
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
    // All customers, sorted by wealth
    for (const auto& b : customers) {
        std::cout << b << std::endl;
    }

    auto show_all_bank_accounts = ([](const TheBank &customers)
        {
            // All customers, sorted by wealth (lambda version)
            for (const auto& b : customers) {
                std::cout << b << std::endl;
            }
        } );

    show_all_bank_accounts(customers);

    // Find sneaky customers
    for (const auto& customer : customers) {
        using Iter = std::multiset<Customer>::iterator;
        std::pair<Iter,Iter> ret = customers.equal_range(customer);
        auto account_count = 0;
        for (auto iter = ret.first; iter != ret.second; iter++) {
            if (account_count++) {
                // Customer has two accounts!
                std::cout << *iter << std::endl;
            }
        }
    }

    // Get rid of a customer
    customers.erase(Customer("Zaphod"));

    // Get rid of all customers
    customers.clear();

    // End
}

int main(int, char**) {
    backward_sort();
    forward_sort();
    default_sort();
    account_demo();
}
```
To build:
<pre>
cd std_multiset
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Backward sorted multiset
zaphod
vogon
universe
mice
marvin
arthur

# Forward sorted multiset
arthur
marvin
mice
universe
vogon
zaphod

# Default sorted multiset
arthur
marvin
mice
universe
vogon
zaphod

# Create a std::multiset of BankCustomer -> Account
new cash BankAccount(0x7ffeed6fdbc0, cash $100)
copy cash constructor called for BankAccount(0x7ffeed6fdbc0, cash $100)
copy cash constructor result is  BankAccount(0x7ffeed6fdc00, cash $100)
new customer Customer(Arthur, BankAccount(0x7ffeed6fdc00, cash $100))
copy cash constructor called for BankAccount(0x7ffeed6fdc00, cash $100)
copy cash constructor result is  BankAccount(0x7f91d4c05c28, cash $100)
delete customer Customer(Arthur, BankAccount(0x7ffeed6fdc00, cash $100))
delete account BankAccount(0x7ffeed6fdc00, cash $100)
delete account BankAccount(0x7ffeed6fdbc0, cash $100)
new cash BankAccount(0x7ffeed6fdb78, cash $100000)
copy cash constructor called for BankAccount(0x7ffeed6fdb78, cash $100000)
copy cash constructor result is  BankAccount(0x7ffeed6fdbb0, cash $100000)
new customer Customer(Zaphod, BankAccount(0x7ffeed6fdbb0, cash $100000))
copy cash constructor called for BankAccount(0x7ffeed6fdbb0, cash $100000)
copy cash constructor result is  BankAccount(0x7f91d4c05c68, cash $100000)
delete customer Customer(Zaphod, BankAccount(0x7ffeed6fdbb0, cash $100000))
delete account BankAccount(0x7ffeed6fdbb0, cash $100000)
delete account BankAccount(0x7ffeed6fdb78, cash $100000)
new cash BankAccount(0x7ffeed6fdb30, cash $999999)
copy cash constructor called for BankAccount(0x7ffeed6fdb30, cash $999999)
copy cash constructor result is  BankAccount(0x7ffeed6fdb68, cash $999999)
new customer Customer(Zaphod, BankAccount(0x7ffeed6fdb68, cash $999999))
copy cash constructor called for BankAccount(0x7ffeed6fdb68, cash $999999)
copy cash constructor result is  BankAccount(0x7f91d4c05ca8, cash $999999)
delete customer Customer(Zaphod, BankAccount(0x7ffeed6fdb68, cash $999999))
delete account BankAccount(0x7ffeed6fdb68, cash $999999)
delete account BankAccount(0x7ffeed6fdb30, cash $999999)
new cash BankAccount(0x7ffeed6fdae8, cash $0)
copy cash constructor called for BankAccount(0x7ffeed6fdae8, cash $0)
copy cash constructor result is  BankAccount(0x7ffeed6fdb20, cash $0)
new customer Customer(Marvin, BankAccount(0x7ffeed6fdb20, cash $0))
copy cash constructor called for BankAccount(0x7ffeed6fdb20, cash $0)
copy cash constructor result is  BankAccount(0x7f91d4c05ce8, cash $0)
delete customer Customer(Marvin, BankAccount(0x7ffeed6fdb20, cash $0))
delete account BankAccount(0x7ffeed6fdb20, cash $0)
delete account BankAccount(0x7ffeed6fdae8, cash $0)
new cash BankAccount(0x7ffeed6fdaa0, cash $666)
copy cash constructor called for BankAccount(0x7ffeed6fdaa0, cash $666)
copy cash constructor result is  BankAccount(0x7ffeed6fdad8, cash $666)
new customer Customer(TheMice, BankAccount(0x7ffeed6fdad8, cash $666))
copy cash constructor called for BankAccount(0x7ffeed6fdad8, cash $666)
copy cash constructor result is  BankAccount(0x7f91d4c05d28, cash $666)
delete customer Customer(TheMice, BankAccount(0x7ffeed6fdad8, cash $666))
delete account BankAccount(0x7ffeed6fdad8, cash $666)
delete account BankAccount(0x7ffeed6fdaa0, cash $666)
new cash BankAccount(0x7ffeed6fda58, cash $10)
copy cash constructor called for BankAccount(0x7ffeed6fda58, cash $10)
copy cash constructor result is  BankAccount(0x7ffeed6fda90, cash $10)
new customer Customer(Ford, BankAccount(0x7ffeed6fda90, cash $10))
copy cash constructor called for BankAccount(0x7ffeed6fda90, cash $10)
copy cash constructor result is  BankAccount(0x7f91d4c05d68, cash $10)
delete customer Customer(Ford, BankAccount(0x7ffeed6fda90, cash $10))
delete account BankAccount(0x7ffeed6fda90, cash $10)
delete account BankAccount(0x7ffeed6fda58, cash $10)

# All customers, sorted by wealth
Customer(Zaphod, BankAccount(0x7f91d4c05c68, cash $100000))
Customer(Zaphod, BankAccount(0x7f91d4c05ca8, cash $999999))
Customer(TheMice, BankAccount(0x7f91d4c05d28, cash $666))
Customer(Marvin, BankAccount(0x7f91d4c05ce8, cash $0))
Customer(Ford, BankAccount(0x7f91d4c05d68, cash $10))
Customer(Arthur, BankAccount(0x7f91d4c05c28, cash $100))

# All customers, sorted by wealth (lambda version)
Customer(Zaphod, BankAccount(0x7f91d4c05c68, cash $100000))
Customer(Zaphod, BankAccount(0x7f91d4c05ca8, cash $999999))
Customer(TheMice, BankAccount(0x7f91d4c05d28, cash $666))
Customer(Marvin, BankAccount(0x7f91d4c05ce8, cash $0))
Customer(Ford, BankAccount(0x7f91d4c05d68, cash $10))
Customer(Arthur, BankAccount(0x7f91d4c05c28, cash $100))

# Find sneaky customers

# Customer has two accounts!
Customer(Zaphod, BankAccount(0x7f91d4c05ca8, cash $999999))

# Customer has two accounts!
Customer(Zaphod, BankAccount(0x7f91d4c05ca8, cash $999999))

# Get rid of a customer
default constructor BankAccount(0x7ffeed6fd9d8, cash $0)
new temporary customer Customer(Zaphod, BankAccount(0x7ffeed6fd9d8, cash $0))
delete customer Customer(Zaphod, BankAccount(0x7f91d4c05c68, cash $100000))
delete account BankAccount(0x7f91d4c05c68, cash $100000)
delete customer Customer(Zaphod, BankAccount(0x7f91d4c05ca8, cash $999999))
delete account BankAccount(0x7f91d4c05ca8, cash $999999)
delete customer Customer(Zaphod, BankAccount(0x7ffeed6fd9d8, cash $0))
delete account BankAccount(0x7ffeed6fd9d8, cash $0)

# Get rid of all customers
delete customer Customer(TheMice, BankAccount(0x7f91d4c05d28, cash $666))
delete account BankAccount(0x7f91d4c05d28, cash $666)
delete customer Customer(Ford, BankAccount(0x7f91d4c05d68, cash $10))
delete account BankAccount(0x7f91d4c05d68, cash $10)
delete customer Customer(Arthur, BankAccount(0x7f91d4c05c28, cash $100))
delete account BankAccount(0x7f91d4c05c28, cash $100)
delete customer Customer(Marvin, BankAccount(0x7f91d4c05ce8, cash $0))
delete account BankAccount(0x7f91d4c05ce8, cash $0)

# End
</pre>
