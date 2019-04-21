#include "vm.hpp"
#include <stdio.h>
#include <string.h>

int main() {
    VM vm;
    const val_type program[] = {
            VM::PCI, 4,
            // defines a data segment
            '7', '\n',
            //program
            VM::PUSHI, 'c', 
            VM::PUTC, 
            VM::PUSHI, 'd',
            VM::PUSHI, 1,
            VM::ADD,
            VM::PUTC,
            VM::PUSHI, '\n',
            VM::PUTC,
            VM::PUSHD, 2,
            VM::PUTC,
            VM::PUSHD, 3,
            VM::PUTC,
            0,
    };
    
    ::memcpy(vm._data, program, sizeof(program));
    
    vm.run();

    exit(0);

    vm._push(2);
    vm._push(1);
    vm.less();
    vm._push('0');
    vm.add();
    vm.putc();
    vm._push('\n');
    vm.putc();
}
