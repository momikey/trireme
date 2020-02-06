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
	* n. Index: holds an index pointer for indexed load/store operations
	* o. Overflow: used to hold the high word of multiplication or division
	* p. Pointer: holds a data pointer that is the base for load/store operations
	* q. (this register currently has no special use)
	* r. Return: holds the current system call return address
	* s. Stack: holds the address of the stack pointer
	* t. (this register currently has no special use)
	* u. Modulus: holds the remainder after a division operation
	* v. Vector: address of the system call table
	* w. (this register currently has no special use)
	* x. Extension: this register is reserved for future expansion
	* y. (this register currently has no special use)
	* z. Zero: always holds a value of %000000

In addition, the Trireme CPU contains 9 system registers. These require special instructions to access, and they can be locked to prevent user code from overwriting their values. As with the user register set, the system set can be divided into three categories.

The instruction pointer register, IP, is normally inaccessible to code; it contains the address of the instruction currently executing, and it can only be modified by branch instructions.

The four debug registers, DR1-4 are only used if the Trap flag (see below) is set to a value of +1. If the flag is set appropriately, then the CPU will issue a breakpoint exception when the instruction pointer's value equals that of any debug register.

Last are the control registers, CR1-4. Each of these has a different function, with CR4 currently reserved for expansion. CR1 is the flags register, a set of trits that indicate the result of certain operations or control their method of execution.

* CR1 flags register trits (all those not shown are undefined, and should be considered reserver):
	* 0. Carry (C) flag: holds carry result of previous arithmetic instruction
	* 1. Sign (S) flag: holds sign of last result or last used operand
	* 2. Direction (D) flag: determines auto-increment or decrement after indirect addressing load/store
	* 5. Absolute (A) flag: determines whether the pointer register `rp` is used as a base for load/store operations
	* 6. Binary (B) flag: determines whether I/O operations are preceded by a conversion to binary (+) or ternary (-)
	* 7. Trap (T) flag: determines whether debug breakpoints are enabled
	* 8. Interrupt (I) flag: determines whether interrupts are enabled
	* 9. Protect (P) flag: determines whether load/stores to system registers are allowed; (-) allows changing IP

CR2 is the interrupt vector register, containing the base address of a vector table to handle hardware-based interrupts. When an interrupt, including a CPU exception, occurs, execution is diverted to the address located in the word starting at `CR2 + (interrupt# * 3)`. The previous value of the instruction pointer is saved into CR3, the interrupt return address register, where it can be accessed by a handler subroutine.

* The currently defined hardware exceptions are:
	* 0. `#D0`: divide by zero exception
	* 1. `#BK`: debug breakpoint
	* 2. `#PV`: protection violation
	* 3. `#IF`: invalid flag value
	* 4. `#OP`: undefined opcode

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

For all load and store operations, if the Absolute flag is set to +1, the base pointer register `rp` will be added to any computed address.

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

The `ldn` instruction performs an indexed load. The starting memory address of the source word is the sum of the given displacement (1 hexad), the base pointer register `rp`, and the index pointer register `rn`. After the load, the value of the Direction flag is added to the `rn` register.

* Format: ldn %AB, r0
* Effect: r0 := memory_word(rp + rn + %AB); rn := rn + Direction flag

#### LAD

The `lad` loads an address into the r0 register.

* Format: lad %ABCD
* Effect: r0 := %ABCD

