#include "rb_set.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <format>

/********************************************************************

Type aliases and type definitions.

********************************************************************/

// Script interpreter supports operation on rb::set<int>.
using DataType = int;
using SetType = rb::set<DataType>;
using SizeType = std::size_t;

// Enum class for querying the entry of commands.
enum class CommandEntry
{
  // Set creation and management commands.
  set_create,
  set_copy,
  set_copy_assign,
  set_move,
  set_move_assign,
  set_swap,
  set_equal,
  set_destroy,
  set_destroy_all,

  // Basic data operation commands.
  set_insert,
  set_erase,
  set_modify,
  set_contains,
  set_print,
  set_size,
  set_is_empty,
  set_clear,

  // Iterator related commands.
  iter_get_begin,
  iter_get_end,
  iter_increase,
  iter_decrease,
  iter_is_end,
  iter_erase,
  iter_discard,
  iter_compare,
  iter_find,
  iter_print,

  // Print all sets that currently exists.
  set_print_all
};

// Mapping object that maps the string to its correlated
// command entry.
std::map<std::string, CommandEntry> command_map =
{
  // Set creation and management commands.
  {"set_create", CommandEntry::set_create},
  {"set_copy", CommandEntry::set_copy},
  {"set_copy_assign", CommandEntry::set_copy_assign},
  {"set_move", CommandEntry::set_move},
  {"set_move_assign", CommandEntry::set_move_assign},
  {"set_swap", CommandEntry::set_swap},
  {"set_equal", CommandEntry::set_equal},
  {"set_destroy", CommandEntry::set_destroy},
  {"set_destroy_all", CommandEntry::set_destroy_all},

  // Basic data operation commands.
  {"set_insert", CommandEntry::set_insert},
  {"set_erase", CommandEntry::set_erase},
  {"set_modify", CommandEntry::set_modify},
  {"set_contains", CommandEntry::set_contains},
  {"set_print", CommandEntry::set_print},
  {"set_size", CommandEntry::set_size},
  {"set_is_empty", CommandEntry::set_is_empty},
  {"set_clear", CommandEntry::set_clear},

  // Iterator related commands.
  {"iter_get_begin", CommandEntry::iter_get_begin},
  {"iter_get_end", CommandEntry::iter_get_end},
  {"iter_increase", CommandEntry::iter_increase},
  {"iter_decrease", CommandEntry::iter_decrease},
  {"iter_is_end", CommandEntry::iter_is_end},
  {"iter_erase", CommandEntry::iter_erase},
  {"iter_discard", CommandEntry::iter_discard},
  {"iter_compare", CommandEntry::iter_compare},
  {"iter_find", CommandEntry::iter_find},
  {"iter_print", CommandEntry::iter_print},

  // Print all sets that currently exists.
  {"set_print_all", CommandEntry::set_print_all}
};

/********************************************************************

Foward declearations.

********************************************************************/

// Default mode.
// Read script command from command line.
void command_line_mode();

// When script file is specified, read script commands
// from a given file. This mode is for easier testing.
void script_file_mode(const std::string& input_file_name);

// Parser function for splitting a script line into separate
// words for later interpretation.
std::vector<std::string>
line_parse(const std::string& script_line);

// Extract operands from a parsed line.
// Return an empty vector when failed.
std::vector<DataType> extract_operands(
  const std::vector<std::string>& parsed_line,
  std::size_t num_of_operands);

// Execute a parsed command line.
void parsed_line_executor(
  const std::vector<std::string>& parsed_line,
  std::map<DataType, rb::set<DataType>>& set_map,
  std::map<DataType, rb::set<DataType>::iterator>& iter_map);

/********************************************************************

Main function entry.

********************************************************************/

int main(int argc, char const *argv[])
{
  if (argc == 1) {
    command_line_mode();
  } else {
    std::string file_name{argv[1]};
    script_file_mode(file_name);
  }
  
  return 0;
}

/********************************************************************

Definitions of previously decleared functions.

********************************************************************/

