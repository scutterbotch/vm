#include "vm.hpp"
#include <stdio.h>
#include <string.h>

int main() {
    VM vm;
    
    const char* str = "Hello World!\0"; 
    FILE* fp = ::fopen("prog.y", "w");
    for(const char* c=str; *c; ++c) {
        putc(VM::PUSHI, fp);
        putc(*c, fp);
        putc(VM::PUTC, fp);
    }
    putc(VM::PUSHI, fp);
    putc('\n', fp);
    putc(VM::PUTC, fp);
   
    putc(VM::HALT, fp);
    ::fclose(fp);

    // trampoline!
    const val_type program[] = {
        VM::PCI, 9,
        'p', 'r', 'o', 'g', '.', 'y', 0,
        VM::GETF, 2, 15,
        VM::PCI, 15,
        VM::HALT
    }; 
    ::memcpy(vm._data, program, sizeof(program));

    vm.run();

    exit(0);
}
