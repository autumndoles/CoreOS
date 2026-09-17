#include <stdint.h>

/*
 * VGA text-mode video memory.
 *
 * Each screen cell is two bytes:
 *   byte 0 = character
 *   byte 1 = color
 */

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static uint8_t color = 0x0F;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void clear_screen(void)
{
    for (uint8_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[y * VGA_WIDTH + x] =
                ((uint16_t)color << 8) | ' ';
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}

static void put_char(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;

        if (cursor_y >= VGA_HEIGHT)
            cursor_y = 0;

        return;
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] =
        ((uint16_t)color << 8) | (uint8_t)c;

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;

        if (cursor_y >= VGA_HEIGHT)
            cursor_y = 0;
    }
}

static void print(const char* text)
{
    while (*text)
    {
        put_char(*text);
        text++;
    }
}

void kernel_main(void)
{
    clear_screen();

    print("========================================\n");
    print("              Welcome to MyOS\n");
    print("========================================\n\n");

    print("Kernel initialized successfully!\n");
    print("CPU: x86\n");
    print("Display: VGA text mode\n");
    print("Kernel language: C\n\n");

    print("MyOS is alive.\n");
    print("> ");

    /*
     * The kernel has nothing else to do yet.
     * Keep the CPU halted rather than executing
     * random memory.
     */
    while (1)
    {
        __asm__ volatile ("hlt");
    }
}
