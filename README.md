# Lab 12

## Overview

This is another reverse-engineering lab. This lab gets a bit more complex since
it is dealing with data and memory layout for structs and arrays of structs.

The main task of this lab is to reverse-engineer three functions in program
`mystery4`. To help, we have also provided a program `prog4.c` which has similar
or identical functions so that you can inspect their assembly translation.

---

### `prog4.c`

To get started, build the program `prog4` by typing `make` at the command
prompt. Then use `gdb` to disassemble the functions `struct_by_val`,
`struct_by_ref`, `set_struct_vals`, and `access_list`.  

Start `gdb` (i.e., `gdb prog4`) and then at the prompt type 
```
disas struct_by_val
```
to see the x86-64 instructions for the function `struct_by_val`.  Be sure to
understand what each instruction is doing. Do the same for all the functions.
Look at the source code in `prog4.c` and Chapter 3.8 (Data) of CS:APP to help
your understanding.

This lab requires you to look at register values and memory locations. The
programs are compiled with optimizations and some debug information, but not
enough to give you line numbers and access to variable names.  

So, you should utilize `gdb` commands like:
`p $<register_name>` and `x/20 <address>`.

Use these to examine machine state for all functions, but in particular look at
memory for the linked list in the function `access_list`. Set a breakpoint at
`access_list`, run until the program stops there, disassemble `access_list` and
you'll see the instructions for `access_list`. Recall the only argument resides
in `%rdi`.

You can show its value by using `p $rdi` (I use `p/x $rdi` to get hex). This is
the value of head, the address of first node in the linked list. 

You can see the fields of the node by typing `x/4 $rdi`, which displays the 4
32-bit words starting at the address contained in $rdi. You should see something
like this.
```
(gdb) x/4 $rdi
0x56330480f260: 0x00000002      0x00000000      0x0480f280      0x00005633
```
This corresponds to the first node with a val = 2 and the next =
0x000056330480f280

Notice two things: 
1. The 0 value in the second 32-bit word
2. The number of bytes between this node's address and the value of next = 32.
   The struct ll_node has an int field and a next field.  The int val field is
   only 32-bits long, hence it occupies the first 4 bytes in memory. The pointer
   next is 8 bytes, and C must align that on an 8 byte boundary, so introduces
   the padding (unused memory) after the val field to obtain this alignment.

Similarly, on our systems, `malloc` returns addresses aligned on `32` byte
boundaries.  Since this list is created by calling `malloc` to obtain memory for
each node, `malloc` always returns `16` bytes even though the node requires only
`8` bytes of memory. 

We will talk a lot more about `malloc`'s implementation later this semester. But
for now to see the full list which has `3` nodes in it, you can type `x/24 $rdi`
at the `gdb` prompt. 

The `24` comes from each node taking eight 4-byte chunks of memory (32 bytes)
and we happen to be calling `malloc` consecutively to allocate the list. If the
nodes were allocated at very different points in the execution they may not be
consecutive and would be better to display each node by using the next value.
For example, to show the next node type:

```
(gdb) x/4 0x000056330480f280
0x56330480f280: 0x00000003      0x00000000      0x0480f2a0      0x00005633
```

**TIP**: the `gdb` command can take a format specifier. `x/4w` would display
what's above using `4`-byte (32-bit) words. `x/4g` would display things in
`8`-byte (64-bit) groups as follows: 

```
(gdb) x/4g 0x000056330480f280
0x56330480f280: 0x0000000000000003      0x000056330480f2a0
0x56330480f290: 0x0000000000000000      0x0000000000000021
```

A final note, the addresses may differ on each execution since Linux uses a
technique called address space randomization that changes memory layout for each
program execution. It's a security thing... We'll get to that soon.

---

### `mystery4`

The main task of this lab is to reverse engineer three functions in program
`mystery4`. There is also an optional fourth function to investigate.

Run the program `mystery4` (type `./mystery4`) it prompts for some inputs and
you need to figure out the correct inputs by examining the instructions of the
corresponding functions.  For this lab, the functions are nearly (but not
quite!) identical to those in `prog4.c`. However, `main` is different and you
don't have source code for that.

You can run the program interactively (you enter answers on the command line)
for initial testing of your program.  You can run each test indidually by
  passing an integer argument to mystery4 (e.g., `./mystery4 1` will perform the
  first test) or just run `./mystery4` and it will progress through all tests in
  order. Ultimately for final testing and submission you need to place your
  answers in files called:
1. `solution_f1.txt`
2. `solution_f2.txt`
3. `solution_f3.txt`
4. `solution_f4.txt` (Optional)
   
where `f1` corresponds to `struct_by_ref`, `f2` to `set_struct_vals`, `f3` to
`access_list`, and `f4` to `struct_by_val`. These files should have your answers
on a single line with spaces between the integer values, followed by a newline. 

For example, if the solution to `f2` is "1 2 3", then the contents of
`solution_f2.txt` should be:
```text
1 2 3

```
(Notice the trailing newline at the end of the file.)

Then you can use file redirect to execute the program with your solution file
(i.e., `./mystery4 1 < solution_f1.txt`)

---

### Final Local Testing Before Submission

Test your results by running

```bash
python3 test_kit.py mystery4
```

If your files are not properly named, the last line of the error will look
something like this:

```bash
FileNotFoundError: [Errno 2] No such file or directory: 'solution_f1.txt'
```

Please follow the naming format we provided for you above, and make sure all
four solution files exist in your lab directory.

---

### Submission

1. Please run the below before submitting to Gradescope to delete your compiled
   executables (NOTE: `mystery4` is provided to you and SHOULD NOT be deleted):
   ```bash
   make clean
   ```

2. Submit your solution .txt files to Gradescope via GitLab submission.
