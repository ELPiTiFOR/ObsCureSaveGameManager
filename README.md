# Intro
Work in progress

# Format `.sav`
In the following list, "[n]" means that the field is n bytes long

## Items
- [4] CRC32 checksum of the content of the rest of the file
- [1] 0x06
- [4] The index of the savegame, that is "N" in "gameN.sav"
- [1] Room
- [4] Play time (number of milliseconds)
- [1] Number of times the player saved
- [2] ???
- [1] The difficulty and mode. Each one of the 4 least significant bits
    represent something. Starting from the rightmost bit, the 0th bit represents
    the easy difficulty, the 1st bit represents the normal difficulty, the 2nd
    bit represents hard, and the 3rd represents Special Mode.
- [7] 0x64 00 44 08 31 02 3E

- [1] (opt) 0x07 (--> handgun ammo ahead?)
- [4] Handgun ammo in MSB
- [4] Handgun ammo in LSB
- [1] (opt) 0x06 (--> shotgunammo ahead?)
- [4] Shotgun ammo in MSB
- [4] Shotgun ammo in LSB

- [9*n] Each item in the L2/L/Q/Left inventory
    - [4] ID location of the item in LSB
    - [1] Quantity of that item ?
    - [3] Special info on that item ? (only for special items like maps, keys, documents)
    - [1] 0x00

## Each character (starts at offset 0x247)
- [59] Basic info (order might be: ???, Josh, Kenny, Ashley, Shannon)
    - [3] ??? always 0x37 01 1C
    - [4] The door next to which the character is standing (? does it really take 4 bytes?)
    - [1] The room where the character is located
    - [14] ???
        - [1] 0x00???
        - [4] X coordinate in LSB?
        - [4] Y coordinate in LSB?? not sure at all
        - [4] Z coordinate???
        - [1] Rotation
    - [4] ID location of the weapon currently in the hand of the character, in LSB
    - [29] ???
                    Ignore the next 4
                    - [4] X coordinate in LSB ?
                    - [4] Y coordinate in LSB ?
                    - [4] Z coordinate in LSB ?
                    - [4] Rotation in LSB?
    - [4] Health as a single precision float in LSB
- [9*n] Each weapon they have?
    - [4] ID location of weapon
    - [1] quantity (always 0x01)
    - [4] Special info (gun ammo (byte and a half), attached flashlight <-- need to investigate on this...)

## Progress? (starting at 0x85A)
Here there is some info on the progress of the game (the drawings on the map!).
- [m] This pattern is repeated an arbitrary number of times :
    - [2] Length of this section, without these 2 bytes (n) in LSB
    - [n] Info, mostly info of a room, like the items that have been picked up
        - [1] 1byte-ID of the room (just like at offset 0x09)
        - [1] Info on the kinematics that have already been played or not?
        - [n-2] ???
- [1] 0x09 --> this represents the end of the Progress section

# Authors
Francisco "ELPiTiFOR"