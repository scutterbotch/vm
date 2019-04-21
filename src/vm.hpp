/* 
    VM where every opcode can be changed
    on the fly, whcih means you can change
    the vm
 */
#include <stdio.h>
#include <functional>

#define STACK_SIZE 1<<16
#define DATA_SIZE 1<<16
#define OP_SIZE 1<<5
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
typedef int val_type;

val_type Less(val_type a, val_type b) { return a < b; }
val_type Add(val_type a, val_type b) { return a + b; }


struct VM {
    val_type _pc;
    val_type _sp;
    val_type _fp;

    val_type _stack[STACK_SIZE];
    val_type _data[DATA_SIZE];
    typedef void(VM::*OP)();

    val_type sp_get() { return _sp; }
    void sp_set(val_type v) { _sp = v; }

    val_type fp_get() { return _fp; }
    void fp_set(val_type v) { _fp = v; }
   
    val_type pc_get() { return _pc; }
    void pc_set(val_type v) { _pc = v; }

    val_type stack_get(val_type __sp) { return _stack[__sp]; }
    val_type stack_get() { return stack_get(sp_get()-1); }
    void stack_set(val_type v, val_type __sp) { _stack[__sp] = v; } 

    val_type data_get(val_type dp){ return _data[dp]; }
    void data_set(val_type dp, val_type d) { _data[dp] = d; }

    // facility
    void _push(val_type v) { _stack[_sp++] = v; }
    val_type _pop() { return _stack[--_sp]; }
    val_type _peek() { return _stack[_sp-1]; }
    void pr_stack() { for( int i=0; i < 10; ++i ) printf(">> %d\n", _stack[i]);}

    void run(){
        for( val_type op; ; ) {
            if(!(op = data_get(_pc++)))
                break;
            (((VM*)this) ->* (_ops[op]))();
        }
    }

    // ******
    // ops
    
    // print char
    void putc(){ ::putc(_pop(), ::stdout); }
    void puti(){ ::printf("%d", _pop()); }

    // read from file
    //GETF FILENAME_LOC, DATA_LOC_START
    void getf(){ 
        char fname[32];
        int i = 0;
        int name_loc = _data[_pc++];
        int dest_loc = _data[_pc++];
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
    
    enum {
        HALT,
        PUSHI, 
        POP,
        DUP,

        PC,
        PCI,
        PCZI,
        
        PUSHD,
        LOAD,

        SAVE,
        SAVED,
       
        PUTC,
        PUTI,
        
        GETF,

        LESS,
        LE,
        GT,
        GE,
        EQ,
        NEQ,

        ADD,
        SUB,
        MUL,
        DIV,
  
        AND,
        OR,
        XOR, 

        NOT,
        NEG,
    };
    OP _ops[OP_SIZE];

    VM() : _pc(0), _sp(0), _fp(0) {
        _ops[PUSHI] = &VM::pushi;
        _ops[POP] = &VM::pop;
        _ops[DUP] = &VM::dup;
        
        _ops[PUTC] = &VM::putc;
        _ops[PUTI] = &VM::puti;

        _ops[GETF] = &VM::getf;
        
        _ops[PC] = &VM::pc;
        _ops[PCI] = &VM::pci;
        _ops[PCZI] = &VM::pczi;
        _ops[PUSHD] = &VM::pushd;
        _ops[LOAD] = &VM::load;
        _ops[SAVE] = &VM::save;
        _ops[SAVED] = &VM::saved;
        
        _ops[LESS] = &VM::less;
        _ops[LE] = &VM::le;
        _ops[GT] = &VM::greater;
        _ops[GE] = &VM::ge;
        _ops[EQ] = &VM::eq;
        _ops[NEQ] = &VM::neq;

        _ops[ADD] = &VM::add;
        _ops[SUB] = &VM::sub;
        _ops[MUL] = &VM::mul;
        _ops[DIV] = &VM::div;

        _ops[AND] = &VM::And;
        _ops[OR] = &VM::Or;
        _ops[XOR] = &VM::Xor;
        
        _ops[NOT] = &VM::Not;
        _ops[NEG] = &VM::neg;
    }
};

