# RISC-V disassembler

RISC-V disassembler is a simple disassembler for RISC-V encoded instuctions. It currently only supports the RV32I Base Instruction Set.
It operates in two modes:
1. REPL Mode: Running the tool without any arguments spins up a REPl where you can enter an instruction and receive it decoded.
2. File Mode: Running the tool with a file path as an argument, it reads the contents of the file and decodes the line separated instructions
in the file. This is perfect for batch disassembling.

## Installation

Clone the repository and build it using *Make*.

```bash
git clone https://github.com/niclassic/disassembler.git
cd disassembler
make
``` 

## Usage

```bash
./disassembler
Enter an instruction (e.g. 0x80000537). Exit with "exit."
>> 0x80000537
LUI x10 -524288
>> exit
```

## License

MIT License

Copyright (c) [2024] [niclassic]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
