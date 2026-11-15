# Nuked-MT32

WIP low-level Roland MT-32 emulator by nukeykt.

Uses MCS-96 emulator from Olivier Galibert/MAME.

LA32 emulator is based on the decap of John McMaster.

Reverb chip emulator is based on the decap of InfoSecDJ.

Extra thanks to giulioz for sending the reverb chip for decapping and finding bugs in LA32 core.

## Supported devices

Only second generation of MT-32 is currently emulated.

ROM images should be named as such:

- Control ROM (128kb) -> mt32_cpu.bin
- PCM ROM (512kb) -> mt32_pcm.bin
- Reverb ROM (32kb) -> mt32_reverb.bin

## Control key mapping

- Q -> Part 1
- W -> Part 2
- E -> Part 3
- R -> Sound group
- T -> Volume
- A -> Part 4
- S -> Part 5
- D -> Part 6
- F -> Sound
- G -> Master volume
- \+ and \- -> Volume knob
