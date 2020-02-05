# Architecture

Trireme uses a load/store RISC-style architecture, for ease of implementation.

## Basic specifications

Six trits make a hexad, the equivalent of a byte; these hold 729 possible values, in the interval $[-364,364]$. This is the smallest addressable unit of memory.
Three hexads (18 trits) make a machine word, which is the basic unit of code for Trireme. All instructions are word-aligned, and all registers are able to hold one word. In addition, Trireme is little-endian, so the three hexads of a word are stored in memory in the order low, middle, high.

## Registers

Trireme has 27 user-accessible registers, named by using the base-27 representation shown in [the introduction](introduction.md). All registers have a width of 18 trits, equal to one machine word or three hexads. These registers can be divided into three categories: accumulator, general, and special. Their uses are as follows:

* r0: accumulator register, can be implied in certain operations
* rA-rM: general-purpose registers, can be used for anything
* rn-rz: special-purpose registers, mostly reserved for system functions, addressing modes, etc.
	n. Index: holds an index pointer for indexed load/store operations
	o. Overflow: used to hold the high word of multiplication or division
	p. Pointer: holds a data pointer that is the base for load/store operations
	q. (this register currently has no special use)
	r. Return: holds the current system call return address
	s. Stack: holds the address of the stack pointer
	t. (this register currently has no special use)
	u. Modulus: holds the remainder after a division operation
	v. Vector: address of the system call table
	w. (this register currently has no special use)
	x. Extension: this register is reserved for future expansion
	y. (this register currently has no special use)
	z. Zero: always holds a value of %000000

In addition, the Trireme CPU contains 9 system registers. These require special instructions to access, and they can be locked to prevent user code from overwriting their values. As with the user register set, the system set can be divided into three categories.

The instruction pointer register, IP, is normally inaccessible to code; it contains the address of the instruction currently executing, and it can only be modified by branch instructions.

The four debug registers, DR1-4 are only used if the Trap flag (see below) is set to a value of +1. If the flag is set appropriately, then the CPU will issue a breakpoint exception when the instruction pointer's value equals that of any debug register.

Last are the control registers, CR1-4. Each of these has a different function, with CR4 currently reserved for expansion. CR1 is the flags register, a set of trits that indicate the result of certain operations or control their method of execution.

* CR1 flags register trits (all those not shown are undefined, and should be considered reserver):
	\0. Carry (C) flag: holds carry result of previous arithmetic instruction
	\1. Sign (S) flag: holds sign of last result or last used operand
	\2. Direction (D) flag: determines auto-increment or decrement after indirect addressing load/store
	\5. Absolute (A) flag: determines whether the pointer register `rp` is used as a base for load/store operations
	\6. Binary (B) flag: determines whether I/O operations are preceded by a conversion to binary (+) or ternary (-)
	\7. Trap (T) flag: determines whether debug breakpoints are enabled
	\8. Interrupt (I) flag: determines whether interrupts are enabled
	\9. Protect (P) flag: determines whether load/stores to system registers are allowed; (-) allows changing IP

CR2 is the interrupt vector register, containing the base address of a vector table to handle hardware-based interrupts. When an interrupt, including a CPU exception, occurs, execution is diverted to the address located in the word starting at `CR2 + (interrupt# * 3)`. The previous value of the instruction pointer is saved into CR3, the interrupt return address register, where it can be accessed by a handler subroutine.

* The currently defined hardware exceptions are:
	\0. `#D0`: divide by zero exception
	\1. `#BK`: debug breakpoint
	\2. `#PV`: protection violation
	\3. `#IF`: invalid flag value
	\4. `#OP`: undefined opcode

## Memory

Trireme's CPU can access a 12-trit address space: 000000 000000, or %0000. (A more advanced version can extend this to 18, but this is a future development.) This address space is 531,441 hexads, which is roughly equivalent to 512K. Code and data both live in this space, and they are not distinguished by the processor itself.

A special 9-trit I/O address space comprises 19,673 addresses, although only 9,842 of these (%000-%MMM) are normally used. These memory locations are accessed using the `inb`, `int`, `oub`, and `out` instructions, as described below.

