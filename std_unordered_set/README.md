How to use std::unordered_set
=============================

std::unordered_set allows insertion of unique elements but provides no
sorting mechanism. It is likely implemented as a hash table on the back
end. So all the normal concerns for hash tables apply.

Hash tables are generally O(1) and trees O(log N), but there are some big
caveats with hashes. If your hash table say had 10 elements and you added
10, 100, 1000, 10000 and they happen all to hash to the same slot, then
std::unordered_set is going to have linked list performance O(N). This is
obviously a worse case scenario, but things like that happen.

For std::unordered_set we need to provide a hash function when inserting
elements. That function will be invoked as std::hash<type>, so we need
to define it within the std namespace e.g.:
```C++
namespace std {
    template <class T> struct hash<BankCustomer<T> > {
        size_t operator()(const BankCustomer<T> & x) const noexcept {
            return std::hash<std::string>()(x.get_name());
        }
    };
};
```
Additionally we will need to provide comparitors for our class so that
the hasher can detect collisions. e.g.:
```C++
    friend bool operator==(const class BankCustomer<T> & lhs,
                           const class BankCustomer<T> & rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!=(const class BankCustomer<T> & lhs,
                           const class BankCustomer<T> & rhs) {
        return lhs.name != rhs.name;
    }
```
If you are not creating a custom class, then things are simpler. Here is
such an example. Notice below the 2nd insert of "Zaphod" is ignored as this
value exists already:
```C++
    std::unordered_set< std::string > m;

    m.insert("zaphod");
    m.insert("zaphod"); // oops
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
How do we detect insert fails? Well insert returns a pair:
```C++
    pair<iterator,bool> insert ( const value_type& x );
```
So we can just check for false in pair.second e.g.:
```C++
    using Account = BankAccount<int>;
    using Customer = BankCustomer<int>;
    using TheBank = std::unordered_set<Customer>;

    TheBank customers;
    customers.insert(Customer("Zaphod",  Account(100000)));
    if (!customers.insert(Customer("Zaphod", Account(999999))).second) {
        // Someone (Zaphod, let's face it) tried to add a 2nd account!
    }
```
Iteration over a std::unordered_map is simple:
```C++
    auto show_all_bank_accounts = ([](const TheBank &customers)
        {
            for (const auto& b : customers) {
                std::cout << b << std::endl;
            }
        } );

    show_all_bank_accounts(customers);
```
For finding elements, you can use surprisingly, find():
```C++
    auto f = customers.find(Customer("Zaphod"));
    if (f != customers.end()) {
        // found
    }
```
There is also std::equal_range which is really there for std::multiset
but all the set and map containers seem to support it, so here goes:
```C++
    using Iter = std::unordered_set<Customer>::iterator;
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
#include <unordered_set>

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
    std::string get_name (void) const {
        return name;
    }
    friend std::ostream& operator<<(std::ostream &os, const BankCustomer<T>& o) {
        os << o.to_string();
        return os;
    }
    friend bool operator==(const class BankCustomer<T> & lhs,
                           const class BankCustomer<T> & rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!=(const class BankCustomer<T> & lhs,
                           const class BankCustomer<T> & rhs) {
        return lhs.name != rhs.name;
    }
    bool operator==(const class BankCustomer<T> & o) {
        return name == o.name;
    }
    bool operator!=(const class BankCustomer<T> & o) {
        return name != o.name;
    }
};

namespace std
{
    template <class T> struct hash<BankCustomer<T> >
    {
        size_t operator()(const BankCustomer<T> & x) const noexcept {
            return std::hash<std::string>()(x.get_name());
        }
    };
}

