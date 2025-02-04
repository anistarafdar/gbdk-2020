@page docs_toolchain GBDK Toolchain


@anchor toolchain_overview
# Overview
GBDK 2020 uses the SDCC compiler along with some custom tools to build Game Boy ROMs.
- All tools are located under `bin/`
- The typical order of tools called is as follows. (When using lcc these steps are usually performed automatically.)
  1. Compile and assemble source files (.c, .s, .asm) with @ref sdcc and @ref sdasgb
  2. Optional: perform auto banking with @ref bankpack on the object files
  3. Link the object files into .ihx file with @ref sdldgb
  4. Validate the .ihx file with @ref ihxcheck
  5. Convert the .ihx file to a ROM file (.gb, .gbc) with @ref makebin

To see individual arguments and options for a tool, run that tool from the command line with either no arguments or with `-h`.


# Data Types
For data types and special C keywords, see @ref file_asm_gbz80_types_h "asm/gbz80/types.h" and @ref file_asm_types_h "asm/types.h".

Also see the SDCC manual (scroll down a little on the linked page): http://sdcc.sourceforge.net/doc/sdccman.pdf#section.1.1


@anchor toolchain_changing_important_addresses 
# Changing Important Addresses
It is possible to change some of the important addresses used by the toolchain at link time using the -Wl-g XXX=YYY and =Wl-b XXX=YYY flags (where XXX is the name of the data, and YYY is the new address). 

@ref lcc will include the following linker defaults for @ref sdldgb if they are not defined by the user.

  - `_shadow_OAM`
    - Location of sprite ram (requires 0xA0 bytes).
    - Default `-Wl-g _shadow_OAM=0xC000`

  - `.STACK`
    - Initial stack address
    - Default `-Wl-g .STACK=0xE000`

  - `.refresh_OAM`
    - Address to which the routine for refreshing OAM will be copied (must be in HIRAM). Default
    - Default `-Wl-g .refresh_OAM=0xFF80`

  - `_DATA`
    - Start of RAM section (starts after Shadow OAM)
    - Default `-Wl-b _DATA=0xc0A0`

  - `_CODE`
    - Start of ROM section
    - Default `-Wl-b _CODE=0x0200`


@anchor toolchain_compiling_programs
# Compiling programs

The @ref lcc program is the front end compiler driver for the actual compiler, assembler and linker. It works out what you want to do based on command line options and the extensions of the files you give it, computes the order in which the various programs must be called and then executes them in order. Some examples are:

  - Compile the C source 'source.c', assemble and link it producing the Gameboy image 'image.gb'

        lcc -o image.gb source.c

  - Assemble the file 'source.s' and link it producing the Gameboy image 'image.gb'

        lcc -o image.gb source.s

  - Compile the C program 'source1.c' and assemble it producing the object file 'object1.o' for later linking.

        lcc -c -o object1.o source1.c

  - Assemble the file 'source2.s' producing the object file 'object2.o' for later linking

        lcc -c -o object2.o source2.s

  - Link the two object files 'object1.o' and 'object2.o' and produce the Gameboy image 'image.gb'

        lcc -o image.gb object1.o object2.o

  - Do all sorts of clever stuff by compiling then assembling source1.c, assembling source2.s and then linking them together to produce image.gb. 

        lcc -o image.gb source1.c source2.s

Arguments to the assembler etc can be passed via lcc using -Wp..., -Wf..., -Wa... and -Wl... to pass options to the pre-processor, compiler, assembler and linker respectivly. Some common options are:

  - To generate an assembler listing file.

        -Wa-l

  - To generate a linker map file.

        -Wl-m

  - To bind var to address 'addr' at link time.

        -Wl-gvar=addr

For example, to compile the example in the memory section and to generate a listing and map file you would use the following. Note the leading underscore that C adds to symbol names.

    lcc -Wa-l -Wl-m -Wl-g_snd_stat=0xff26 -o image.gb hardware.c


@subsection Makefiles
Using Makefiles

Please see the sample projects included with GBDK-2020 for a couple different examples of how to use Makefiles.

You may also want to read a tutorial on Makefiles. For example:  
https://makefiletutorial.com/
https://www.tutorialspoint.com/makefile/index.htm

@anchor build_tools
# Build Tools

@anchor lcc
## lcc
lcc is the compiler driver (front end) for the GBDK/sdcc toolchain.
It can be used to invoke all the tools needed for building a rom.
If preferred, the individual tools can be called directly.
- the `-v` flag can be used to show the exact steps lcc executes for a build
- lcc can compile, link and generate a binary in a single pass: `lcc -o somerom.gb somesource.c`
- @anchor lcc_debug
  lcc now has a `-debug` flag that will turn on the following recommended flags for debugging
    - `--debug` for sdcc (lcc equiv: `-Wf-debug`)
    - `-y` enables .cdb output for @ref sdldgb (lcc equiv: `-Wl-y`)
    - `-j` enables .noi output for @ref sdldgb (lcc equiv: `-Wl-j`)


@anchor sdcc
## sdcc
SDCC C Source compiler
- Arguments can be passed to it through @ref lcc using `-Wf-<argument>` and `-Wp-<argument>` (pre-processor)


@anchor sdasgb
## sdasgb
SDCC Assembler for the gameboy
- Arguments can be passed to it through @ref lcc using `-Wa-<argument>`


@anchor bankpack
## bankpack
Automatic Bank packer

When enabled, automatically assigns banks for object files where bank has been set to `255`, see @ref rom_autobanking.
Unless an alternative output is specified the given object files are updated with the new bank numbers.
- Can be enabled by using the `-autobank` argument with @ref lcc.
- Must be called after compiling/assembling and before linking
- Arguments can be passed to it through @ref lcc using `-Wb-<argument>`

Limitations
- `__banked` functions cannot be called from within the same source file they are declared in.
- With data it is easier, because if you access data from the code in the same bank you don’t need to switch the bank (access to `__bank_*` symbol).


@anchor sdldgb
## sdldgb
The SDCC linker for the gameboy.

Links object files (.o) into a .ihx file which can be processed by @ref makebin
- Arguments can be passed to it through @ref lcc using `-Wl-<argument>`


@anchor ihxcheck
## ihxcheck
IHX file validator

Checks .ihx files produced by @ref sdldgb for correctness.
- It will warn if there are multiple writes to the same ROM address. This may indicate mistakes in the code or ROM bank overflows
- Arguments can be passed to it through @ref lcc using `-Wi-<argument>`


@anchor makebin
## makebin
IHX to ROM converter

Converts .ihx files produced by @ref sdldgb into ROM files (.gb, .gbc).
- Arguments can be passed to it through @ref lcc using `-Wm-<argument>`


@anchor gbdk_utilities
# GBDK Utilities


@anchor utility_gbcompress
## GBCompress
Compresssion utility

Compresses (and decompresses) binary file data with the gbcompress algorithm (also used in GBTD/GBMB). Decompression support is available in GBDK, see @ref gb_decompress().


@anchor png2mtspr
## PNG to Metasprite
Tool for converting PNGs into GBDK format MetaSprites

@todo Document png2mtspr
