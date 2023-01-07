#include "../common/common.h"
#include <algorithm>
#include <functional> // for _1, _2
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

template < class T > class BankAccount;

template < class T > class BankAccount
{
private:
  T cash {};

public:
  BankAccount() { std::cout << "default constructor " << to_string() << std::endl; }
  BankAccount(T cash) : cash(cash) { std::cout << "new cash " << to_string() << std::endl; }
  BankAccount(const BankAccount &o)
  {
    std::cout << "copy cash constructor called for " << o.to_string() << std::endl;
    cash = o.cash;
    std::cout << "copy cash constructor result is  " << to_string() << std::endl;
  }
  ~BankAccount() { std::cout << "delete account " << to_string() << std::endl; }
  void deposit(const T &deposit)
  {
    cash += deposit;
    std::cout << "deposit cash called " << to_string() << std::endl;
  }
  using CheckTransactionCallback = std::function< void(T) >;
  int check_transaction(int cash, CheckTransactionCallback fn)
  {
    if (cash < 100) {
      throw std::string("transaction is too small for Mr Money Bags");
    } else {
      fn(cash);
    }
    return cash;
  }
  T    balance(void) const { return cash; }
  bool check_balance(T expected) const
  {
    if (cash == expected) {
      return true;
    } else {
      throw std::string("account has different funds " + to_string() + " than expected " + std::to_string(expected));
    }
  }
  friend std::ostream &operator<<(std::ostream &os, const BankAccount< T > &o)
  {
    os << "$" << std::to_string(o.cash);
    return os;
  }
  std::string to_string(void) const
  {
    auto              address = static_cast< const void              *>(this);
    std::stringstream ss;
    ss << address;
    return "BankAccount(" + ss.str() + ", cash $" + std::to_string(cash) + ")";
  }
};

template < class T > class BankCustomer;

template < class T > class BankCustomer
{
private:
  std::string      name {};
  BankAccount< T > account;

public:
  BankCustomer(void) { std::cout << "default customer " << to_string() << std::endl; }
  BankCustomer(const std::string &name) : name(name)
  {
    std::cout << "new temporary customer " << to_string() << std::endl;
  }
  BankCustomer(const std::string &name, const BankAccount< T > &account) : name(name), account(account)
  {
    std::cout << "new customer " << to_string() << std::endl;
  }
  ~BankCustomer() { std::cout << "delete customer " << to_string() << std::endl; }
  std::string          to_string(void) const { return "Customer(" + name + ", " + account.to_string() + ")"; }
  friend std::ostream &operator<<(std::ostream &os, const BankCustomer< T > &o)
  {
    os << o.to_string();
    return os;
  }
  friend bool operator<(const class BankCustomer< T > &lhs, const class BankCustomer< T > &rhs)
  {
    return lhs.name > rhs.name;
  }
};

static void backward_sort(void)
{
  DOC("Backward sorted set");
  std::set< std::string, std::greater< std::string > > m;

  m.insert("zaphod");
  m.insert("zaphod"); // Should be ignored
  m.insert("universe");
  m.insert("arthur");
  m.insert("marvin");
  m.insert("mice");
  m.insert("vogon");

  for (auto i : m) {
    std::cout << i << std::endl;
  }
}

static void forward_sort(void)
{
  DOC("Forward sorted set");
  std::set< std::string, std::less< std::string > > m;

  m.insert("zaphod");
  m.insert("universe");
  m.insert("arthur");
  m.insert("marvin");
  m.insert("mice");
  m.insert("vogon");

  for (auto i : m) {
    std::cout << i << std::endl;
  }
}

static void default_sort(void)
{
  DOC("Default sorted set");
  std::set< std::string > m;

  m.insert("zaphod");
  m.insert("universe");
  m.insert("arthur");
  m.insert("marvin");
  m.insert("mice");
  m.insert("vogon");

  for (auto i : m) {
    std::cout << i << std::endl;
  }
}

static void account_demo(void)
{
  DOC("Create a std::set of BankCustomer -> Account");
  using Account  = BankAccount< int >;
  using Customer = BankCustomer< int >;
  using TheBank  = std::set< Customer >;

  //
  // Notice, Zaphod has two accounts and the set allows both
  //
  TheBank customers;
  customers.insert(Customer("Arthur", Account(100)));
  customers.insert(Customer("Zaphod", Account(100000)));
  if (! customers.insert(Customer("Zaphod", Account(999999))).second) {
    DOC("Someone (Zaphod, let's face it) tried to add a 2nd account!");
  }
  customers.insert(Customer("Marvin", Account(0)));
  customers.insert(Customer("TheMice", Account(666)));
  customers.insert(Customer("Ford", Account(10)));

  //
  // Two ways to print this. One a simple loop. the other a lambda:
  //
  DOC("All customers, sorted by wealth");
  for (const auto &b : customers) {
    std::cout << b << std::endl;
  }

  auto show_all_bank_accounts = ([](const TheBank &customers) {
    DOC("All customers, sorted by wealth (lambda version)");
    for (const auto &b : customers) {
      std::cout << b << std::endl;
    }
  });

  show_all_bank_accounts(customers);

  DOC("Find Zaphod via find");
  auto f = customers.find(Customer("Zaphod"));
  if (f != customers.end()) {
    std::cout << *f << std::endl;
  }

  DOC("Find customers via equal_range");
  for (const auto &customer : customers) {
    using Iter                  = std::set< Customer >::iterator;
    std::pair< Iter, Iter > ret = customers.equal_range(customer);
    for (auto iter = ret.first; iter != ret.second; iter++) {
      std::cout << *iter << std::endl;
    }
  }

  DOC("Get rid of a customer");
  customers.erase(Customer("Zaphod"));

  DOC("Get rid of all customers");
  customers.clear();

  DOC("End");
}

int main(int, char **)
{
  backward_sort();
  forward_sort();
  default_sort();
  account_demo();
}
