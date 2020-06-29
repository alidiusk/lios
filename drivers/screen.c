#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include "../libc/string.h"

// Declaration of private functions
int get_cursor_offset();
void set_cursor_offset(int32_t offset);
int print_char(char c, int32_t col, int32_t row, char attr);
int get_offset(int32_t row, int32_t col);
int get_offset_row(int32_t offset);
int get_offset_col(int32_t offset);
void scroll_screen();

// **********************************************************************
// * Public Kernel API Functions                                        *
// **********************************************************************

// Clears the screen with blank characters.
// Sets the cursor at the top right of the screen afterwards.
void clear_screen() {
    uint32_t screen_size = MAX_COLS * MAX_ROWS;
    uint8_t* vidmem = (uint8_t *) VIDEO_ADDRESS;

    for (uint32_t i = 0; i < screen_size; i++) {
        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_offset(0, 0));
}

// Print a message on the specified location
// If col, row are negative, we will use cursor offset
void kprint_at(char* message, int32_t row, int32_t col) {
    uint32_t offset;
    if (row >= 0 && col >= 0) {
        offset = get_offset(row, col);
    } else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    uint32_t i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i], row, col, 0);

        row = get_offset_row(offset);
        col = get_offset_col(offset);
        i += 1;
    }
}

//Prints message at cursor
void kprint(char* message) {
    kprint_at(message, -1, -1);
}

//Prints message at cursor with a newline
void kprintln(char* message) {
    append(message, '\n');
    kprint_at(message, -1, -1);
}

// Deletes the last character from the cursor
void kprint_backspace() {
    int32_t offset = get_cursor_offset() - 2;
    int32_t row = get_offset_row(offset);
    int32_t col = get_offset_col(offset);
    print_char(0x08, row, col, WHITE_ON_BLACK);
}

// **********************************************************************
// * Private Kernel Functions                                           *
// **********************************************************************

// Innermost print function for our kernel, directly accesses video memory.
//
// If 'col' and 'row' are negative, we will print at current cursor location.
// If 'attr' is zero it will use 'WHITE_ON_BLACK' as default.
// Returns the offset of the next character
// Sets the video cursor to the returned offset.
int print_char(char c, int32_t row, int32_t col, char attr) {
    uint8_t* vidmem = (uint8_t *) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    // bounds check; print red 'E' in bottom right cell if error
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2 * MAX_ROWS * MAX_COLS - 2] = 'E';
        vidmem[2 * MAX_ROWS * MAX_COLS - 1] = RED_ON_WHITE;
    }

    uint32_t offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(row, col);
    } else {
        offset = get_cursor_offset();
    }

    if (c == '\n') { // Newline
        // must get row here because we could have changed
        // offset to cursor position
        row = get_offset_row(offset);
        offset = get_offset(row + 1, 0);
    } else if (c == 0x08) { // Backspace
        vidmem[offset] = 0;
        vidmem[offset+1] = attr;
    } else { // 'Normal' characters
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        scroll_screen();
        offset = get_offset(MAX_ROWS - 1, 0);
    }

    set_cursor_offset(offset);
    return offset;
}

void scroll_screen() {
    uint8_t* vidmem = (uint8_t *) VIDEO_ADDRESS;
    uint8_t empty_row[MAX_COLS*2];
    memset(empty_row, 0, MAX_COLS*2);

    memcpy(vidmem, &vidmem[get_offset(1, 0)], 2*(MAX_ROWS - 1)*MAX_COLS);
    memcpy(&vidmem[get_offset(MAX_ROWS - 1, 0)], empty_row, 2*MAX_COLS);

    set_cursor_offset(get_offset(MAX_ROWS, 0));
}

int get_cursor_offset() {
    // upper byte
    port_byte_out(REG_SCREEN_CTRL, 14);
    uint32_t offset = port_byte_in(REG_SCREEN_DATA) << 8;

    // lower byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    // position * size of character cell
    return 2 * offset;
}

void set_cursor_offset(int32_t offset) {
    // similar to 'get_cursor_offset()', but we write instead of read data
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

int get_offset(int32_t row, int32_t col) {
    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int32_t offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int32_t offset) {
    return (offset - (get_offset_row(offset) * MAX_COLS * 2)) / 2;
}
