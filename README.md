# Multiplicative Persistence Search

A search algorithm for finding integers with multiplicative persistence.

## Building and Running

This program is written in strict ANSI C, so it should compile on any GNU/Linux
platform as long as `gcc`, `make`, and `gmp-devel` are installed. To build, just
run `make`.

    Usage: ./mp-search <digits> [<seed>] [<d>] [<min>]

**<digits>** is the only required parameter and is the number of digits in the
integer for which you'd like to search. For example, use 234 to generate a vast
number of 234-digit numbers and check them for persistence. Since 11 is the
current persistence record, the program will only output to STDOUT if a
persistence of 12 or greater is found.

Every 60 seconds, the program will output the current state to STDERR. You can
plug the state back into another run of the program to continue where it left
off (checkpointing in case of failure). You can also launch multiple parallel
copies of the program on different numbers of digits, for example.

## References

Check out the [Numberphile video](https://youtu.be/Wim9WJeDTHQ) by Matt Parker
that inspired me to write this program.