## Instruction set

Trireme uses a load-store architecture, with a number of indirect addressing modes. Arithmetic operations can only be performed on data in registers, meaning that any code intending to alter values in memory must first load them into registers first, then store the result.

All instructions take up a single machine word, or 3 hexads. They are word-aligned, as well, with the low, middle, and high hexads laid out in a balanced fashion, having their addresses lowest trits as negative, zero, and positive. Memory location 0, written %0000, is thus actually in the middle of a word on the Trireme CPU; its word starts at %000n, or -1.

As a whole, the instruction set contains a total of 95 basic instructions at present, with variations and synonyms creating a total of 172 mnemonics. All of these are 3 letters long, although the simulator's assembler recognizes an additional 9 two-letter pseudo-instructions. They can be divided into eight broad categories, each of which will be described below.

### Arithmetic

All arithmetic operations set the Carry and Sign flags based on the result of the operation.

#### ADD

`add` adds the values in two source registers, storing the result in a destination register.

* Format: add rA, rB, rC
* Effect: rC := rA + rB

* Format: add rA, rB
* Effect: rB := rA + rB

* Format: add rA
* Effect: rA := rA + rA

The `add rA` form is an alternative way of multiplying a register by 2.

#### ADC

`adc` adds the values in two source registers, then adds the carry flag value to the result, with the sum stored in the destination register.

* Format: adc rA, rB, rC
* Effect: rC := rA + rB + Carry flag

* Format: adc rA, rB
* Effect: rB := rA + rB + Carry flag

* Format: adc rA
* Effect: rA := rA + rA + Carry flag

#### ADI

`adi` adds a hexad immediate value to a source register, placing the result in a destination register.

* Format: adi rA, rB, 123
* Effect: rB := rA + 123

* Format: adi rA, 123
* Effect: rA := rA + 123

#### PAD

`pad` adds a 9-trit immediate value to the value in a source register, then places the result in that same register.

* Format: pad rA, 1234
* Effect: rA := rA + 1234

If the immediate value fits in 6 trits, then `adi` is preferred to `pad` for the sake of clarity.

#### SUB

`sub` subtracts the second source register from the first, moving the result into the destination register.

* Format: sub rA, rB, rC
* Effect: rC := rA - rB

* Format: sub rA, rB
* Effect: rB := rA - rB

* Format: sub rA
* Effect: rA := rA - rA

The `sub rA` form serves the same function as `srz` below.

#### SBC

`sbc` subtracts the second source register from the first, then subtracts the Carry flag's value from the result. The final difference is placed in the destination register.

* Format: sbc rA, rB, rC
* Effect: rC := rA - rB - Carry flag

* Format: sbc rA, rB
* Effect: rB := rA - rB - Carry flag

* Format: sbc rA
* Effect: rA := rA - rA - Carry flag

#### SBI

`sbi` subtracts an immediate hexad value from the source register, placing the difference in the destination register.

* Format: sbi rA, rB, 123
* Effect: rB := rA - 123

* Format: sbi rA, 123
* Effect: rA := rA - 123

Note that `sbi` is equivalent to `adi` with the immediate value's sign changed.

#### PSB

`psb` subtracts a 9-trit immediate value from the value in a source register, then places the result in that same register.

* Format: psb rA, 1234
* Effect: rA := rA - 1234

If the immediate value fits in 6 trits, then `sbi` is preferred to `psb` for the sake of clarity.

#### MUL

`mul` multiplies the values in two source registers, placing the lower word of the result in the destination register, and the higher word in the overflow register `ro`.

* Format: mul rA, rB, rC
* Effect: (ro:rC) := rA * rB

* Format: mul rA, rB
* Effect: (ro:rB) := rA * rB

* Format: mul rA
* Effect: (ro:rA) := rA

The `mul rA` form is a simple method of computing the square of a value, but care must be taken to account for the overflow in `ro`.

#### MLI

