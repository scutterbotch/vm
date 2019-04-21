#include "vm.hpp"
#include <stdio.h>
#include <string.h>

#define CHECK(data, val, accum) \
    if((data) != (val)) {\
        printf("[%s: %s: %d]: " #data " != " #val " (%d)\n",\
                __FILE__, __FUNCTION__, __LINE__, data);\
        accum += 1;\
    }

int arithmetic() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 10,
            // defines a data segment
            '7', '\n',  // these play no role in the below
            // where our answers go
            0, 0, 0, 0, 0, 0,
            //program
            //
            // PUSH and SAVED
            VM::PUSHI, 'c', 
            VM::SAVED, 4,  // save top of stack to loc 4

            // ADD and SAVED
            VM::PUSHI, 'd',
            VM::PUSHI, 1,
            VM::ADD,
            VM::SAVED, 5,  // save top of stack to loc 5

            // DIV
            VM::PUSHI, 10,
            VM::PUSHI, 50,
            VM::DIV,
            VM::SAVED, 6,

            // MUL
            VM::PUSHI, 10,
            VM::PUSHI, 50,
            VM::MUL,
            VM::SAVED, 7,

            // SUB
            VM::PUSHI, 10,
            VM::PUSHI, 50,
            VM::SUB,
            VM::SAVED, 8,
            
            // SUB
            VM::PUSHI, 50,
            VM::PUSHI, 10,
            VM::SUB,
            VM::SAVED, 9,
            
            VM::HALT,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._data[4], 'c', e);
    CHECK(vm._data[5], 'e', e);
    CHECK(vm._data[6], 5, e);
    CHECK(vm._data[7], 500, e);
    CHECK(vm._data[8], 40, e);
    CHECK(vm._data[9], -40, e);
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, sizeof(program)/sizeof(val_type), e)
    return e;
}

int compare_less() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 10,
            // defines a data segment
            // where our answers go
            0, 0, 0, 0, 0, 0, 0, 0,
            //program
            //
            // SHOULD be FALSE
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::LESS,
            VM::SAVED, 2,  // save top of stack to loc 2

            // SHOULD be true
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::LESS,
            VM::SAVED, 3,  
            
            // SHOULD be FALSE
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::LE,
            VM::SAVED, 4,  // save top of stack to loc 2

            // SHOULD be true
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::LE,
            VM::SAVED, 5,  
          
            // SHOULD be true
            VM::PUSHI, 'c',
            VM::PUSHI, 'c',
            VM::LE,
            VM::SAVED, 6,  
           
            VM::HALT,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._data[2], 0, e);
    CHECK(vm._data[3], 1, e);
    CHECK(vm._data[4], 0, e);
    CHECK(vm._data[5], 1, e);
    CHECK(vm._data[6], 1, e);
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, sizeof(program)/sizeof(val_type), e)
    return e;
}

int compare_greater() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 10,
            // defines a data segment
            // where our answers go
            0, 0, 0, 0, 0, 0, 0, 0,
            //program
            //
            // SHOULD be TRUE
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::GT,
            VM::SAVED, 2,  // save top of stack to loc 2

            // SHOULD be false
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::GT,
            VM::SAVED, 3,  
            
            // SHOULD be TRUE
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::GE,
            VM::SAVED, 4,  // save top of stack to loc 2

            // SHOULD be FALSE
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::GE,
            VM::SAVED, 5,  
          
            // SHOULD be true
            VM::PUSHI, 'c',
            VM::PUSHI, 'c',
            VM::GE,
            VM::SAVED, 6,  
           
            VM::HALT,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._data[2], 1, e);
    CHECK(vm._data[3], 0, e);
    CHECK(vm._data[4], 1, e);
    CHECK(vm._data[5], 0, e);
    CHECK(vm._data[6], 1, e);
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, sizeof(program)/sizeof(val_type), e)
    return e;
}


int compare_eq() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 10,
            // defines a data segment
            // where our answers go
            0, 0, 0, 0, 0, 0, 0, 0,
            //program
            //
            // SHOULD be TRUE
            VM::PUSHI, 'c',
            VM::PUSHI, 'c',
            VM::EQ,
            VM::SAVED, 2,  // save top of stack to loc 2

            // SHOULD be false
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::EQ,
            VM::SAVED, 3,  
            
            // SHOULD be false
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::EQ,
            VM::SAVED, 4,  
            
            // SHOULD be TRUE
            VM::PUSHI, 'c',
            VM::PUSHI, 'd',
            VM::NEQ,
            VM::SAVED, 5,  // save top of stack to loc 2

            // SHOULD be TRUE
            VM::PUSHI, 'd',
            VM::PUSHI, 'c',
            VM::NEQ,
            VM::SAVED, 6,  // save top of stack to loc 2

            // SHOULD be FALSE
            VM::PUSHI, 'c',
            VM::PUSHI, 'c',
            VM::NEQ,
            VM::SAVED, 7,  
          
            VM::HALT,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._data[2], 1, e);
    CHECK(vm._data[3], 0, e);
    CHECK(vm._data[4], 0, e);
    CHECK(vm._data[5], 1, e);
    CHECK(vm._data[6], 1, e);
    CHECK(vm._data[7], 0, e);
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, sizeof(program)/sizeof(val_type), e)
    return e;
}

int main() {
    return arithmetic() 
        && compare_less() 
        && compare_greater()
        && compare_eq();
}

