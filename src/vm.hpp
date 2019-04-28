/* 
    VM where data and text segements are intermingled
    which allows us to write self-modifying code easily
 */
#pragma once

#include <stdio.h>
#include <functional>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/enum.hpp>


#define STACK_SIZE 64
#define DATA_SIZE (32*1024)
#define OP_SIZE 1<<6
typedef int val_type;

// building blocks
#define UNARY(f, name) \
void name() {\
    val_type a = _pop();\
    _push(f(a));\
}

#define BINARY(f, name)\
void name() {\
    val_type a = _pop();\
    val_type b = _pop();\
    _push(a f b);\
}

#define BINARYF(f, name)\
void name() {\
    val_type a = _pop();\
    val_type b = _pop();\
    _push(f(a,b));\
}

#define OPCODES \
        ((PUSHI)(pushi))\
        ((POP)(pop))\
        ((DUP)(dup))\
\
        ((PUTC)(putc))\
        ((PUTI)(puti))\
\
        ((GETF)(getf))\
\
        ((PC)(pc))\
        ((PCI)(pci))\
        ((PCZI)(pczi))\
        ((PUSHD)(pushd))\
        ((LOAD)(load))\
        ((SAVE)(save))\
        ((SAVED)(saved))\
\
        ((LESS)(less))\
        ((LE)(le))\
        ((GT)(greater))\
        ((GE)(ge))\
        ((EQ)(eq))\
        ((NEQ)(neq))\
\
        ((ADD)(add))\
        ((SUB)(sub))\
        ((MUL)(mul))\
        ((DIV)(div))\
\
        ((AND)(And))\
        ((OR)(Or))\
        ((XOR)(Xor))\
\
        ((NOT)(Not))\
        ((NEG)(neg))\
\
        ((CALL)(call))\
        ((RET)(ret))\
\
        ((PUSHR)(pushr))\
        ((POPR)(popr))


#define _HEAD(a,b,c) BOOST_PP_SEQ_HEAD(c)
#define _MAKE_FN(a, b) &a::b
#define _DEF_FN(a,b,c) _ops[BOOST_PP_SEQ_HEAD(c)] = _MAKE_FN(VM, BOOST_PP_SEQ_ELEM(1, c));

#define _STR(x) #x
#define _STRX(a,b,x) _STR(x\0)
#define VM_TOKENS BOOST_PP_SEQ_TRANSFORM(_HEAD, _, OPCODES)
#define VM_TOKEN_STRING BOOST_PP_SEQ_TRANSFORM(_STRX, _, VM_TOKENS)
#define VM_OP_DEFINE BOOST_PP_SEQ_FOR_EACH(_DEF_FN, _, OPCODES)
#define VM_STRING_LIST BOOST_PP_SEQ_ENUM(VM_TOKEN_STRING)
#define VM_NUM_OPS BOOST_PP_SEQ_SIZE(OPCODES)

const char* k_token_names[] = { "HALT", VM_STRING_LIST };

struct VM {
    val_type _pc; // program ctr
    val_type _sp; // stack ptr
    val_type _rp; // return ptr

    val_type _stack[STACK_SIZE];
    val_type _callstack[STACK_SIZE];
    
    val_type _data[DATA_SIZE];

    typedef void(VM::*OP)();

    val_type sp_get() { return _sp; }
    void sp_set(val_type v) { _sp = v; }

    val_type pc_get() { return _pc; }
    void pc_set(val_type v) { _pc = v; }

    val_type stack_get(val_type __sp) { return _stack[__sp]; }
    val_type stack_get() { return stack_get(sp_get()-1); }
    void stack_set(val_type v, val_type __sp) { _stack[__sp] = v; } 

    val_type data_get(val_type dp){ return _data[dp]; }
    void data_set(val_type dp, val_type d) { _data[dp] = d; }

    // ************
    // facility
    //
    // callstack
    void _pushr(val_type v) { _callstack[_rp++] = v; }
    val_type _popr() { return _callstack[--_rp]; }

    // data stack
    void _push(val_type v) { _stack[_sp++] = v; }
    val_type _pop() { return _stack[--_sp]; }
    val_type _peek() { return _stack[_sp-1]; }