void command_line_mode()
{
  std::cout << "Script Interpreter -- Command-line Mode\n";
  // Stores the sets to operate on.
  // Mapped from serial numbers to rb::set objects.
  std::map<DataType, rb::set<DataType>> set_map;
  // Stores the iterators to operate on.
  // Mapped from serial numbers to set iterator objects.
  std::map<DataType, rb::set<DataType>::iterator> iter_map;
  std::cout << ">> ";
  // Get each line from script file and execute it.
  std::string script_line;
  while (std::getline(std::cin, script_line)) {
    // Handle special cases of inputs.
    // Ignore commetns.
    if (script_line[0] == '/' && script_line[1] == '/')
    {
      continue;
    }
    if (script_line.empty()) {
      continue;
    }
    if (script_line == "quit") {
      break;
    }
    
    // Parse the line if it is not a comment, not empty or
    // not quit command.
    auto parsed_line = line_parse(script_line);

    // Execute the parsed line.
    parsed_line_executor(parsed_line, set_map, iter_map);

    std::cout << ">> ";
  }
  std::cout << "\nSet contents at termination: \n\n";
  for (const auto& x : set_map) {
    std::cout << std::format("Set {}\n"
      "================================================\n",
      x.first);
    for (const auto& i : x.second) {
      std::cout << i << '\n';
    }
    std::cout << '\n';
  }
}

void script_file_mode(const std::string& input_file_name)
{
  // Open a script file.
  std::ifstream script_file;
  script_file.open(input_file_name);

  // Stores the sets to operate on.
  // Mapped from serial numbers to rb::set objects.
  std::map<DataType, rb::set<DataType>> set_map;
  // Stores the iterators to operate on.
  // Mapped from serial numbers to set iterator objects.
  std::map<DataType, rb::set<DataType>::iterator> iter_map;

  // Get each line from script file and execute it.
  std::string script_line;
  while (std::getline(script_file, script_line)) {
    // Handle special cases of inputs.
    // Print comments in the script.
    if (script_line[0] == '/' && script_line[1] == '/')
    {
      std::cout << script_line << '\n';
      continue;
    }
    if (script_line.empty())
    {
      continue;
    }
    if (script_line == "quit") {
      break;
    }
    
    // Parse the line if it is not a comment, not empty or
    // not quit command.
    auto parsed_line = line_parse(script_line);

    // Execute the parsed line.
    parsed_line_executor(parsed_line, set_map, iter_map);
  }
  std::cout << "\nSet contents at termination: \n\n";
  for (const auto& x : set_map) {
    std::cout << std::format("Set {}\n"
      "================================================\n",
      x.first);
    for (const auto& i : x.second) {
      std::cout << i << '\n';
    }
    std::cout << '\n';
  }
}

std::vector<std::string>
line_parse(const std::string& script_line)
{
  std::string line_word;
  std::vector<std::string> parsed_line;

  for (auto& x : script_line) {
    if (x == ' ') {
      if (!line_word.empty()) {
        parsed_line.push_back(line_word);
        line_word.clear();
      }
    } else {
      line_word.append(std::string{x});
    }
  }
  parsed_line.push_back(line_word);

  return parsed_line;
}

std::vector<DataType> extract_operands(
  const std::vector<std::string>& parsed_line,
  std::size_t num_of_operands)
{
  std::vector<DataType> operands;
  for (int i = 0; i < num_of_operands; ++i) {
    try {
      operands.push_back(std::stoi(parsed_line[i + 1]));
    } catch (...) {
      std::cout << "Illegal parameters.\n";
      operands.clear();
      return operands;
    }
  }
  return operands;
}