`mli` multiplies a source register value by an immediate hexad value, placing the lower word of the result in the destination register, and the higher word in the overflow register `ro`.

* Format: mli rA, rB, 123
* Effect: (ro:rB) := rA * 123

* Format: mli rA, 123
* Effect: (ro:rA) := rA * 123

#### PMU

`pmu` multiplies a source register by a 9-trit immediate value, with the product's lower word being placed in the same register. The higher word, as usual, is moved into the overflow register `ro`.

* Format: pmu rA, 1234
* Effect: (ro:rA) := rA * 1234

#### DIV

`div` divides a two-word value (high word in the overflow register `ro`, low word in the given source register) by the value in the second source register, placing the quotient in the destination register, the remainder in the modulus register `ru`. If the quotient is too large to fit in a single word, the Carry flag is set to its sign.

* Format: div rA, rB, rC
* Effect: rC := (ro:rA) / rB; ru := (ro:rA) % rB

* Format: div rA, rB
* Effect: rB := (ro:rA) / rB; ru := (ro:rA) % rB

* Format: div rA
* Effect: rA := (ro:rA) / rA; ru := (ro:rA) % rA

#### DVI

`dvi` divides a two-word value (high word in the overflow register `ro`, low word in the given source register) by an immediate hexad value, placing the quotient in the destination register, the remainder in the modulus register `ru`. If the quotient is too large to fit in a single word, the Carry flag is set to its sign.

* Format: dvi rA, rB, 123
* Effect: rB := (ro:rA) / 123; ru := (ro:rA) % 123

* Format: dvi rA, 123
* Effect: rA := (ro:rA) / 123; ru := (ro:rA) % 123

#### PDV

`pdv` divides a two-word value (high word in the overflow register `ro`, low word in the given source register) by a 9-trit immediate value, placing the quotient in the register where the low word was, the remainder in the modulus register `ru`. If the quotient is too large to fit in a single word, the Carry flag is set to its sign.

* Format: pdv rA, 1234
* Effect: rA := (ro:rA) / 1234; ru := (ro:rA) % 1234

`dvi` is preferred to `pdv` when the immediate value fits in a single hexad.

#### Synonyms

The following mnemonics are accepted synonyms for arithmetic instructions.

* `inc rA`: synonym for `pad rA, 1`
* `dec rA`: synonym for `psb rA, 1`
* `neg rA`: synoynm for `sti rA`

### Logical

Logical operations include comparison and tritwise calculation. While balanced ternary offers far more Boolean-like functions than binary, implementing all of them in a processor architecture is infeasible, so Trireme contains only those that have appeared in literature on the subject.

All of these operations are register-only, the same as the arithmetic set.

#### CMP

`cmp` sets the Carry and Sign flags based on the result of subtracting the value in the second source register from that contained in the first.

* Format: cmp rA, rB
* Effect: Carry and Sign flags set based on (rA - rB)

An effective equivalent to `cmp rA, rB` is `sub rA, rB, rz`, using the zero register as a destination.

#### CMI

`cmi` sets the Carry and Sign flags as if the given immediate hexad value were subtracted from the source register.

* Format: cmp rA, 123
* Effect: Carry and Sign flags set based on (rA - 123)

As above, `cmi rA, 123` is effectively the same operation as `sub rA, rz, 123`.

#### SHL

`shl` shifts the value in the source register to the left by the given number of trits. The last trit shifted out of the register is saved in the Carry flag.

* Format: shl rA, 3
* Effect: rA := rA &lt;&lt; 3

Shifting left in balanced ternary is equivalent to multiplying by 3.

#### SHR

`shr` shifts the value in the source register to the right by the given number of trits, saving the last trit shifted in the Carry flag.

* Format: shr rA, 3
* Effect: rA := rA >> 3

Shifting right in balanced ternary is the same as dividing by 3, with the result rounded to the nearest integer.

#### ROL

`rol` rotates the value in the source register to the left by the given number of trits.

* Format: rol rA, 3
* Effect: rotate_left(rA, 3)

#### ROR

