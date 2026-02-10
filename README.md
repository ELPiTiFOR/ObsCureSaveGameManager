# Intro
This tool aims to provide a way to comfortably manage more than 10 savegames for
the game ObsCure. This game only provides 10 slots, which may not be enough for
some people. If you're out of slots, you can make some space by exporting some
or all of your savegames to another directory. If you want them back, you can
import them to one of the 10 slots.

ObsCureSaveGameManager lets you select the folder where your in-game savegames
are located, and the folder where you want to save or keep your backups. After
that, you can easily export your savegames (to create new backups) and import
your backups (to be able to load them in-game).

# Version
This tool was developed on Windows 10 and was tested with the Steam version of
ObsCure.

# How to use
First, go to the settings by click on the "Settings" button in the upper right
corner of the menu. Then set the path of your backups directory and the one of
your savegames (you can also set the path where the game is installed but that
is only useful for a feature that cannot be used through the Graphical User
Interface). Use the "Browse" button to open the File Explorer and select a
folder from your disk drive. Don't forget to apply the changes. These settings
will be saved in a `.config` file.

You might be wondering where your ObsCure savegames are located. You should go
to the directory where Steam is installed in your PC and search for this:
`E:\Program Files (x86)\Steam\userdata\{some_number}\254460\remote`
The "{some_number}" thing represents a directory the name of which is a number,
which I think is related to your Steam account. The `254460` directory is the
one of ObsCure.

Once you've set up your paths, you can start exporting some of your savegames.
The menu is divided into the list of backups (left part of the menu, which
should be empty at first) and the list of savegames in-game (right part). The
backup list only shows a limited number of backups at a time. In order to
navigate through these pages, use the arrow buttons.

## Export a single savegame
To export a savegame, click on a savegame from the savegame list (right part),
a window will pop up asking for a name, give your backup the name you want and
click on "Export".

## Import a single backup
To import a backup, click on the backup from the backup list (left part) and
then click on the savegame that you want to overwrite. If you've selected a
backup but don't want to import it anymore, click on the "X" (cross) button in
top of the backup list to deselect it.

## Export all the savegames
You can export all 10 savegames at once with the "Export All" button, you have
to give the group of savegames a name first, use the text input next to the
button for that purpose. This will also generate an `.ls` file which specifies
which backups are part of this group/list.

## Import a whole list
You can import a group of 10 backups at a time if you've previously exported
them with the "Export All" feature. To do that, write the name of the group in
the text input and click on "Import All"

## Refresh
If you save your game in-game, the list of savegames on the tool won't be
updated. To refresh the list of savegames and backups, use the "Refresh" button.

## Correct CRC32
The `.sav` files start with a 4 byte integer in LSB that represents the CRC32
checksum of the rest of the file. The game checks if the integer and the actual
CRC32 checksum are equal, if not, it will consider the savegame is corrupt. If
you manually modify the `.sav` files, the CRC32 checksum will probably not be
correct anymore. In order to correct it, you can use the "C" button next to the
corresponding slot.

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
- [6] 0x64 00 44 08 31 02

- [1] Number of item slots (groups of 9 bytes) (usually 0x3E)
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