/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "CPUFreq.h"
#include "VectorTable.h"

extern int main(void);

_Noreturn void RTCoreMain(void)
{
    // Init Vector Table
    VectorTableInit();

    // Set CPU clock
    CPUFreq_Set(197600000);

    // Invoke ThreadX main
    main();

    for (;;) {
		__asm__("wfi");
    }
}
