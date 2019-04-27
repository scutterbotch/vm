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
            VM::PCI, 2,
            // defines a data segment
            //program
            //
            // push args onto stack
            VM::PUSHI, '\n',
            VM::PUSHI, 'i',
            VM::PUSHI, 'h',
            VM::CALL, 15, 
            VM::PUTC,
            VM::PUSHI, '\n',
            VM::PUTC,
            VM::HALT,
            
            // subroutine
            VM::PUTC,
            VM::PUTC,
            VM::PUTC,
            VM::PUSHI, 'x',
            VM::RET
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    int e;
//    CHECK(vm._data[4], 'c', e);
//    CHECK(vm._data[5], 'e', e);
//    CHECK(vm._data[6], 5, e);
//    CHECK(vm._data[7], 500, e);
//    CHECK(vm._data[8], 40, e);
//    CHECK(vm._data[9], -40, e);
    CHECK(vm._sp, 0, e);
    CHECK(vm._pc, 15, e)
    return e;
}

int main() {
    for(int i = 0 ; i < VM_NUM_OPS; ++i )
        printf("%02d: %s\n", i, k_token_names[i]);
    printf("....\n");
    return callit();
}

