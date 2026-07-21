### Folder Design of lanmo

/include
* vm.h
  * VM struct
    * frame
    * program - from program.h
    * memory
  * frame struct
  * push_frame
  * pop_frame
  * get_stack_trace
* stack.h
  * stack - definition
  * stack_push
  * stack_pop
  * stack_peek
  * stack_gc
* program.h
  * symbol_table
  * main_function_pointer
* value.h
  * Value definition
  * DataTypes enum
  * make_int
  * make_str
  * make_float
  * make_function
  * make_list
  * make_obj
  * ...all the function required for create a Value
* native_function.h 
  * print
  * input
  * type
  * time
  * ...all the similar native functions
* memory.h 
  * create_slots
  * write
  * read
  * delete_slot
  * delete_slots

/src
  * parser.h/parser.c
    * parse - parses and returns a program 
    * get_main_function - return int
  * vm.h/vm.c
    * init_vm - init the symbolTable and inits memory stack etc.,
    * load_main
    * start
lm.c
    * print_usage
    * run_program - call parser.parse() and vm.start()
    * main

```
BY AI
include/
    vm.h
    value.h
    stack.h
    frame.h
    parser.h
    native.h
    program.h

src/
    vm.c
    parser.c
    stack.c
    native.c
    value.c
    memory.c

main.c
```