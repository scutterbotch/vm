#include "vm.hpp"
#include <stdio.h>
#include <string.h>

int main() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 4,
            // defines a data segment
            '1', '\n',
            //program
            VM::PUSHI, 'h', 
            VM::PUTC, 
            VM::PUSHI, 'd',
            VM::PUSHI, 1,
            VM::ADD,
            VM::PUTC,
            VM::PUSHI, '-',
            VM::PUTC,
            VM::PUSHI, '\n',
            VM::PUTC,

            // put data at data location 2 at top of stack and print 
            VM::PUSHI, 2,   // push pointer 2 to stack
            VM::LOAD,       // load the data at the location pointed to by stack
            VM::PUTC,

            // put data at data location 3 at top of stack and print 
            VM::PUSHI, 3,
            VM::LOAD,
            VM::PUTC,

            // add one to data at location 2
            VM::PUSHI, 1,
            VM::PUSHD, 2,   // push onto stack data at loc 2
                            // pushd is equiv to PUSHI, LOAD
            VM::ADD,

            // save what's at the top of the stack to location 2
            VM::SAVED, 2,

            // push what's at location 2 to top of stack
            VM::PUSHD, 2,  // pointer to mem location 2
            
            VM::PUTC,
            VM::PUSHD, 3,
            VM::PUTC,

            // now modify myself
            VM::PUSHI, 1,
            VM::PUSHD, 5,
            VM::SUB,
            VM::DUP,
            VM::SAVED, 5,
            VM::DUP,
            VM::PUTC,
            VM::PUSHI, 'a',
            VM::SUB,
            VM::NEG,
            VM::DUP,
            VM::PUTI,
            VM::NOT,
            VM::DUP,
            VM::PUTI,
            VM::PUSHI, '\n',
            VM::PUSHI, '-',
            VM::PUSHI, '-',
            VM::PUSHI, '\n',
            VM::PUTC, VM::PUTC, VM::PUTC, VM::PUTC,
            VM::PCZI, 4,
            0,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    exit(0);
}
