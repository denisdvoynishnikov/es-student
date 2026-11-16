#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"
 
extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

// memory.c

static void title(void)
{
    printf("%-10s %-10s %-10s %-10s\n", "area", "start", "end", "size");
}

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n", name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

#define XIP_SIZE _u(0x200000)
#define ROM_SIZE _u(0x4000)

void mem_info(void)
{
    // шапка таблицы: область, начало, конец, размер
    title();
    // flash — XIP_BASE и PICO_FLASH_SIZE_BYTES
    row("flash", XIP_BASE, XIP_BASE + XIP_SIZE);
    // sram — базовый адрес из SDK, размер из datasheet
    row("sram", SRAM_BASE, SRAM_END);
    // rom — базовый адрес из SDK, размер из datasheet
    row("rom", ROM_BASE, ROM_BASE + ROM_SIZE);

    // image — от __flash_binary_start до __flash_binary_end
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    // free  — от __flash_binary_end до конца флеш-памяти
    row("free", (uintptr_t)&__flash_binary_end, XIP_BASE + XIP_SIZE);
    // boot2 — от __boot2_start__ до __boot2_end__
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    // text  — от __boot2_end__ до __etext: код и константы
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);

    // data flash — хранение .data, от __etext, длиной с .data
    row("data flash", (uintptr_t)&__etext, (uintptr_t)&__etext + (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__);
    // data ram   — работа .data, от __data_start__ до __data_end__
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    // bss        — от __bss_start__ до __bss_end__
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    // heap       — от __bss_end__ до __HeapLimit
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    // stack      — от __StackBottom до __StackTop
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);
    
    // шапка
    printf("%-10s\n", "total");

    // итог: образ во флеш и из чего он сложился
    printf("  flash image %9u = boot2 %u + text %u + data %u\n",
        (uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start,
        (uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__,
        (uintptr_t)&__etext - (uintptr_t)&__boot2_end__,
        (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__
    );
    // итог: свободно во флеш-памяти из всего её объёма
    printf("  flash free  %9u of %u\n", 
        XIP_BASE + XIP_SIZE - (uintptr_t)&__flash_binary_end,
        XIP_SIZE
    );
    // итог: занято в ОЗУ — .data и .bss
    printf("  ram used   %9u = data %u + bss %u\n", 
          (uintptr_t)&__data_end__ -  (uintptr_t)&__data_start__ +  (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__,
          (uintptr_t)&__data_end__ -  (uintptr_t)&__data_start__,
          (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__
    );
    // итог: свободно в ОЗУ — под кучу и под стек
    printf("  ram free   %9u for heap and %u for stack\n", 
        (uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__,
        (uintptr_t)&__StackTop - (uintptr_t)&__StackBottom
    );
}