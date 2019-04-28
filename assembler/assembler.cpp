#include "vm.hpp"
#include <stdio.h>
#include <string.h>

#define CHECK(data, val, accum) \
    if((data) != (val)) {\
        printf("[%s: %s: %d]: " #data " != " #val " (%d)\n",\
                __FILE__, __FUNCTION__, __LINE__, data);\
        accum += 1;\
    }


/*
 * I23, I'c', c, i
 *
 * is a program that:
 * 0) pushes the number 23 to the stack
 * 1) pushes the letter 'c' (ie 99) to the stack
 * 2) prints 'c'
 * 3) prints 23
 */

int asm_it() {
    VM vm;
    const val_type program[] = {
        VM::PCI, 274, 0, 0, 0, 0, 0, 0, 0, 0,     //10
        // defines a data segment
        '\'', ',', '\n', 0, 0, 0, 0, 0, 0, 0,    //20 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,           //30
        // text code, requires arg, machine_code
        'I', 1,  VM::PUSHI, //b @ 30
        '.', 0,  VM::DUP,   //33
        'c', 0,  VM::PUTC,  //36
        'i', 0,  VM::PUTI,  //39
        'f', 1,  VM::GETF,  //42
        'j', 0,  VM::PC,  //45
        'J', 1,  VM::PCI,  //48
        'z', 1,  VM::PCZI,  //51
        'L', 1,  VM::PUSHD,  //54
        'l', 0,  VM::LOAD,  //57
        's', 1,  VM::SAVE,  //60
        'S', 1,  VM::SAVED,  //63
        '<', 0,  VM::LESS,  //66
        ':', 0,  VM::LE,  //69
        '>', 0,  VM::GT,  //72
        ';', 0,  VM::GE,  //75
        '=', 0,  VM::EQ,  //78
        '~', 0,  VM::NEQ,  //81
        '+', 0,  VM::ADD,  //84
        '-', 0,  VM::SUB,  //87
        '*', 0,  VM::MUL,  //90
        '/', 0,  VM::DIV,  //93
        
        '&', 0,  VM::AND,  //96
        '|', 0,  VM::OR,  //99
        '^', 0,  VM::XOR,  //102
        '!', 0,  VM::NOT,  //105
        
        'n', 0,  VM::NEG,  //108
        
        'C', 0,  VM::CALL,  //111
        'R', 0,  VM::RET,  //114
        '}', 0,  VM::PUSHR, //117
        '{', 0,  VM::POPR, //120
                 0, 0, 0, 0, 0, 0, 0, //end@130
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //140
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //150
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //160
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //170
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //180
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //190
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //200
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //210
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //220
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //230
        '1', '2', '9', '8', ',', 0, 0, 0, 0, 0, //240
        // integer parsing for immediate mode
        // 0) get data ptr on stack
        // 1) push next char
        // 2) if ',' cleanup and return
        // 3) move char to rstack
        // 4) mult old by 10 (top of stack)
        // 5) bring back char to dstack
        // 6) sub '0'
        // 6) add
        // 7) goto 1
        
        // this is cleanup 
        // 240
        VM::POP, // get comma out
        VM::POPR,
        VM::POP,
        VM::RET,
        // actual subroutine
        // 244
        VM::PUSHR, // get data ptr on Rstack
        VM::PUSHI, 0, // initialize
        
        // beginning of loop 
        // 247
        VM::POPR,   // get dataptr onto datastack
        VM::DUP,
        VM::PUSHR,
        VM::LOAD,   // get the value pointed to by stack (what was in rs)
        // now compare to comma
        VM::DUP,
        VM::PUSHI, ',',
        VM::NEQ,
        VM::PCZI, 240,

        // make number
        VM::PUSHI, '0',
        VM::SUB,
        VM::NEG, 
        // move it temporarily to rstack
        VM::PUSHR,
        // mult prev by 10
        VM::PUSHI, 10,
        VM::MUL,
        // bring back new digit and add
        VM::POPR,
        VM::ADD,

        // increment the index into data
        VM::POPR,  // get dataptr onto datastack
        VM::PUSHI, 1,
        VM::ADD,
        VM::PUSHR, // put it back
        VM::PCI, 247,

        // try it out
        VM::PUSHI, 230,  // location of data "12,"

        VM::CALL, 244,
        VM::DUP,
        VM::PUTI,
        VM::HALT,
    };

    ::memcpy(vm._data, program, sizeof(program));

    vm.run();

    int e;
    CHECK(vm._sp, 1, e);
    CHECK(vm._stack[0], 1298, e);
    CHECK(vm._rp, 0, e);
    CHECK(vm._pc, sizeof(program)/sizeof(val_type), e);

    return e;
}


int main() {
    asm_it();
}

