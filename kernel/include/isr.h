#ifndef OS_ISR_H
#define OS_ISR_H

#include "system.h"

// ISR function type
typedef void (*isr)(Register * );

extern isr interruptHandlers[256];

// Exception handler
void finalExceptionHandler(Register reg);

//irq handler
void final_irq_handler(Register  * reg);

void registerInterruptHandler(int num, isr handler);

// exception helpers
extern void exception0();
extern void exception1();
extern void exception2();
extern void exception3();
extern void exception4();
extern void exception5();
extern void exception6();
extern void exception7();
extern void exception8();
extern void exception9();
extern void exception10();
extern void exception11();
extern void exception12();
extern void exception13();
extern void exception14();
extern void exception15();
extern void exception16();
extern void exception17();
extern void exception18();
extern void exception19();
extern void exception20();
extern void exception21();
extern void exception22();
extern void exception23();
extern void exception24();
extern void exception25();
extern void exception26();
extern void exception27();
extern void exception28();
extern void exception29();
extern void exception30();
extern void exception31();
extern void exception128();

// isr helpers
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif //OS_ISR_H
