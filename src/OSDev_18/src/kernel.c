#include <libc/stdint.h>
#include <kernel/terminal.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/interrupt.h>
#include <kernel/keyboard.h>
#include <kernel/memory.h>
#include <kernel/pit.h>
#include <songApp/song.h>
#include <songApp/frequencies.h>

extern uint32_t end;

void PlayMusic(void) {
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        {music_2, sizeof(music_2) / sizeof(Note)},
        {music_3, sizeof(music_3) / sizeof(Note)},
        {music_4, sizeof(music_4) / sizeof(Note)},
        {music_5, sizeof(music_5) / sizeof(Note)},
        {music_6, sizeof(music_6) / sizeof(Note)}
    };

    uint32_t songCount = sizeof(songs) / sizeof(Song);

    SongPlayer* player = CreateSongPlayer();

    if (!player) {
        TerminalWriteString("Failed to create SongPlayer.\n");
        return;
    }

    while(1) {
        for(uint32_t i = 0; i < songCount; i++) {
            player->play_song(&songs[i]);
            SleepInterrupt(1000);
        }
    }
}

void main(void) {
    TerminalInitialize();
    GdtInitialize();
    IdtInitialize();
    PitInitialize();

    RegisterInterruptHandler(IRQ1, KeyboardHandler);

    InitKernelMemory(&end);
    InitPaging();
    PrintMemoryLayout();

    /*

    void* memory1 = malloc(48261);
    void* memory2 = malloc(27261);
    void* memory3 = malloc(12617);

    TerminalWriteString("memory1 = ");
    TerminalWriteHex((uint32_t)memory1);
    TerminalWriteString("\n");

    TerminalWriteString("memory2 = ");
    TerminalWriteHex((uint32_t)memory2);
    TerminalWriteString("\n");

    TerminalWriteString("memory3 = ");
    TerminalWriteHex((uint32_t)memory3);
    TerminalWriteString("\n");

    free(memory2);

    void* memory4 = malloc(1000);

    TerminalWriteString("memory4 = ");
    TerminalWriteHex((uint32_t)memory4);
    TerminalWriteString("\n");

    SleepTest();

    */

    PlayMusic();

    for (;;) {
        __asm__ volatile("hlt");
    }
}