`ror` rotates the value in the source register to the right by the given number of trits.

* Format: ror rA, 3
* Effect: rotate_right(rA, 3)

#### RCL

`rcl` constructs a 19-trit value consisting of the source register's contents followed by the Carry flag, then rotates this value to the left by the given number of trits.

* Format: rcl rA, 3
* Effect: rotate_carry_left(rA, 3)

#### RCR

`rcr` constructs a 19-trit value consisting of the source register's contents followed by the Carry flag, then rotates this value to the right by the given number of trits.

* Format: rcr rA, 3
* Effect: rotate_carry_right(rA, 3)

#### STI

The `sti` instruction inverts a register using simple ternary inversion.

* Format: sti rA
* Effect: T := -T for each trit T in rA

#### PTI

The `pti` instruction inverts a register using positive ternary inversion, where a zero trit is changed to +1.

* Format: pti rA
* Effect: Do the following for each trit T in rA
	* T := +1 if T &#x2208; {-1, 0}
	* T := -1 if T = +1

#### NTI

The `nti` instruction inverts a register using negative ternary inversion, where a zero trit is changed to -1.

* Format: pti rA
* Effect: Do the following for each trit T in rA
	* T := -1 if T &#x2208; {0, +1}
	* T := +1 if T = -1

#### MIN

`min` computes the tritwise minimum of two source registers, placing the result in a destination register.

* Format: min rA, rB, rC
* Effect: rC(i) := min( rA(i), rB(i) ) for i in 0..17

`min` is one possible balanced ternary analogue to the Boolean AND function.

#### MAX

`max` computes the tritwise maximum of two source registers, placing the result in a destination register.

* Format: max rA, rB, rC
* Effect: rC(i) := max( rA(i), rB(i) ) for i in 0..17

`max` is a possible balanced ternary analogue to the Boolean OR function.

#### TEQ

The `teq` instruction compares two source registers tritwise, setting the corresponding trit of a destination register to +1 if they are equal, -1 otherwise.

* Format: teq rA, rB, rC
* Effect: For i in 0..17, do
	* rC(i) := +1 if rA(i) = rB(i)
	* rC(i) := -1 otherwise

The closest binary analogue to `teq` is the Boolean XOR function, but the two are very different.

#### TEM

The `tem` instruction multiplies two source registers tritwise, storing the result in a destination register.

* Format: tem rA, rB, rC
* Effect: rC(i) := rA(i) * rB(i) for i in 0..17

`tem` is another loose analogue to the Boolean XOR function.

#### Synonyms

The following mnemonics are accepted synonums for logical instructions:

* `not rA`: synonym for `sti rA`
* `and rA, rB, rC`: synonym for `min rA, rB, rC`
* `ort rA, rB, rC`: synonym for `max rA, rB, rC`

### Memory

#### LDR

`ldr` loads into the destination register a word (3 hexads, in little-endian order) starting at the source address.

* Format: ldr %ABCD, r0
* Effect: r0 := word at memory(%ABCF:%ABCE:%ABCD)

#### LDL, LDM, LDH

The `ldl`, `ldm`, and `ldh` instructions load a single hexad from memory into the low, middle, or high hexad of the destination register, leaving the other trits unaffected.

* Format: ldl %ABCD, r0
* Effect: r0(low) := hexad at memory(%ABCD)

* Format: ldm %ABCD, r0
* Effect: r0(middle) := hexad at memory(%ABCD)

* Format: ldh %ABCD, r0
* Effect: r0(high) := hexad at memory(%ABCD)

#### LDI

`ldi` loads an immediate hexad value into the destination register, clearing its middle and high hexads.

* Format: ldi %AB, r0
* Effect: r0 := %AB

#### LIL, LIM, LIH

The `lil`, `lim`, and `lih` instructions load an immediate hexad value into the low, middle, or high hexad of the destination register, while leaving the other trits unaffected.

* Format: lil %AB, r0
* Effect: r0(low) := %AB

* Format: lim %AB, r0
* Effect: r0(middle) := %AB

