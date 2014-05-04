Handy Helpful Hints for using make/macross/slinky

1. You need two header files: a header and a sub-header.  The sub-header
declares 'define' symbols, zero page labels (as local labels), macros,
functions, and so on.  The header includes the sub-header and then
additionally contains 'extern' declarations for all program-space labels and
non-zero page variable-space labels plus 'define' declarations which refer to
externals.

Non-zero page variable-space should be laid out in one file that includes just
the sub-header.  The rest of the program should be spread among files each of
which includes the header.

2. In any given Macross assembly (usually one source file plus all of the
other files that it 'include's), you can classify each symbol in two different
orthogonal ways: whether it is global or local on the one hand and whether it
is defined in this assembly or defined in some other assembly.

Being defined means that the symbol has a value which is known and can
therefore be used to generate code.  If a symbol has a value when it is
encountered during assembly, then that value can be used immediately.
If not, then we must wait until linking when (presumably) the value will be
known.

Being global means that the symbol's value is known outside of the assembly in
which the symbol is defined (i.e., that the value is "published" in the object
file for use by the linker).  Conversely, being local means that the value is
known only in the one assembly.

Let's look at the four possible cases when a symbol is encountered during
assembly:

LOCAL, DEFINED: this is the usual case.  The value is used at assembly time.
Multiple independent assemblies can define local symbols with the same names
without conflicting with each other when they are linked.

LOCAL, UNDEFINED: this shouldn't happen.  You will get a complaint from the
assembler.

GLOBAL, DEFINED: during assembly, the symbol is treated just like a defined
local symbol.  However, the value is "published" in the object file for use by
the linker.  The symbol value can be made global either or both of two
different ways: with an 'extern' declaration
		extern foo	; makes the symbol 'foo' global
or, if the symbol is a label, by using two colons instead of one
	foo::	word 123	; 'foo' is global
	bar:	word 456	; 'bar' is local
If a global symbol is defined in more than one object file, the linker will
complain.

GLOBAL, UNDEFINED: this means that the assembler assumes that the value of the
symbol will be defined at link time.  You still have to tell the assembler
that the symbol is global with an 'extern' statement.

Note that all macro, function and variable (Macross variables, not 6502
storage locations used as variables) names MUST be local, as must be struct
names and struct fields.  The assembler will complain if you try to declare
one of these 'extern'.

A 'define' symbol must be local if it refers to any global symbols in its
definition.

3. The fewer global symbols there are, the faster the link will run.

4. When juggling a program that consists of multiple source and object files,
only a very foolish person would not use 'make'.

5. Emacs has a very nice feature whereby you can run 'make' under emacs and
then get taken automagically to the errors:

The emacs function 'compile' (or 'new-compile' on some systems), which is
usually bound to ^X^C (also to the shift-F5 key on my Concept terminal; I
don't know about others), will run make and put the output from make (error
messages and so on) in a buffer with a new window.

After compiling with make, the emacs function 'next-error', which is usually
bound to ^X^N (also to the the F5 key on my Concept) will search in the error
message buffer for the first C-compiler-style error message (which is what
Macross outputs), read the file name and line number from the error message
and then (in another window) position the cursor at that line of that file.
Entering this command again takes you to the next error message, and then the
next, and so on.  The wonderful thing about this is that if you edit your
source file, emacs keeps track of how you have shifted things around so that
it will always take you to the location of the next error, even if your
editing has invalidated the line numbers in the error messages!
