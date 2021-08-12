This program can be used in the following two ways:

1. Use the rb::set library as the substitute of std::set library.
   The rb::set library supports most of the std::set functionalities
   except allocators and some of the C++ 11/17/20 features.

2. Use the stand-alone script_intpr program to operate on rb::set<int>
   objects with either command-line scripting or using a input script
   file.


To run the script interpreter program in command-line mode, invoke the
program with no command-line parameter:

${PROGRAM_DIR}/script_intpr

To run the script interpreter program in script file mode, provide the
program with the directory of the script file:

${PROGRAM_DIR}/script_intpr ${SCRIPT_DIR}/${SCRIPT_FILENAME}


The supported command set and their functionalities are as follows:

// Set creation and management.

set_create     // Create an empty set. Assign the next free handle to
               // the created set.
set_create 0   // Create an empty set with handle 0. No-op if set 0
               // already exists.

set_copy 0     // Make a copy of set 0. Assign the next free handle to
               // the copied set.
set_copy 0 1   // Copy construct a set from 0 and give handle 1 to the
               // copied set. If set 1 already exists, perform a copy
               // assignment.

set_copy_assign 0 1    // Copy assign set 0 to set 1.

set_move 0 1           // Move set 0 to set 1.

set_move_assign 0 1    // Move assign set 0 to set 1.

set_swap 1 2           // Swap set 1 and set 2.

set_equal 1 2          // Check if set 1 equals to set 2.

set_destroy 0          // Destroy set 0.

set_destroy_all        // Destroy all existing sets.


// Basic data operations of the set.

set_insert 0 1234      // Insert 1234 into set 0.

set_erase 0 1234       // Erase 1234 from set 0.

set_modify 0 1234 4321 // modify element 1234 in set 0 to 4321.

set_contains 0 1234    // Check if set 0 contains element 1234.

set_print 0            // Print the content of set 0 to standard 
                       // output.

set_print_all          // Print the content of all existing set.

set_size 0             // Print the size of set 0 to standard output.

set_is_empty 0         // Check if set 0 is empty.

set_clear 0            // Clear the contents of set 0.


// Iterator-related commands.

iter_get_begin 0 1     // Get the begin iterator of set 0, and give 
                       // the iterator a handle of 1.

iter_get_end 0 2       // Get the one past the end iterator of set 0,
                       // and give the iterator a handle of 2.

iter_increase 2        // Increase iterator 2 by one step.

iter_increase 2 5      // Increase iterator 2 by 5 steps.

iter_decrease 2        // Decrease iterator 2 by one step.

iter_decrease 2 5      // Decrease iterator 2 by 5 steps.

iter_is_end 2          // Check if iterator 2 is at the end position.

iter_erase 2 0         // Erase the element pointed to by iterator 2 in
                       // set 0.

iter_discard 2         // Remove iterator 2.

iter_compare 2 3       // Check if iterator 2 and iterator 3 point to
                       // the same location in the same set. End
                       // iterators are the same no matter which set
                       // they are pointing to.

iter_find 0 2 1234     // Find element 1234 in set 0 and relocate
                       // iterator 2 to the position. If iterator 2
                       // does not exist, create iterator 2.

iter_print 2           // Print the content of iterator 2.


// Script interpreter command.

quit                   // Quit the script interpreter. All the existing
                       // sets will be outputted at termination.


Note:

Most of the exceptional cases handling description are removed from the
above documentation and are now handled by the program itself.

For example, if there are illegal parameters, instead of a no-op there
will be error information in the standard output. If the user tries to
find an element from a non-existing set, the program will skip this
command and warn that the set can not be found.

Therefore, some of the description from the project proposal is now
unnecessary in this document.