# Intro
Work in progress

# Before going on
This source code doesn't include the CRC32 implementation (yet). Read the README
in the folder `my_crc`.

# Format `.sav`
In the following list, "[n]" means that the field is n bytes long

## Items
- [4] CRC32 checksum of the content of the rest of the file
- [1] 0x06
- [4] The index of the savegame, that is "N" in "gameN.sav"
- [1] Room
- [1] ???
- [2] Play time
- [1] 0x00
- [1] Number of times the player saved
- [2] ???
- [1] The difficulty and mode. Each one of the 4 least significant bits
    represent something. Starting from the rightmost bit, the 0th bit represents
    the easy difficulty, the 1st bit represents the normal difficulty, the 2nd
    bit represents hard, and the 3rd represents Special Mode.
- [1] 0x64

## Each character
- [59] Basic info
- [9*n] Each weapon they have?

# Authors
Francisco "ELPiTiFOR"