Simultaion of Dynamic Instruction Scheduling
====================


In this project,  a simulator is constructed for an out-of-order superscalar processor based on
Tomasulo’s algorithm that fetches, dispatches, and issues N instructions per cycle. Only the
dynamic scheduling mechanism will be modeled in detail, i.e., perfect caches and perfect branch
prediction are assumed.

The simulator reads a trace file in the following format:
<PC> <operation type> <dest reg #> <src1 reg #> <src2 reg #>
<PC> <operation type> <dest reg #> <src1 reg #> <src2 reg #>

Where:
• <PC> is the program counter of the instruction (in hex).
• <operation type> is either “0”, “1”, or “2”.
• <dest reg #> is the destination register of the instruction. If it is -1, then the
instruction does not have a destination register (for example, a conditional branch
instruction). Otherwise, it is between 0 and 127.
• <src1 reg #> is the first source register of the instruction. If it is -1, then the
• instruction does not have a first source register. Otherwise, it is between 0 and 127.
• <src2 reg #> is the second source register of the instruction. If it is -1, then the
• instruction does not have a second source register. Otherwise, it is between 0 and 127.

For example:
ab120024 0 1 2 3
ab120028 1 4 1 3
ab12002c 2 -1 4 7

Means:
“operation type 0” R1, R2, R3
“operation type 1” R4, R1, R3
“operation type 2” -, R4, R7 // no destination register!
It will be possible to download the traces directly from the web.
4. Outputs

Outputs from Simulator:
The simulator outputs the following measurements after completion of the run:
1. Total number of instructions in the trace.
2. Total number of cycles to finish the program.
3. Average number of instructions completed per cycle (IPC).
The simulator also outputs the timing information for every instruction in the trace, in a
format that is used as input to the scope tool. The scope tool’s input format is described in a later
section.

