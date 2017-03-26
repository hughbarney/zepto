# Zepto Emacs
Smallest usable Editor in less than 1000 lines of C.

Zepto Emacs is the smallest in a family of Emacs type editors inspired by MicroEmacs, Nano, Pico and my earlier project known as Perfect Emacs [1].

* Zepto is a single window minimal editor in less than 1000 lines of C that uses VT100 escape sequences instead of ncurses.
* Atto  is the smallest fuctional Emacs in less than 2000 lines of C.
* FemtoEmacs is an Atto based Emacs with the FemtoLisp extension language.

> A designer knows he has achieved perfection not when there is nothing left to add, but when there is nothing left to take away.
> -- <cite>Antoine de Saint-Exupery</cite>

## Goals of Zepto Emacs

* Provide just enough editing features to be able to make small changes to files
* Act as an experimental platform for using VT100 / rawio instead of using the curses library
* Smallest code footprint to demonstrate the buffer editor concept without the distraction of more advanced editor features.
* Be easy to understand without requiring extensive study (to encourage further experimentation).

## Why the name Zepto ?
The small Emacs naming scheme appears to use sub-unit prefixes in decending order with each further reduction of functionality. The Nano and Pico Emacs editors have been around for a while.

* Nano means 10 to the power of minus 9
* Pico means 10 to the power of minus 12 
* Femto means 10 to power of minus 15
* Atto means 10 to power of minus 18
* Zepto means 10 to the power of minus 21

In Defining Atto as the lowest functional Emacs I have had to consider the essential feature set that makes Emacs, 'Emacs'.  I have defined this point as a basic Emacs command set and key bindings; the ability to edit multiple files (buffers), and switch between them; edit the buffers in mutliple windows, cut, copy and paste; forward and reverse searching, a replace function and basic syntax hilighting. The proviso being that all this will fit in less than 2000 lines of C.

Zepto has the smallest possible feature set to make a viable file editor. Zepto supports basic movement around the file, character insertion, deletion, backspace, line deletion, cut, copy, paste and the ability to search for a text string.  Although Zepto uses a subset of the Emacs keyboard command set; it cant really be considered to be an Emacs in that it does not support the editing of multiple files in multiple windows.

## Derivation
Femto, Atto and Zepto is based on the public domain code of Anthony Howe's editor (commonly known as Anthony's Editor or AE, [2]).  Rather than representing a file as a linked list of lines, the AE Editor uses the concept of a Buffer-Gap [4,5,6].  A Buffer-Gap editor stores the file in a single piece of contiguous memory with some extra unused space known as the buffer gap.  On character insertion and deletion the gap is first moved to the current point.  A character deletion then extends the gap by moving the gap pointer back by 1 OR the gap is reduced by 1 when a character is inserted.  The Buffer-Gap technique is elegant and significantly reduces the amount of code required to load a file, modify it and redraw the display.  The proof of this is seen when you consider that Atto supports almost the same command set that Pico supports,  but Pico requires almost 17 times the amount of code.

## Comparisons with Other Emacs Implementations

    Editor         Binary   BinSize     KLOC  Files

    zepto          zepto      25962     1.052     9
    atto           atto       33002     1.9k     12
    pEmacs         pe         59465     5.7K     16
    Esatz-Emacs    ee         59050     5.7K     14
    GNOME          GNOME      55922     9.8k     13
    Zile           zile      257360    11.7k     48
    Mg             mg        585313    16.5K     50
    uEmacs/Pk      em        147546    17.5K     34
    Pico           pico      438534    24.0k     29
    Nano           nano      192008    24.8K     17
    jove           jove      248824    34.7k     94
    Qemacs         qe        379968    36.9k     59
    ue3.10         uemacs    171664    52.4K     16
    GNUEmacs       emacs   14632920   358.0k    186

## Starting Zepto
Zepto can only open one file at a time.  The filename to edit must be specified on the command line.

    $ zepto filename

## Zepto Key Bindings
    C-A   begining-of-line
    C-B   backward-character
    C-D   delete-char
    C-E   end-of-line
    C-F   forward Character
    C-G	  Abort (at prompts)
    C-H   backspace
    C-I   handle-tab
    C-J   newline
    C-K   kill-to-eol
    C-L   refresh display
    C-M   Carrage Return
    C-N   next line
    C-P   previous line
    C-S   search-forwards
    C-V   Page Down
    C-X   CTRL-X command prefix

    esc-<   Start of file
    esc->   End of file
    esc-v   Page Up

    ^X^C  Exit. Any unsaved files will require confirmation.
    ^X^S  Save current buffer to disk, using the buffer's filename as the name of

    Home  Beginning-of-line
    End   End-of-line
    Del   Delete character under cursor
    Ins   Toggle Overwrite Mode
    Left  Move left
    Right Move point right
    Up    Move to the previous line
    Down  Move to the next line
    Backspace delete caharacter on the left

### Searching
    C-S enters the search prompt, where you type the search string
    BACKSPACE - will reduce the search string, any other character will extend it
    C-S at the search prompt will search forward, will wrap at end of the buffer
    ESC will escape from the search prompt and return to the point of the match
    C-G abort the search and return to point before the search started

### Copying and moving
    C-<spacebar> Set mark at current position
    ^W     Delete region
    ^Y     Yank back kill buffer at cursor
    esc-w  Copy Region
    esc-k  Kill Region

A region is defined as the area between this mark and the current cursor position. The kill buffer is the text which has been most recently deleted or copied.

Generally, the procedure for copying or moving text is:
1. Mark out region using M-<spacebar> at the beginning and move the cursor to the end.
2. Delete it (with ^W) or copy it (with M-W) into the kill buffer.
3. Move the cursor to the desired location and yank it back (with ^Y).

## Copying
  Zepto code is released to the public domain.
  hughbarney AT gmail.com 2017

## References
    [1] Perfect Emacs - https://github.com/hughbarney/pEmacs
    [2] Anthony's Editor - https://github.com/hughbarney/Anthony-s-Editor
    [3] MG - https://github.com/rzalamena/mg
    [4] Jonathan Payne, Buffer-Gap: http://ned.rubyforge.org/doc/buffer-gap.txt
    [5] Anthony Howe,  http://ned.rubyforge.org/doc/editor-101.txt
    [6] Anthony Howe, http://ned.rubyforge.org/doc/editor-102.txt

