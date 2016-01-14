# macross 6502, an assembler for people who hate assembly language

*Macross* is a fancy macro cross assembler for the 6502. Macross assembles the given files in the order specified on the command line. Macross source file names must end with ``.m''.

*Slinky* is a multi-file relocating object linker designed to be used in conjunction with the Macross assembler. Slinky links the given Macross object files in the order specified on the command line together into one executable object file.


## History

Macross and Slinky were developed 1984-1987 by Chip Morningstar at Lucasfilm Ltd. for use in the game [Habitat](https://github.com/Museum-of-Art-and-Digital-Entertainment/habitat).

The source has been resurrected in the context of preserving and restoring Habitat at [The Museum of Art and Digital Entertainment](http://www.themade.org/).


## Status

The original archive contents are checked in as the first commit. Subsequent commits provide clean ups and compile fixes for modern machines.

Currently, Macross and Slinky compile and work on OS X (32 bit).


## Documentation

See the directory [doc](doc).


## License

MIT