Modern C++ examples
===================

Are you looking for a collection of short straightforward snippets of
modern C++ covering the core features in C++11 and beyond? Of course 
you are!

As I'm constantly learning C++ myself (disclaimer, I'm not an expert,
just enthusiastic), I wanted to create some simple examples of each
of the main features of the language. You can build each example
individually or as a whole.

Let me know if this is useful to anyone or if there are any areas you want
covered. Suggestions, enhancements and errata welcome.

This is a (probably perpetual) work in progress!

[How to print hello world in color in C++, because why not](hello_world_color/README.md)

[How to use std::initializer_list](initializer_lists/README.md)

[How to use std::initializer_list with your own container like class](initializer_lists_with_custom_vector/README.md)

[How to use constexpr for fame and profit](constexpr/README.md)

[How to use range-based for loops](range_based_for_loop/README.md)

[How to use range-based for loops with your own iterator](range_based_for_loop_custom_begin_end/README.md)

[How to use std::sort with a custom container and iterators](std_sort_with_custom_iterators/README.md)

[How to use std::bind with simple example](std_bind/README.md)

[How to use std::bind with methods or proxy functions](std_bind_with_a_method/README.md)

[How to use std::bind with callback inside a class](std_bind_with_a_class_callback/README.md)

[How to use std::move to avoid the cost of temporaries](std_move/README.md)

[How to use std::forward to invoke the exactly correct function through a template](std_forward/README.md)

[How to create and use std::unique_ptr](std_unique_ptr/README.md)

[How to create and use std::shared_ptr](std_shared_ptr/README.md)

[How to make a wrapper around std::shared_ptr](std_shared_ptr_wrapper/README.md)

To build all the examples, just do:
```C++
   make 
```
To build all the examples and the documentation and run every examle, do:
```C++
   sh ./RUNME
```