* Format: lih %AB, r0
* Effect: r0(high) := %AB

#### LDA

`lda` loads a destination register with a word in memory. The memory address of the low hexad is the lowest 12 trits of the value in the source register.

* Format: lda rA, rB
* Effect: rB := word starting at memory(rA)

#### LAL, LAM, LAH

The `lal`, `lam`, and `lah` instructions load a destination register's low, middle, or high hexad with the value at the memory location pointed to by the source register. The other trits in the destination are unaffected.

* Format: lal rA, rB
* Effect: rB(low) := hexad at memory(rA)

* Format: lam rA, rB
* Effect: rB(middle) := hexad at memory(rA)

* Format: lah rA, rB
* Effect: rB(high) := hexad at memory(rA)

#### LDX

The `ldx` instruction is reserved for a future version of the Trireme architecture, to load from a memory address extending beyong the normal 12 trits.

#### LDN

#### LAD

The `lad` loads an address into the r0 register.

* Format: lad %ABCD
* Effect: r0 := %ABCD

This instruction can be used as a 12-trit immediate load, potentially saving an instruction over using the `lil`/`lim` pair. (The simulator's builtin assembler properly handles labels and symbolic constants, the original purpose of `lad`.)

#### STR

#### STL, STM, STH

#### STA

#### SAL, SAM, SAH

#### STN

#### STX

The `stx` instruction is reserved for a future version of the Trireme architecture, to store data in memory addresses beyond the normal 12-trit address space.

#### MOV

#### XCH

#### STZ, STP, STN

#### PSH

#### POP

#### Synonyms

The `clr` mnemonic is a synonym for `stz`.

### Branch

#### BRA

#### BRR

#### BRI

#### BRS

#### BRL

#### BZc, BPc, BNc

#### SZc, SPc, SNc

#### TBc

#### CAL

#### CAR

#### CAA

#### RET

#### ZFc, PFc, NFc

### Conversion

#### BIN

#### TRI

#### FDR

#### RDR

### I/O

#### INT

#### INB

#### OUT

#### OUB

### System

#### SYS

#### SRT

#### BRK

The `brk` instruction triggers a debug breakpoint exception.

* Format: brk
* Effect: Raise the `#BK` hardware exception

#### NOP

The `nop` instruction does nothing. It can be used for padding or to indicate uninitialized code space.

* Format: nop
* Effect: none

#### LSR

#### SSR

#### UND

The `und` instruction maps to a specific opcode that is guaranteed to remain undefined in all future versions of the Trireme architecture. It will thus always trigger a hardware exception.

* Format: und
* Effect: Raise the `#OP` hardware exception

### Assembler pseudo-instructions

# Miscellaneous notes

## Memory map

Though the CPU currently makes no distinction, its 12-trit address space is, in fact, divided into separate regions for user and system code and data. All words with negative addresses (excluding the last trit of the address) are considered to lie within the system region, while the rest are for use by user applications.

A unit of 729 hexads can be called a memory *page*. Using this terminology, the lowest 81 pages of system memory (addresses %zzzz-%xMMM) are reserved for a system kernel, analogous to ROM on a real microcontroller. The following 27 pages (%wzzz-%wMMM) are reserved as system work RAM. Next, the 27 pages from %vzzz-%vMMM are intended for a future text-mode video display. All other system memory locations are considered open, though the "top" half page (%00zz-%000o) holds the CPU stack by default.

At boot, execution begins at location %zzzz, while the stack pointer is set to %000q.

## System RAM

The system RAM area holds two main defined areas. First, the system call table is located in the 3 pages at %wzzz-wxMM. This vector table contains a series of pointers used by the `sys` instruction.

Following the system call table is the hardware vector table. Only a single page in length (%wwzz-%wwMM), this table contains pointers to the interrupt routines called when a hardware exception occurs.

Both of these tables can be moved to any location in memory. The system call table's base address is located in the `rv` special register, while the system register CR2 holds the base address of the hardware vector table. Both of these can be protected from rogue code by setting appropriate system flags.
