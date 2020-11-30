// Licensed under the MIT License.

// Use this app to test the device and SDK installation succeeded, and that you
// can deploy and debug applications on the real-time core.

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "CPUFreq.h"
#include "VectorTable.h"
#include "NVIC.h"

int main(void);

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
