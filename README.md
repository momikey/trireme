## Description

Trireme is a simulator for a hypothetical computer system designed around [balanced ternary](https://en.wikipedia.org/Balanced_ternary) arithmetic.

## Installation

Trireme uses CMake for configuration and compilation. It uses a few third-party libraries:

* [Boost](https://www.boost.org) (1.66.0+)
* [PEGTL](https://github.com/taocpp/PEGTL/tree/2.x) (2.7.1+)
* [fmt](https://fmt.dev/) (6.0.0+)
* [Replxx](https://github.com/AmokHuginnsson/replxx) (0.0.2)

In the future, I'd like to set up Conan or some other dependency manager. For now, I have Boost and PEGTL installed globally, with fmt and Replxx copied into the `thirdparty` directory (not included in this repo).

## Rationale

Although essentially every computer in existence today relies on binary arithmetic, other bases are possible, and they have been used historically. Decimal-based systems were once somewhat common in the US, while the USSR's (mostly experimental) Setun functioned using balanced ternary. Trireme is my attempt at exploring the possibilities offered by the change of this most fundamental level of computing. It's intended for experimenting, education, and a way to see just how many of our assumptions rely on binary.

Why bother? I have my reasons:

* I'm very interested in the lowest levels of computing and development.
* I have a nostalgic fondness for older architectures such as the 6502.
* I wanted to write something in Modern C++, and what's better for C++ than a low-level simulation?
* Balanced ternary arithmetic has appealed to me ever since I read it in Knuth's _The Art of Computer Programming_.
* I thought it'd be fun to experiment with something like this, if only to see how algorithms, data structures, and other fundamental aspects of computer science carry over when you change the math.

## The computer

Trireme simulates a simple processor. (In the future, I'd like to add "peripherals" such as audio, graphics, etc.) In essence, it's a microcontroller, an embedded system with limited memory. It's a load-store architecture, though not fully RISC, and it has all the basic features you'd expect from a 6502, Z80, or similar CPU. Except, of coures, binary arithmetic.

What you get:

* 14 general-purpose registers
* Hardware multiplication and division
* 177,147 words of memory
* A separate I/O space with almost 20,000 memory locations
* Binary I/O instructions

What will come later:

* A plugin architecture for I/O peripherals, including those made by you
* Floating-point arithmetic
* Additional emulated hardware, such as video (character and pixel), audio, and more

## Thinking in ternary

The Wikipedia link above should give you an idea of what balanced ternary *is*. For how it relates to the Trireme simulator, read on.

Trireme uses the concept of a *hexad*"*. This is a ternary analogue to the byte, containing 6 trits instead of 8 bits. (The proper term might be "sextet", but I think "hexad" is more aesthetically pleasing, as well as less prone to misunderstanding.)

Hexads can hold values up to ±364; the balance in balanced ternary means that negative numbers are first-class values, with no need for complement notation. Memory in Trireme is addressed by hexads, and some assembler instructions operate directly on them. In addition, they can be used to store ASCII characters or the individual bytes of a UTF-8 character.

Three hexads make a *word*. Trireme uses a fixed-length instruction set, so all instructions are one word long, the same size as machine registers. Words have a numerical range of around ±193 million, or slightly more than 28 bits. (18 * ln(3)/ln(2) ~= 28.53, in case you're wondering.)

All the usual arithmetic works the same in ternary as it does in binary, so you only need to worry about the different ranges. But since we're not working with bits, we can't really do bitwise operations. Instead, Trireme offers a selection of "tritwise" ops:

* Shifts (`shl`, `shr`) and rotates (`rol`, `ror`, `rcl`, `rcr`) work the same as on binary CPUs. You can't use a shift to multiply or divide by 2, however; in ternary, it's 3 instead.
* `min` and `max` are roughly equivalent to binary OR and AND, respectively.
* We have three different forms of inversion, (`sti`, `pti`, `nti`) depending on what value 0 should convert to. "Simple" ternary inversion leaves it unchanged, so it's considered the counterpart to NOT.
* The `teq` operation gives a value of +1 if two trits are the same, -1 otherwise. This is close to an inverted XOR.
* `tem` is nothing more than a tritwise multiplication.

There are, in fact, thousands more basic operators, but these are the ones Trireme uses at present. In the future, I may add new instructions for some of the rest, depending on what looks to be necessary or useful.

### Note on notation

For ease of writing and understanding, I have devised a base-27 encoding for Trireme integers. Base-27 numbers are in all cases prefixed with %. (I chose this symbol because it evokes a balance scale, appropriate for balanced ternary.)

Digits use the numeral 0, the uppercase letters A-M, and the lowercase letters n-z. 0 obviously represents 0. The uppercase letters are increasing positive integers (A=1, B=2, etc.), while the lowercase set represents decreasing negative integers (n=-1 to z=-13).

As 27 is the cube of 3 (3<sup>3</sup> = 27), each base-27 digit encodes three trits. Thus, a hexad is two digits, and a machine word six. This is similar to hexadecimal notation, in which two digits make a byte, while four, eight, or 16 make a word.

Some examples of the notation:

* %00 = 0
* %0A = 1
* %0w = -10
* %By = 42
* %000ABC = 786
* %0n0000 = -531441
* %vKutCI = -123456789

## The shell

Trireme comes with a rudimentary shell that allows you to explore the simulated CPU. From this command-line interface, you can type assembly instructions, and they will be assembled and inserted into the CPU's memory. In addition, you have access to the following commands:

* `.reg R# (= value)` prints or sets the value of any of the CPU's 27 accessible registers. These are "numbered" as `r0`, `rA-rM`, and `rn-rz`. Note that the letters *are* case-sensitive; uppercase letters indicate general-purpose registers, while lowercase letters are for special-purpose registers, not all of which are used at present. Values can be specified as decimal integers or in the base-27 notation described above.
* `.mem address (= value)` prints or sets the value of the memory word starting at `address`. Both the address and value can be given as integers in decimal or base-27. Note that Trireme currently does no checking to ensure alignment.
* `.ip address` sets the instruction pointer to `address`. In this case, the value you give may be changed to be word-aligned; for Trireme, this means the lowest trit should be negative, so an IP value of 0 will be realigned to -1.
* `.at address` sets a "virtual" instruction pointer. This isn't where program execution will start. Instead, it is where the next assembled instruction will be stored. An alternative command, `.org`, may be used in place of `.at`; this may be more familiar to users of binary assemblers.
* `.run` begins execution at the address given by the instruction pointer. Execution continues until a debug breakpoint (`brk` instruction) or some other error. At present, Trireme has very little in the way of error checking, so beware of infinite loops and other problems.

## License

Trireme is open source software under the [MIT license](LICENSE).