void parsed_line_executor(
  const std::vector<std::string>& parsed_line,
  std::map<DataType, rb::set<DataType>>& set_map,
  std::map<DataType, rb::set<DataType>::iterator>& iter_map)
{ 
  // Check if the command belongs to the command list.
  if (command_map.find(parsed_line[0]) == command_map.end()) {
    std::cout << "Command not found.\n";
    return;
  }

  // Command found in the map. Get the correlated entry for it.
  CommandEntry command_entry = command_map[parsed_line[0]];
  switch (command_entry)
  {
    // Set creation and management commands.
    case CommandEntry::set_create :
      if (parsed_line.size() == 1) {
        SizeType free_handle = 0;
        while (set_map.contains(free_handle)) {
          ++free_handle;
        }

        set_map.emplace(free_handle, rb::set<DataType>{});
        std::cout << std::format("Handle not specified. "
          "Assigned handle {} for the created set.\n",
          free_handle);

      } else if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          set_map.emplace(operands[0], rb::set<DataType>{});
          std::cout << std::format(
            "Created a set with handle {}.\n", operands[0]);
        } else {
          std::cout << std::format("Set {} already exists.\n",
            operands[0]);
        }
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_copy :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        SizeType free_handle = 0;
        while (set_map.contains(free_handle)) {
          ++free_handle;
        }

        set_map.emplace(free_handle,
          set_map.find(operands[0])->second);
        std::cout << std::format("Handle not specified. "
          "Copy constructed set {} from set {}.\n",
          free_handle, operands[0]);
        
      } else if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        if (set_map.contains(operands[1])) {
          auto assign_to = set_map.find(operands[1]);
          auto assgin_from = set_map.find(operands[0]);
          assign_to->second = assgin_from->second;

          std::cout << std::format(
            "Set {} found. Copy assigned set {} to set {}.\n",
            operands[1], operands[0], operands[1]);
          
        } else {
          set_map.emplace(operands[1],
            set_map.find(operands[0])->second);

          std::cout << std::format(
            "Copy constructed set {} from set {}.\n",
            operands[1], operands[0]);
        }

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_copy_assign :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }
        if (!set_map.contains(operands[1])) {
          std::cout << std::format("Set {} not found.\n",
            operands[1]);
          break;
        }
        auto assign_to = set_map.find(operands[1]);
        auto assgin_from = set_map.find(operands[0]);
        assign_to->second = assgin_from->second;

        std::cout << std::format(
          "Copy assigned set {} to set {}.\n",
          operands[0], operands[1]);
        
      } else {
        std::cout << "Illegal parameters.\n"; 
      }
      break;


    case CommandEntry::set_move :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format(
            "Set {} not found.\n", operands[0]);
          break;
        }

        set_map.erase(operands[1]);
        auto temp = std::move(set_map.find(operands[0])->second);
        set_map.erase(operands[0]);

        set_map.emplace(operands[1], std::move(temp));
        std::cout << std::format("Moved set {} to set {}.\n",
          operands[0], operands[1]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;
      

    case CommandEntry::set_move_assign :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format(
            "Set {} not found.\n", operands[0]);
          break;
        }

        set_map.erase(operands[1]);
        auto temp = std::move(set_map.find(operands[0])->second);
        set_map.erase(operands[0]);

        set_map.emplace(operands[1], std::move(temp));
        std::cout << std::format(
          "Move assigned set {} to set {}.\n",
          operands[0], operands[1]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_swap :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }
        if (!set_map.contains(operands[1])) {
          std::cout << std::format("Set {} not found.\n",
            operands[1]);
          break;
        }

        std::swap(set_map.find(operands[0])->second,
          set_map.find(operands[1])->second);
        std::cout << std::format(
          "Swapped set {} with set {}.\n",
          operands[0], operands[1]);
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_equal :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }
        if (!set_map.contains(operands[1])) {
          std::cout << std::format("Set {} not found.\n",
            operands[1]);
          break;
        }

        if (set_map.find(operands[0])->second ==
          set_map.find(operands[1])->second)
        {
          std::cout << std::format(
            "Set {} equals set {}.\n",
            operands[0], operands[1]);
        } else {
          std::cout << std::format(
            "Set {} does not equal set {}.\n",
            operands[0], operands[1]);
        }

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_destroy :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        set_map.erase(operands[0]);
        std::cout << std::format("Removed set {}.\n",
          operands[0]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_destroy_all :
      if (parsed_line.size() == 1) {
        set_map.clear();
        std::cout << "All sets destroyed.\n";

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;

    // Basic data operation commands.
    case CommandEntry::set_insert :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        auto insert_pos = set_map.find(operands[0]);
        insert_pos->second.insert(operands[1]);

        std::cout << std::format(
          "Inserted {} to set {}.\n", operands[1], operands[0]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;

      
    case CommandEntry::set_erase :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        auto erase_pos = set_map.find(operands[0]);
        if (!erase_pos->second.contains(operands[1])) {
          std::cout << std::format(
            "Element {} not found in set {}.\n",
            operands[1], operands[0]);
          break;
        }
        erase_pos->second.erase(operands[1]);
        std::cout << std::format(
          "Element {} removed from set {}.\n",
          operands[1], operands[0]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_modify :
      if (parsed_line.size() == 4) {
        auto operands = extract_operands(parsed_line, 3);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }
        
        auto modify_pos = set_map.find(operands[0]);
        if (!modify_pos->second.contains(operands[1])) {
          std::cout << std::format(
            "Element {} not found in set {}.\n",
            operands[1], operands[0]);
          break;
        }

        modify_pos->second.erase(operands[1]);
        modify_pos->second.insert(operands[2]);
        std::cout << std::format(
          "Element {} modified to {}.\n",
          operands[1], operands[2]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_contains :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        auto contains_pos = set_map.find(operands[0]);
        if (contains_pos->second.contains(operands[1])) {
          std::cout << std::format(
            "Set {} contains element {}.\n",
            operands[0], operands[1]);
        } else {
          std::cout << std::format(
            "Set {} does not contain element {}.\n",
            operands[0], operands[1]);
        }
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_print :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        std::cout << std::format("\nSet {}\n"
          "================================================\n",
          operands[0]);
        auto print_pos = set_map.find(operands[0])->second;
        for (const auto& i : print_pos) {
          std::cout << i << '\n';
        }
        std::cout << '\n';
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;
      

    case CommandEntry::set_size :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }
      
        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        std::cout << std::format(
          "Size of set {} is {}.\n",
          operands[0], set_map.find(operands[0])->second.size());
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;

    case CommandEntry::set_is_empty :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }
      
        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        if (set_map.find(operands[0])->second.empty()) {
          std::cout << std::format(
            "Set {} is empty.\n", operands[0]);
        } else {
          std::cout << std::format(
            "Set {} is not empty.\n", operands[0]);
        }
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::set_clear :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }
      
        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        set_map.find(operands[0])->second.clear();
        std::cout << std::format(
          "Set {} is cleared.\n", operands[0]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    // Iterator related commands.
    case CommandEntry::iter_get_begin :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        if (iter_map.contains(operands[1])) {
          auto& iter = iter_map.find(operands[1])->second;
          iter = set_map.find(operands[0])->second.begin();
          std::cout << std::format(
            "Iterator {} already exists and was relocated to "
            "the beginning of set {}.\n",
            operands[1], operands[0]);
          break;
        }

        iter_map.emplace(operands[1],
          set_map.find(operands[0])->second.begin());
        std::cout << std::format(
          "Iterator {} created and set to the beginning of "
          "set {}.\n",
          operands[1], operands[0]);
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_get_end :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }

        if (iter_map.contains(operands[1])) {
          auto& iter = iter_map.find(operands[1])->second;
          iter = set_map.find(operands[0])->second.end();
          std::cout << std::format(
            "Iterator {} already exists and was relocated to "
            "the end of set {}.\n",
            operands[1], operands[0]);
          break;
        }

        iter_map.emplace(operands[1],
          set_map.find(operands[0])->second.end());
        std::cout << std::format(
          "Iterator {} created and set to the end of set {}.\n",
          operands[1], operands[0]);
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_increase :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        auto& iter = iter_map.find(operands[0])->second;
        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end position.\n", operands[0]);
        } else {
          ++iter;
          std::cout << std::format(
            "Iterator {} increased by one step.\n", operands[0]);
        }

      } else if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }
        
        auto& iter = iter_map.find(operands[0])->second;
        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end position.\n", operands[0]);
        } else {
          for (int i = 0; i < operands[1]; ++i) {
            ++iter;
          }
          std::cout << std::format(
            "Iterator {} increased by {} steps.\n",
            operands[0], operands[1]);
        }
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;
      

    case CommandEntry::iter_decrease :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        auto& iter = iter_map.find(operands[0])->second;
        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end position.\n", operands[0]);
        } else {
          --iter;
          std::cout << std::format(
            "Iterator {} decreased by one step.\n", operands[0]);
        }

      } else if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        auto& iter = iter_map.find(operands[0])->second;
        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end position.\n", operands[0]);
        } else {
          for (int i = 0 ; i < operands[1]; ++i) {
            --iter;
          }
          std::cout << std::format(
            "Iterator {} decreased by {} steps.\n",
            operands[0], operands[1]);
        }
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_is_end :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        if (iter_map.find(operands[0])->second.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end position.\n", operands[0]);
        } else {
          std::cout << std::format(
            "Iterator {} is not at end postion.\n", operands[0]);
        }
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_erase :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        if (!set_map.contains(operands[1])) {
          std::cout << std::format("Set {} not found.\n",
            operands[1]);
          break;
        }

        auto& iter = iter_map.find(operands[0])->second;
        auto& set = set_map.find(operands[1])->second;

        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end postion.\n", operands[0]);
          break;
        }

        auto origin_iter = iter++;
        set.erase(origin_iter->value);

        std::cout << std::format(
          "Element pointed to by iterator {} was erased. "
          "Iterator {} relocated to the next position in "
          "set {}.\n", operands[0], operands[0], operands[1]);
        
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;

    
    case CommandEntry::iter_discard :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        iter_map.erase(operands[0]);
        std::cout << std::format(
          "Iterator {} removed from iterator list.\n",
          operands[0]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_compare :
      if (parsed_line.size() == 3) {
        auto operands = extract_operands(parsed_line, 2);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }
        if (!iter_map.contains(operands[1])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[1]);
          break;
        }

        if (iter_map.find(operands[0])->second.get_ptr() == 
          iter_map.find(operands[1])->second.get_ptr())
        {
          std::cout << std::format(
            "Iterator {} is at the same position "
            "as iterator {}.\n",
            operands[0], operands[1]);
        } else {
          std::cout << std::format(
            "Iterator {} is at a different position "
            "from iterator {}.\n",
            operands[0], operands[1]);
        }
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_find :
      if (parsed_line.size() == 4) {
        auto operands = extract_operands(parsed_line, 3);
        if (operands.empty()) {
          break;
        }

        if (!set_map.contains(operands[0])) {
          std::cout << std::format("Set {} not found.\n",
            operands[0]);
          break;
        }
        auto& set = set_map.find(operands[0])->second;

        if (!set.contains(operands[2])) {
          std::cout << std::format("Element {} not found.\n",
            operands[2]);
          break;
        }

        if (!iter_map.contains(operands[1])) {
          iter_map.emplace(operands[1], set.begin());
          std::cout << std::format("Iterator {} not found. "
            "Created iterator {}.\n", operands[1], operands[1]);
        }
        auto& iter = iter_map.find(operands[1])->second;

        iter = set.find(operands[2]);
        std::cout << std::format(
          "Element {} found in set {}. Moved iterator {} "
          "to element {}.\n",
          operands[2], operands[0], operands[1], operands[2]);

      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    case CommandEntry::iter_print :
      if (parsed_line.size() == 2) {
        auto operands = extract_operands(parsed_line, 1);
        if (operands.empty()) {
          break;
        }

        if (!iter_map.contains(operands[0])) {
          std::cout << std::format("Iterator {} not found.\n",
            operands[0]);
          break;
        }

        auto& iter = iter_map.find(operands[0])->second;
        if (iter.is_end()) {
          std::cout << std::format(
            "Iterator {} is at end postition.\n", operands[0]);
          break;
        }
        std::cout << std::format(
          "Iterator {} points to element {}.\n",
          operands[0], iter->value);
          
      } else {
        std::cout << "Illegal parameters.\n";
      }
      break;


    // Print all sets that currently exists.
    case CommandEntry::set_print_all :
      std::cout << '\n';
      for (const auto& x : set_map) {
        std::cout << std::format("Set {}\n"
          "================================================\n",
          x.first);
        for (const auto& i : x.second) {
          std::cout << i << '\n';
        }
        std::cout << '\n';
      }
      break;
  }
}