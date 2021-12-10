#include "../common/common.h"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

static const auto thread_loop_count = 1000;

////////////////////////////////////////////////////////////////////////////
// Here we start a thread with a class wrapper. We will not complete
// destruction of this class until our thread is finished.
////////////////////////////////////////////////////////////////////////////
class MyThread
{
public:
  ~MyThread()
  {
    std::cout << to_string() << " destructor" << std::endl;
    //
    // Wait for our thread to finish
    //
    if (my_thread.joinable()) {
      my_thread.join();
    }
  }
  MyThread(const std::string &data, const int count) : data(data), count(count)
  {
    //
    // Start our thread.
    //
    my_thread = std::thread(&MyThread::run, this);
    std::cout << to_string() << " constructor" << std::endl;
  }
  std::string to_string(void) const
  {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return "MyThread(" + ss.str() + ")";
  }
  void run()
  {
    while (count--) {
      std::cout << data;
    }
  }

private:
  std::thread my_thread;
  std::string data;
  int         count;
};

static void thread_with_class_example()
{
  DOC("Start 2 threads with class wrappers");
  MyThread thread1("A", thread_loop_count);
  MyThread thread2("B", thread_loop_count);
  DOC("Wait for threads to finish");
}

////////////////////////////////////////////////////////////////////////////
// Here we start threads with a local function that is passed some context
////////////////////////////////////////////////////////////////////////////
struct thread_context {
  std::string output;
};
static void thread_fn(const thread_context &context)
{
  for (int i = 0; i < thread_loop_count; i++) {
    std::cout << context.output;
  }
}

static void thread_with_local_function_and_cotext()
{
  DOC("Start 2 threads implemented as function calls");
  auto c1 = thread_context {std::string("A")};
  auto c2 = thread_context {std::string("B")};
  auto a  = std::thread(thread_fn, std::ref(c1));
  auto b  = std::thread(thread_fn, std::ref(c2));
  a.join();
  b.join();
  DOC("Wait for threads to finish");
}

////////////////////////////////////////////////////////////////////////////
// Here we start threads with lambdas
////////////////////////////////////////////////////////////////////////////
static void thread_with_lambda()
{
  DOC("Start 2 threads implemented as lambdas");
  auto l1 = ([](void) {
    for (int i = 0; i < thread_loop_count; i++) {
      std::cout << "A";
    }
  });
  auto l2 = ([](void) {
    for (int i = 0; i < thread_loop_count; i++) {
      std::cout << "B";
    }
  });
  auto a  = std::thread(l1);
  auto b  = std::thread(l2);
  a.join();
  b.join();
  DOC("Wait for threads to finish");
}

int main()
{
  thread_with_class_example();
  thread_with_local_function_and_cotext();
  thread_with_lambda();

  DOC("End");
  return 0;
}
