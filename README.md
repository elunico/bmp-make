# Writing BMPs from Strings of text

This program will let you create a BMP image from a string of input.

It is composed of primarily two parts


## `transform.cpp`
The `transform.cpp` program is meant to read a file off the file system and transform it in a way that produces useful output.
This output is written to standard out in order to work with the `main.cpp` program. The expected format for the input to that
program is described in the next section. The output of `transform.cpp` should be appropriate for input to `main.cpp`

For the most part, using this program would focus on modifying this file. The goal of this file is to take some data, usually
off the file system, and transform it into a useable stream of data for `main.cpp` which is then written to stdout and piped to `main.cpp`

### Command Line Arguments
This program also takes between two and three command line arguments

The first argument is the name of the file whose contents will be transformed by the program

The second file is the format of data to output to standard out. It can be either `bin` or `dec` which correspond to the expected input data for `main.cpp` for more information on the data format of each of these modes, see the command line arugments section in the `main.cpp` section

The third argument is how chars per line to output from the data. New line chars are inserted in the data by default. They are inserted such that the resulting data has as many columns per line as lines in total (or as close as possible). Since `main.cpp` uses line breaks to deliminate lines in the image, this effectively produces data that will result in square images. This can be changes by supplying a third argument which indicates how many chars per line in the output there should be. When this is then fed to `main.cpp` this will have the effect of determining how many pixels per line of the output image there will be

### Example

transforming digits of pi saved to a file on the file system into binary equivalents

```shell
$ ./transform pi-1000000.txt bin
```

## `main.cpp`
This file reads input from stdin. The input is in the form of numbers encoded as normal ascii text. In other words, this program reads
ASCII chars from input in the range `0x30` through `0x39` and `0x61` through `0x66` plus `0x07`. This is done to allow on-the-fly creation of images through piping in the terminal.

The actual range of acceptable input chars and their values varies based on the first command line argument. More information is presented below.

The format of data uses the new line char `\\n` to deliminate lines of the image.

Therefore an input string such as `1111\n1111\n0000\n0000` in `bin` mode would have two 4-pixel lines of white and then 2 4-pixel lines of black.

### Command line arguments

The program takes 2 command line arguments

The first argument to the program is the filename to save the bmp as

The second argument is the format of the input. This may be `bin`, `dec`, or `hex` and it indicates
to the `main.cpp` program what kind of data it will be given. This determines the brightness levels for each input digit. If binary mode is used
with the `bin` argument, then `0` represents black and `1` represents white. If `dec` is used then `0` represents black and `9` represents white with shades of gray between. Finally, if `hex` is used then `0` represents black and `f` represents white and shades of gray inbetween.

***Note that only lowercase hex values can be used meaning letters `A` through `F` are invalid only `a` through `f` are allowed***

### Example:

a 4x4 image with two white pixel rows and 2 black pixel rows

```shell
$ echo '1111\n1111\n0000\n0000' | ./main bin image.bmp
```

### Example

creating an image from a file containing digits of pi

```shell
$ cat pi-1000000.txt | ./main dec pi.bmp
```

### Example

reading the digits of pi off a file in the file system and using `transform.cpp` to convert them to binary then piping them to `main.cpp` to create the image

```shell
$ ./transform pi-1000000.txt bin | ./main bin pi-bin.bmp
```
