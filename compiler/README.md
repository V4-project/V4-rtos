# V4 Compiler (V4-front)

**Status**: Optional Component

Forth-to-bytecode compiler for V4 RTOS, translating Forth source code into executable V4 VM bytecode.

**Note**: This component is optional. You can use V4 RTOS with pre-compiled bytecode or by calling C APIs directly from your application.

## Features

- Complete Forth lexer and parser
- V4 bytecode generation
- Error reporting with line numbers
- Symbol table management
- Compile-time constant folding (planned)
- Peephole optimization (planned)

## Structure

```
compiler/
├── include/
│   ├── lexer.h          # Tokenization
│   ├── parser.h         # Syntax analysis
│   ├── compiler.h       # Code generation
│   └── symbols.h        # Symbol table
├── src/
│   ├── lexer.c
│   ├── parser.c
│   ├── compiler.c
│   └── symbols.c
└── tests/
    └── test_compiler.c
```

## Building

```bash
cmake -B build -DV4_BUILD_KERNEL=ON -DV4_BUILD_COMPILER=ON
cmake --build build
```

## Compilation Flow

```
Forth Source → Lexer → Parser → Compiler → V4 Bytecode
                tokens    AST      codegen    binary
```

## Example

```forth
\ Input: Forth source
: DOUBLE  ( n -- n*2 )
    2 * ;

: QUAD  ( n -- n*4 )
    DOUBLE DOUBLE ;
```

```c
// Compile to bytecode
uint8_t bytecode[256];
size_t len = v4_compile(source, bytecode, sizeof(bytecode));

// Execute in VM
v4_execute(bytecode, len);
```

## API

```c
// Compile Forth source to bytecode
int v4_compile(
    const char* source,
    uint8_t* bytecode,
    size_t bytecode_size
);

// Compile and load into VM
int v4_compile_and_load(
    v4_vm_t* vm,
    const char* source
);
```

## Error Handling

```c
typedef struct {
    int line;
    int column;
    char message[128];
} v4_compile_error_t;

v4_compile_error_t error;
if (v4_compile_ex(source, bytecode, size, &error) < 0) {
    fprintf(stderr, "Error at %d:%d: %s\n",
        error.line, error.column, error.message);
}
```

## Supported Forth Words

- Arithmetic: `+`, `-`, `*`, `/`, `MOD`
- Stack: `DUP`, `DROP`, `SWAP`, `OVER`, `ROT`
- Logic: `AND`, `OR`, `XOR`, `NOT`
- Comparison: `<`, `>`, `=`, `<=`, `>=`
- Control: `IF...THEN`, `IF...ELSE...THEN`, `BEGIN...UNTIL`, `BEGIN...AGAIN`
- Memory: `@`, `!`, `C@`, `C!`
- Definitions: `:`, `;`, `CONSTANT`, `VARIABLE`

## Memory Footprint

- Flash: ~35 KB
- RAM: ~4 KB (compilation buffers)

## Future Enhancements

- Constant folding optimization
- Dead code elimination
- Inline expansion
- Cross-compilation support

## License

Dual licensed under MIT or Apache-2.0.
