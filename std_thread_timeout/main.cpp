#include "../common/common.h"
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>

//
// Return a timestamp std::string to we can see how long things take
//
std::string timestamp(void)
{
  auto now      = std::chrono::system_clock::now();
  auto seconds  = std::chrono::time_point_cast< std::chrono::seconds >(now);
  auto mseconds = std::chrono::duration_cast< std::chrono::milliseconds >(now - seconds);
  auto date     = std::chrono::system_clock::to_time_t(now);

  struct tm local_time;
  localtime_r(&date, &local_time);

  char buffer[ 128 ];
  buffer[ 0 ]      = '\0';
  auto buffer_size = sizeof(buffer) - 1;
  auto out         = strftime(buffer, buffer_size, "%H:%M:%S", &local_time);
  out += snprintf(buffer + out, buffer_size - out, ".%03d ", (int) mseconds.count());

  return std::string(buffer);
}

int my_function_that_might_block(int x)
{
  std::cout << "Function begins at :" << timestamp() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Function ends at   :" << timestamp() << std::endl;
  return 1;
}

int wrap_my_slow_function()
{
  std::mutex              my_mutex;
  std::condition_variable my_condition_var;
  int                     result;

  std::unique_lock< std::mutex > my_lock(my_mutex);

  //
  // Spawn a thread to call my_function_that_might_block().
  // Pass in the condition variables and result by reference.
  //
  std::thread t([ &my_condition_var, &result ]() {
    result = my_function_that_might_block(1);
    // Unblocks one of the threads currently waiting for this condition.
    my_condition_var.notify_one();
  });

  //
  // Detaches the thread represented by the object from the calling
  // thread, allowing them to execute independently from each other. B
  //
  t.detach();

  if (my_condition_var.wait_for(my_lock, std::chrono::seconds(1)) == std::cv_status::timeout) {
    //
    // Throw an exception so the caller knows we failed
    //
    std::cout << "Timed out at       :" << timestamp() << std::endl;
    throw std::runtime_error("Timeout");
  }

  return result;
}

int main()
{
  DOC("Run a function that might block");

  try {
    wrap_my_slow_function();
    //
    // Success, no timeout
    //
  } catch (std::runtime_error &e) {
    //
    // Do whatever you need here upon timeout failure
    //
    return 1;
  }

  DOC("End");

  return 0;
}