This instruction can be used as a 12-trit immediate load, potentially saving an instruction over using the `lil`/`lim` pair. (The simulator's builtin assembler properly handles labels and symbolic constants, the original purpose of `lad`.)

#### STR

`str` stores the word in the source register into memory, starting at the destination address. The store is little-endian, and no alignment checking is done.

* Format: str r0, %ABCD
* Effect: memory(%ABCF:%ABCE:%ABCD) := r0

#### STL, STM, STH

The `stl`, `stm`, and `sth` instructions store the low, middle, or high hexad of the source register into the destination memory address.

* Format: stl r0, %ABCD
* Effect: memory(%ABCD) := r0(low)

* Format: stm r0, %ABCD
* Effect: memory(%ABCD) := r0(middle)

* Format: sth r0, %ABCD
* Effect: memory(%ABCD) := r0(high)

#### STA

`sta` stores the word in the source register into memory starting at the address in the destination register. Only the lowest 12 trits of the destination register are considered; the rest are treated as 0. No alignment checking is done.

* Format: sta rA, rB
* Effect: memory_word(rB) := rA

#### SAL, SAM, SAH

The `sal`, `sam`, and `sah` instructions store the low, middle, or high hexad of the source register into the memory location given by the destination register.

* Format: sal rA, rB
* Effect: memory(rB) := rA(low)

* Format: sam rA, rB
* Effect: memory(rB) := rA(middle)

* Format: sah rA, rB
* Effect: memory(rB) := rA(high)

#### STN

The `stn` instruction performs an indexed store. The word in the source register is stored into memory starting at the address given by the sum of the displacement (1 hexad), the base pointer register `rp`, and the index pointer register `rn`. Then, the value of the Direction flag is added to the `rn` register.

* Format: stn r0, %AB
* Effect: memory_word(rp + rn + %AB) := r0; rn := rn + Direction flag

#### STX

The `stx` instruction is reserved for a future version of the Trireme architecture, to store data in memory addresses beyond the normal 12-trit address space.

#### MOV

`mov` copies the word in the source register into the destination register. The source value is left unchanged.

* Format: mov rA, rB
* Effect: rB := rA

Note that `mov` does not affect any flags. (An alternative that does affect flags is `add rA, rB, rz`.)

#### XCH

`xch` exchanges the values in two registers in a single, atomic operation.

* Format: xch rA, rB
* Effect: rA <-> rB

#### SRZ, SRP, SRN

The `srz`, `srp`, and `srn` instructions set all the trits in the target register to 0, +1, or -1, respectively.

* Format: srz r0
* Effect: r0 := %000000

* Format: srp r0
* Effect: r0 := %MMMMMM

* Format: srn r0
* Effect: r0 := %zzzzzz

#### PSH

`psh` pushes the value in the source register onto the system stack, then decrements the stack pointer register `rs` by 3.

* Format: psh r0
* Effect: memory_word(rs) := r0; rs := rs - 3

#### POP

`pop` pops a word from the system stack into the destination register, then increments the stack pointer register `rs` by 3.

* Format: pop r0
* Effect: r0 := memory_word(rs); rs := rs + 3

#### Synonyms

The `clr` mnemonic is a synonym for `srz`.

### Branch

 In all branch instructions, the lowest trit of the computed address is ignored, as execution in Trireme is word-aligned; the new value of the instruction pointer will have a lowest trit of -1.

#### BRA

`bra` causes execution to branch unconditionally to an absolute address anywhere within the 12-trit memory space.

* Format: bra %ABCD
* Effect: IP := %ABCB

#### BRR

The `brr` instruction creates an unconditional branch to the address contained in the given register.

* Format: bra r0
* Effect: IP := align(r0)

#### BRI

`bri` causes an unconditional branch to an address given by the sum of the register's contents and the immediate hexad displacement.

* Format: bri r0, %AB
* Effect: IP := align(r0 + %AB)

#### BRS, BRL

The `brs` and `brl` instructions cause an unconditional branch relative to the instruction pointer. The given displacement is 1 hexad in the case of `brs`, 2 for `brl`, and it is added to the IP to compute the target address.

* Format: brs %AB
* Effect: IP := IP + %AB

* Format: brl %ABCD
* Effect: IP := IP + %ABCB

#### BZc, BPc, BNc

This class of instructions creates conditional branches, which are taken only if the chosen flag has a value of 0 (for `bzC`), +1 (for `bpC`), or -1 (for `bnC`). All branches are relative to the instruction pointer.

The final letter of the mnemonic indicates the flag that is tested:

|Letter| Flag     |
|------|----------|
| C    | Carry    |
| S    | Sign     |
| D    | Direction|
| A    | Absolute |
| B    | Binary   |
| T    | Trap     |
| I    | Interrupt|
| P    | Protect  |

* Format: bzc %00AB
* Effect: IP := IP + %00AB if Carry flag = 0

* Format: bpa %00AB
* Effect: IP := IP + %00AB if Absolute flag = +1

* Format: bns %00AB
* Effect: IP := IP + %00AB if Sign flag = -1

#### SZc, SPc, SNc

This set of instructions creates a conditional branch that skips over the following instruction only if the chosen flag equals the given value: 0 for `szC`, +1 for `spC`, -1 for `snC`.

The final letter of the mnemonic indicates the flag this is tested, as above.

* Format: szc
* Effect: IP := IP + 3 if Carry flag = 0

* Format: spa
* Effect: IP := IP + 3 if Absolute flag = +1

* Format: sns
* Effect: IP := IP + 3 if Sign flag = -1

#### TBc

The `tbC` family of instructions creates a 3-way indirect branch. The value added to the instruction pointer is taken from the first address register if the chosen flag is -1, the second if it is 0, and the third if it is +1.

The final letter of the mnemonic indicates the flag that is tested, as above.

* Format: tbc rA, rB, rC
* Effect: One of the following
	* IP := align(IP + rA) if Carry flag = -1
	* IP := align(IP + rB) if Carry flag = 0
	* IP := align(IP + rC) if Carry flag = +1

#### CAL

`cal` initiates a subroutine branch to the given address. The current value of the instruction pointer is pushed onto the system stack, as by the `psh` instruction. Then, the given address is loaded into IP, and execution continues from this new location.

* Format: cal %00AB
* Effect: The following, in order
	* memory_word(rs) := IP
	* rs := rs - 3
	* IP := %00AB

#### CAR

`car` initiates a subroutine branch relative to the instruction pointer. The current value of IP is saved on the system stack, as for `cal`.

* Format: car %00AB
* Effect: The following, in order
	* memory_word(rs) := IP
	* rs := rs - 3
	* IP := IP + %00AB

#### CAA

`caa` initiates a subroutine branch to the address stored in the given register. This is an absolute address.

* Format: caa r0
* Effect: The following, in order
	* memory_word(rs) := IP
	* rs := rs - 3
	* IP := align(IP + r0)

#### RET

`ret` returns from a subroutine, an unconditional branch created by popping a word from the stack into the instruction pointer.

* Format: ret
* Effect: The following, in order
	* IP := align(memory_word(rs))
	* rs := rs + 3

#### ZFc, PFc, NFc

This family of instructions sets a flag to a given value: 0 for `zfC`, +1 for `pfC`, -1 for `nfC`.

The final letter of the mnemonic indicates the flag to be changed, using the table above.

### Conversion

#### BIN

The `bin` instruction converts the value in the source register to binary. The destination register will be filled with the lowest 18 bits of the result, as trits that are never set to -1.

* Format: bin rA, rB
* Effect: rB := to_binary(rA)

As an example, the value %000ABC, or decimal 786, will be converted to binary 00_0000_0011_0001_0010. This result is then encoded in nonnegative trits, with the result %00AICC.

#### TRI

`tri` performs a binary-to-ternary conversion on the source register, treating its trits as though they were binary bits instead. The result is placed in the destination register.

* Format: tri rA, rB
* Effect: rB := to_ternary(rA)

As an example, the binary value 00_0000_0000_0101_0101 (decimal 85, hexadecimal 0x00055) is encoded as ternary %000ACJ. The `tri` instruction would convert this to ternary %0000DD, storing this value in the destination register.

#### FDR

`fdr` performs the forward diode operation, changing every -1 trit in the source register to 0, then storing the result in the destination register.

* Format: fdr rA, rB
* Effect: For i in 0..17 do
	* rB(i) := rA(i) if rA(i) = +1
	* rB(i) := 0 otherwise

This is considered a conversion operation, not a logical operation, as it is intended to simulate interfacing with binary hardware.

#### RDR

`rdr` performs the reverse diode operation, changing every +1 trit in the source register to 0, then storing the result in the destination register.

* Format: fdr rA, rB
* Effect: For i in 0..17 do
	* rB(i) := rA(i) if rA(i) = -1
	* rB(i) := 0 otherwise

### I/O

For I/O instructions, any value read from or written to the I/O space can be pre-converted into binary or ternary, exactly as with the `bin` or `tri` operations. The setting of the Binary flag determines what conversion, if any, occurs:

| Binary flag | Effect                |
|-------------|-----------------------|
| +1          | Conversion to binary  |
| 0           | No conversion         |
| -1          | Conversion to ternary |

#### INT, INB

The `int` and `inb` instructions take input from an I/O location in ternary or binary, respectively. This word value (16 bits, in the case of binary) is then stored in the destination register.

* Format: int @%ABC, r0
* Effect: r0 := IO(%ABC)

* Format: inb @%ABC, r0
* Effect: r0 := binary_IO(%ABC)

#### OUT, OUB

The `out` and `oub` instructions send a value from the source register to the destination location in I/O space. `out` writes a ternary word, while `oub` writes 16 binary bits.

* Format: out r0, @%ABC
* Effect: IO(%ABC) := r0

* Format: oub r0, @%ABC
* Effect: IO(%ABC) := as_binary(r0)

### System

#### SYS

`sys` initiates a system call, an unconditional branch to the vector at a given location in the system call table. Trireme allows this table to be 729 entries in length, with each entry a word-length memory address. (As the address space is only 12 trits, the upper hexad is ignored at present.) The current value of the instruction pointer is stored in the system return register `rr`.

The system vector register `rv` contains the base address of the system call table.

* Format: sys #123
* Effect: The following, in order
	* rr := IP
	* address_temp := rv + (123 * 3)
	* IP := memory(address_temp)

#### SRT

`srt` returns from a system call subroutine by loading the instruction pointer with the word in the system return register `rr`.

* Format: srt
* Effect: IP := align(rr)

#### BRK

The `brk` instruction triggers a debug breakpoint exception.

* Format: brk
* Effect: Raise the `#BK` hardware exception

#### NOP

The `nop` instruction does nothing. It can be used for padding or to indicate uninitialized code space.

* Format: nop
* Effect: none

#### LSR

`lsr` loads the value in a system register into a general-purpose register. Valid registers begin with CR (control register) or DR (debug register). The instruction pointer can be loaded by specifying CR0.

* Format: lsr CR1, r0
* Effect: r0 := CR1

If the Protect flag is set to +1, then this instruction raises a `#PV` hardware exception.

#### SSR

`ssr` stores the value in the source register into a given system register. CR0 (the IP) may not be overwritten with this instruction, but all others are valid.

* Format: ssr r0, DR1
* Effect: DR1 := r0

If the Protect flag is set to +1, then this instruction raises a `#PV` hardware exception.

#### UND

The `und` instruction maps to a specific opcode that is guaranteed to remain undefined in all future versions of the Trireme architecture. It will thus always trigger a hardware exception.

* Format: und
* Effect: Raise the `#OP` hardware exception

### Assembler pseudo-instructions

The Trireme simulator's included assembler defines 8 pseudo-instructions. None of these are executed by the processor itself; they are intended to aid the programmer.

#### DH, DW

These two instructions insert the given hexads (for `dh`) or words (for `dw`) directly into the assembler's output.

* Format: dh 1, 2, 3
* Effect: The hexads %0A, %0B, %0C are inserted at the assembler's current location

* Format: dw -1, -2, -3
* Effect: The words %00000n, %00000o, %00000p are inserted at the assembler's current location

#### DB

`db` inserts a binary byte value at the current location; this value is encoded into the lowest 8 trits of a ternary word.

* Format: db 255
* Effect: The word %000qzz is inserted at the assembler's current location

#### DS

`ds` inserts a string of ASCII characters, each contained in a single hexad, into the assembled output.

* Format: ds "Hello, world"
* Effect: 12 hexads, corresponding to the bytes in the given string, are inserted at the assembler's current location

#### ZH, ZW

These two instructions insert a given number of hexads or words, all of which are set to zero. This is intended to be used, for example, in defining memory buffers.

* Format: zh 10
* Effect: 10 hexads, all %00, are inserted at the assembler's current location

* Format: zw 3
* Effect: 3 words, all %000000, are inserted at the assembler's current location

#### EQ

`eq` defines a symbolic constant in the assembler. It creates no output, but allows the given value to be substituted for the identifier at any point during assembly.

* Format: eq X, %ABCD
* Effect: The identifier X is now defined to have the value %00ABCD

#### AD

`ad` changes the current assembly location.

* Format: ad %000n
* Effect: The next assembled instruction will be placed at address %000n

# Miscellaneous notes

## Memory map

Though the CPU currently makes no distinction, its 12-trit address space is, in fact, divided into separate regions for user and system code and data. All words with negative addresses (excluding the last trit of the address) are considered to lie within the system region, while the rest are for use by user applications.

A unit of 729 hexads can be called a memory *page*. Using this terminology, the lowest 81 pages of system memory (addresses %zzzz-%xMMM) are reserved for a system kernel, analogous to ROM on a real microcontroller. The following 27 pages (%wzzz-%wMMM) are reserved as system work RAM. Next, the 27 pages from %vzzz-%vMMM are intended for a future text-mode video display. All other system memory locations are considered open, though the "top" half page (%00zz-%000o) holds the CPU stack by default.

At boot, execution begins at location %zzzz, while the stack pointer is set to %000q.

## System RAM

The system RAM area holds two main defined areas. First, the system call table is located in the 3 pages at %wzzz-wxMM. This vector table contains a series of pointers used by the `sys` instruction.

Following the system call table is the hardware vector table. Only a single page in length (%wwzz-%wwMM), this table contains pointers to the interrupt routines called when a hardware exception occurs.

Both of these tables can be moved to any location in memory. The system call table's base address is located in the `rv` special register, while the system register CR2 holds the base address of the hardware vector table. Both of these can be protected from rogue code by setting appropriate system flags.
