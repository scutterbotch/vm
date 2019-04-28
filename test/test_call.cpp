#include "vm.hpp"
#include <stdio.h>
#include <string.h>

#define CHECK(data, val, accum) \
    if((data) != (val)) {\
        printf("[%s: %s: %d]: " #data " != " #val " (%d)\n",\
                __FILE__, __FUNCTION__, __LINE__, data);\
        accum += 1;\
    }

int callit() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 6,
            0, 0, 0, 0,
            // defines a data segment
            //program
            //
            // push args onto stack
            VM::PUSHI, 'i',
            VM::PUSHI, 'h',
            VM::CALL, 19, 
            VM::SAVED, 4,
            VM::PUSHI, '\n',
            VM::SAVED, 5,
            VM::HALT,
            
            // subroutine
            VM::SAVED, 2,
            VM::SAVED, 3,
            VM::PUSHI, 'x',
            VM::RET
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._sp, 0, e);
    CHECK(vm._data[2], 'h', e);
    CHECK(vm._data[3], 'i', e);
    CHECK(vm._data[4], 'x', e);
    CHECK(vm._data[5], '\n', e);
    CHECK(vm._pc, 19, e)
    return e;
}

int loopit(){
    VM vm;
    const val_type program[] = {
            VM::PCI, 6,
            0, 0, 0, 0,
            // defines a data segment
            //program
            //
            // push args onto stack
            VM::PUSHI, 10,
            VM::SAVED, 2,   // save global of value 10
            
            // begining of loop
            VM::PUSHI, 1,
            VM::PUSHD, 2,
            VM::SUB,        // Subtract 1 from global
            VM::SAVED, 2,   // and save it back.
            VM::PUSHD, 3,   // Add one to the global that
            VM::PUSHI, 1,   // counts how many iterations.
            VM::ADD,
            VM::SAVED, 3,   // save the counter
            VM::PUSHD, 2,   // get the thing we're counting down from 
            VM::NOT,        // negate, since we have branch if zero
            VM::PCZI, 10,   // loop back if not done

            VM::HALT,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, 30, e);
    CHECK(vm._data[2], 0, e);
    CHECK(vm._data[3], 10, e);
    return e;
}
int main() {
    callit();
    loopit();
}