    void _pr_stack(val_type* stack, int sp) { 
        for( int i=0; i < 10; ++i ) {
            if(i==sp-1)
                printf("**");
            printf("%d", stack[i]);
            if(i==sp-1)
                printf("**");
            printf(", ");
        } 
    }

    void pr_stack() { _pr_stack(_stack, _sp); }
    void pr_rstack() { _pr_stack(_callstack, _rp); }
    void dbg_print(val_type c) {
        if(c>'a'&&c<'Z')
            printf("%c", c);
        else
            printf("%d", c);
    }

    // run the VM
    void run(){
        for( val_type op; ; ) {
            op = data_get(_pc++);
            if(op == HALT)
                break;
#ifdef DEBUG
            printf("\n-----------------\n");
            printf("\n");
            printf(">> sp=%d: ", _sp);
            pr_stack();
            printf("\n");
            printf(">> rp=%d: ", _rp);
            pr_rstack();
            printf("\n");
            val_type stack = _stack[_sp];
            val_type next_val = data_get(_pc);
            printf("%d: %s ", _pc-1, token_names[op]);
            dbg_print(next_val);
            ::fflush(stdout);
#endif
            (((VM*)this) ->* (_ops[op]))();
#ifdef DEBUG
            printf("\n");
            printf("<< sp=%d: ", _sp);
            pr_stack();
            printf("\n");
            printf("<< rp=%d: ", _rp);
            pr_rstack();
            printf("\n");
#endif
        }
    }

    // ******
    // ops
    // ******
    
    // print char
    void putc(){ 
        printf(">>> ");
        ::putc(_pop(), ::stdout);
        printf("\n");
    }
    void puti(){ 
        printf(">>> ");
        ::printf("%d\n", _pop());
        printf("\n");
    }

    // read from file
    //GETF FILENAME_LOC, DATA_LOC_START
    void getf(){ 
        char fname[32];
        int i = 0;
        int dest_loc = _pop();
        int name_loc = _data[_pc++];
        while(fname[i] = _data[name_loc+i]){
            //printf("%c:", fname[i]);
            i++;
        };
        //printf("%s", fname);
        FILE* fp = ::fopen(fname, "r");
        char ch;
        while((ch = ::fgetc(fp)) != EOF) {
            _data[dest_loc++] = ch;
        }
        ::fclose(fp);
    }
    // push value at next pc onto stack
    void pushi(){ _push(_data[_pc++]); }
    // load value pointed to by stack
    void load(){ _push(_data[_pop()]); }
    // next pc is a pointer (shortcut for the above)
    void pushd(){ _push(_data[_data[_pc++]]); }
  
    // save next value to location at top of stack
    void save(){ _data[_pop()] = _data[_pc++]; }
    // save top of stack to next pc ptr (ptr)
    void saved(){ _data[_data[_pc++]] = _pop(); }

    void pop(){ _pop(); }
   
    void dup(){ _push(_peek()); }

    // jmp
    void pci(){ pc_set(_data[_pc++]); }
    void pc(){ pc_set(_pop()); }
    void pcd(){ pc_set(_data[_data[_pc++]]); }
   
    // set pc to location if top of stack is zero 
    void pczi(){ if( !_pop() ) pc_set(_data[_pc++]); else _pc++; }

    // subroutines
    // assumes args have been pushed onto the stack before calling this
    void call(){ 
        _pushr(_pc+1); 
        _pc = _data[_pc];
    };
    void ret(){ 
        _pc = _popr();
    }

    // allow us to use the return stack
    void pushr() {
        _pushr(_pop());
    }
    void popr() {
        _push(_popr());
    }

    UNARY(!, Not)
    UNARY(-, neg)
    
    BINARY(<, less)
    BINARY(<=, le)
    BINARY(>, greater)
    BINARY(>=, ge)
    BINARY(==, eq)
    BINARY(!=, neq)

    BINARY(+, add)
    BINARY(-, sub)
    BINARY(*, mul)
    BINARY(/, div)

    BINARY(&, And)
    BINARY(|, Or)
    BINARY(^, Xor)

    enum { HALT, BOOST_PP_SEQ_ENUM(VM_TOKENS) };
    OP _ops[OP_SIZE];

    VM() : _pc(0), _sp(0), _rp(0) {
        VM_OP_DEFINE;
        token_names = k_token_names;
   }; 
   const char** token_names;
};