static void insert_test (void) {
    // Default sorted set
    std::unordered_set< std::string > m;

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
    // Create a std::unordered_set of BankCustomer -> Account
    using Account = BankAccount<int>;
    using Customer = BankCustomer<int>;
    using TheBank = std::unordered_set<Customer>;

    //
    // Notice, Zaphod has two accounts and the set allows both
    //
    TheBank customers;
    customers.insert(Customer("Arthur",  Account(100)));
    customers.insert(Customer("Zaphod",  Account(100000)));
    if (!customers.insert(Customer("Zaphod", Account(999999))).second) {
        // Someone (Zaphod, let's face it) tried to add a 2nd account!
    }
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

    // Find Zaphod via find
    auto f = customers.find(Customer("Zaphod"));
    if (f != customers.end()) {
        std::cout << *f << std::endl;
    }

    // Find customers via equal_range
    for (const auto& customer : customers) {
        using Iter = std::unordered_set<Customer>::iterator;
        std::pair<Iter,Iter> ret = customers.equal_range(customer);
        for (auto iter = ret.first; iter != ret.second; iter++) {
            std::cout << *iter << std::endl;
        }
    }

    // Get rid of a customer
    customers.erase(Customer("Zaphod"));

    // Get rid of all customers
    customers.clear();

    // End
}

