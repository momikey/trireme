# Project description

Trireme is a theoretical processor architecture using balanced-ternary (BT) arithmetic and storage.

## Uses

The project is intended to illustrate the assumptions of binary encoding, the possibilities offered by other arithmetics, and the generic nature of computer science elements such as algorithms.

In addition, some present research projects point to the potential utility of BT. For example, a microcontroller based on Josephson junctions rather than traditional FETs might see better power usage with BT instead of binary.

# Terminology and symbols

## Terms

* BT - balanced ternary
* trit - the ternary counterpart to a bit, an element that can take one of three values: +1, 0, -1
* hexad - the ternary counterpart to a byte, a string of 6 trits

"Hexad" is not industry-standard terminology, but I personally dislike the term "sextet" for the smallest addressable unit of memory, and neologisms such as "tryte" are not pleasing to my eye. Thus, I went with "hexad" as the byte analogue. It should not be seen as a shortening of "hexadecimal", as hex is a bit nonsensical on a balanced ternary architecture. It just means that you're dealing with a group of 6 trits, nothing more.

## Notation

* The basic 3 values of a trit are 0, +1, and -1. For notational convenience, we abbreviate to + and - for positive and negative 1, respectively.

## Base-27 encoding

For further compacting, 3 trits can be combined into a single character. This requires 27 values, in the range $[-13,+13]$. Conveniently, the English alphabet employs 26 letters, so we encode the 3-trit values in the following way, where H, M, and L represent the high, middle, and low trits of the triad, respectively:

| H | M | L | Char | Value |
|---|---|---|------|-------|
| 0 | 0 | 0 | 0    | 0     |
| 0 | 0 | + | A    | 1     |
| 0 | + | - | B    | 2     |
| 0 | + | 0 | C    | 3     |
| 0 | + | + | D    | 4     |
| + | - | - | E    | 5     |
| + | - | 0 | F    | 6     |
| + | - | + | G    | 7     |
| + | 0 | - | H    | 8     |
| + | 0 | 0 | I    | 9     |
| + | 0 | + | J    | 10    |
| + | + | - | K    | 11    |
| + | + | 0 | L    | 12    |
| + | + | + | M    | 13    |
| - | - | - | z    | -13   |
| - | - | 0 | y    | -12   |
| - | - | + | x    | -11   |
| - | 0 | - | w    | -10   |
| - | 0 | 0 | v    | -9    |
| - | 0 | + | u    | -8    |
| - | + | - | t    | -7    |
| - | + | 0 | s    | -6    |
| - | + | + | r    | -5    |
| 0 | - | - | q    | -4    |
| 0 | - | 0 | p    | -3    |
| 0 | - | + | o    | -2    |
| 0 | 0 | - | n    | -1    |

As the table illustrates, negative numbers are represented by lowercase letters, while capital letters encode positive integers. In addition, the familiar ROT-13 algorithm, combined with a change in case, indicates negation.

Where needed, use of this notation can be denoted using a prefixed %. (The symbol vaguely resembles a balance as seen from above.)

# Using the simulator

The Trireme simulator works at the instruction level. Timings aren't important on an architecture that doesn't exist in the real world, so they're not included, although this may change in the future. Also, no peripherals are implemented at the moment, apart from a simple string I/O system intended for debugging, experimentation, and illustration.
