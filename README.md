## Description

Trireme is a simulator for a hypothetical computer system designed around [balanced ternary](https://en.wikipedia.org/Balanced_ternary) arithmetic.

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

## License

Trireme is open source software under the [MIT license](LICENSE).