int main(int, char**) {
    insert_test();
    account_demo();
}
```
To build:
<pre>
cd std_unordered_set
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Default sorted set
zaphod
marvin
arthur
universe
vogon
mice

# Create a std::unordered_set of BankCustomer -> Account
new cash BankAccount(0x7ffee046e7e0, cash $100)
copy cash constructor called for BankAccount(0x7ffee046e7e0, cash $100)
copy cash constructor result is  BankAccount(0x7ffee046e820, cash $100)
new customer Customer(Arthur, BankAccount(0x7ffee046e820, cash $100))
copy cash constructor called for BankAccount(0x7ffee046e820, cash $100)
copy cash constructor result is  BankAccount(0x7fcca45040a8, cash $100)
delete customer Customer(Arthur, BankAccount(0x7ffee046e820, cash $100))
delete account BankAccount(0x7ffee046e820, cash $100)
delete account BankAccount(0x7ffee046e7e0, cash $100)
new cash BankAccount(0x7ffee046e790, cash $100000)
copy cash constructor called for BankAccount(0x7ffee046e790, cash $100000)
copy cash constructor result is  BankAccount(0x7ffee046e7c8, cash $100000)
new customer Customer(Zaphod, BankAccount(0x7ffee046e7c8, cash $100000))
copy cash constructor called for BankAccount(0x7ffee046e7c8, cash $100000)
copy cash constructor result is  BankAccount(0x7fcca45040e8, cash $100000)
delete customer Customer(Zaphod, BankAccount(0x7ffee046e7c8, cash $100000))
delete account BankAccount(0x7ffee046e7c8, cash $100000)
delete account BankAccount(0x7ffee046e790, cash $100000)
new cash BankAccount(0x7ffee046e730, cash $999999)
copy cash constructor called for BankAccount(0x7ffee046e730, cash $999999)
copy cash constructor result is  BankAccount(0x7ffee046e768, cash $999999)
new customer Customer(Zaphod, BankAccount(0x7ffee046e768, cash $999999))
delete customer Customer(Zaphod, BankAccount(0x7ffee046e768, cash $999999))
delete account BankAccount(0x7ffee046e768, cash $999999)
delete account BankAccount(0x7ffee046e730, cash $999999)

# Someone (Zaphod, let's face it) tried to add a 2nd account!
new cash BankAccount(0x7ffee046e6f0, cash $0)
copy cash constructor called for BankAccount(0x7ffee046e6f0, cash $0)
copy cash constructor result is  BankAccount(0x7ffee046e728, cash $0)
new customer Customer(Marvin, BankAccount(0x7ffee046e728, cash $0))
copy cash constructor called for BankAccount(0x7ffee046e728, cash $0)
copy cash constructor result is  BankAccount(0x7fcca4504158, cash $0)
delete customer Customer(Marvin, BankAccount(0x7ffee046e728, cash $0))
delete account BankAccount(0x7ffee046e728, cash $0)
delete account BankAccount(0x7ffee046e6f0, cash $0)
new cash BankAccount(0x7ffee046e6a0, cash $666)
copy cash constructor called for BankAccount(0x7ffee046e6a0, cash $666)
copy cash constructor result is  BankAccount(0x7ffee046e6d8, cash $666)
new customer Customer(TheMice, BankAccount(0x7ffee046e6d8, cash $666))
copy cash constructor called for BankAccount(0x7ffee046e6d8, cash $666)
copy cash constructor result is  BankAccount(0x7fcca4504118, cash $666)
delete customer Customer(TheMice, BankAccount(0x7ffee046e6d8, cash $666))
delete account BankAccount(0x7ffee046e6d8, cash $666)
delete account BankAccount(0x7ffee046e6a0, cash $666)
new cash BankAccount(0x7ffee046e650, cash $10)
copy cash constructor called for BankAccount(0x7ffee046e650, cash $10)
copy cash constructor result is  BankAccount(0x7ffee046e688, cash $10)
new customer Customer(Ford, BankAccount(0x7ffee046e688, cash $10))
copy cash constructor called for BankAccount(0x7ffee046e688, cash $10)
copy cash constructor result is  BankAccount(0x7fcca45041f8, cash $10)
delete customer Customer(Ford, BankAccount(0x7ffee046e688, cash $10))
delete account BankAccount(0x7ffee046e688, cash $10)
delete account BankAccount(0x7ffee046e650, cash $10)

# All customers, sorted by wealth
Customer(TheMice, BankAccount(0x7fcca4504118, cash $666))
Customer(Marvin, BankAccount(0x7fcca4504158, cash $0))
Customer(Zaphod, BankAccount(0x7fcca45040e8, cash $100000))
Customer(Ford, BankAccount(0x7fcca45041f8, cash $10))
Customer(Arthur, BankAccount(0x7fcca45040a8, cash $100))

# All customers, sorted by wealth (lambda version)
Customer(TheMice, BankAccount(0x7fcca4504118, cash $666))
Customer(Marvin, BankAccount(0x7fcca4504158, cash $0))
Customer(Zaphod, BankAccount(0x7fcca45040e8, cash $100000))
Customer(Ford, BankAccount(0x7fcca45041f8, cash $10))
Customer(Arthur, BankAccount(0x7fcca45040a8, cash $100))

# Find Zaphod via find
default constructor BankAccount(0x7ffee046e608, cash $0)
new temporary customer Customer(Zaphod, BankAccount(0x7ffee046e608, cash $0))
delete customer Customer(Zaphod, BankAccount(0x7ffee046e608, cash $0))
delete account BankAccount(0x7ffee046e608, cash $0)
Customer(Zaphod, BankAccount(0x7fcca45040e8, cash $100000))

# Find customers via equal_range
Customer(TheMice, BankAccount(0x7fcca4504118, cash $666))
Customer(Marvin, BankAccount(0x7fcca4504158, cash $0))
Customer(Zaphod, BankAccount(0x7fcca45040e8, cash $100000))
Customer(Ford, BankAccount(0x7fcca45041f8, cash $10))
Customer(Arthur, BankAccount(0x7fcca45040a8, cash $100))

# Get rid of a customer
default constructor BankAccount(0x7ffee046e588, cash $0)
new temporary customer Customer(Zaphod, BankAccount(0x7ffee046e588, cash $0))
delete customer Customer(Zaphod, BankAccount(0x7fcca45040e8, cash $100000))
delete account BankAccount(0x7fcca45040e8, cash $100000)
delete customer Customer(Zaphod, BankAccount(0x7ffee046e588, cash $0))
delete account BankAccount(0x7ffee046e588, cash $0)

# Get rid of all customers
delete customer Customer(TheMice, BankAccount(0x7fcca4504118, cash $666))
delete account BankAccount(0x7fcca4504118, cash $666)
delete customer Customer(Marvin, BankAccount(0x7fcca4504158, cash $0))
delete account BankAccount(0x7fcca4504158, cash $0)
delete customer Customer(Ford, BankAccount(0x7fcca45041f8, cash $10))
delete account BankAccount(0x7fcca45041f8, cash $10)
delete customer Customer(Arthur, BankAccount(0x7fcca45040a8, cash $100))
delete account BankAccount(0x7fcca45040a8, cash $100)

# End
</pre>
