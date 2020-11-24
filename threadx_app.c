#include <stdio.h>

// ThreadX and GUIX headers
#include "tx_api.h"
#include "gx_api.h"
#include "flipdot_guix_resources.h"
#include "flipdot_guix_specifications.h"
#include "flipdot_graphics_driver.h"

// MT3620 gpio resources header
#include "GPIO.h"

// Heartbeat LED timer resources
static bool     led_on = false;
// static int counter = 0;
TX_TIMER        my_timer;
#define         LED_1_GREEN_GPIO 9
static bool     ticker_on = false;

// Timer for ticker refresh
#define         CLOCK_TIMER         20

// GuiX resources
#define         GUIX_THREAD_STACK_SIZE 4096
#define         GUIX_THREAD_PRIORITY   4

GX_WINDOW_ROOT  *root;
TX_THREAD       guix_thread;
UCHAR           guix_thread_stack[GUIX_THREAD_STACK_SIZE];

// Timer used for device heartbeat
void timer_expired(ULONG param)
{
    GPIO_Write(LED_1_GREEN_GPIO, led_on);
    led_on = !led_on;
}

// GUIX main thread
VOID guix_thread_entry(ULONG thread_input)
{
    GX_WINDOW_ROOT *root;

    /* Initialize GUIX.  */
    gx_system_initialize();
    
    /* Setup graphics-related hardware and create the display. */
    gx_studio_display_configure(DISPLAY, flipdot_graphics_driver_setup, LANGUAGE_ENGLISH, DISPLAY_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

// Main window event processing
UINT main_event_process(GX_WINDOW *window, GX_EVENT *event_ptr) {
    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_SHOW:

            // Start a timer to update text at regular intervals
            gx_system_timer_start((GX_WIDGET *)window, CLOCK_TIMER, TX_TIMER_TICKS_PER_SECOND/2,TX_TIMER_TICKS_PER_SECOND/2);
            /* Call default event process. */
            gx_window_event_process(window, event_ptr);

            break;

        case GX_EVENT_TIMER:
            if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_TIMER)
            {
                gx_multi_line_text_view_text_set(&main_window.main_window_text_view, ticker_on?"Hello World .":"Hello World");
                ticker_on = !ticker_on;
            }
            else if (event_ptr->gx_event_payload.gx_event_timer_id == 333) {
            }
            break;
        default:
            return gx_window_event_process(window, event_ptr);
    }
}

// ThreadX app main entry point
int main(void)
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();

    return 0;
}


// ThreadX application setup
void    tx_application_define(void *first_unused_memory)
{
    // Create the GUI thread.
    UINT status = tx_thread_create(&guix_thread, 
            "GUIX Thread", 
            guix_thread_entry, 
            0, 
            guix_thread_stack,
            GUIX_THREAD_STACK_SIZE,
            GUIX_THREAD_PRIORITY,
            GUIX_THREAD_PRIORITY, 
            TX_NO_TIME_SLICE,
            TX_AUTO_START);

    if (status != TX_SUCCESS)
    {
        printf("GUI Init failed, please restart\r\n");

    }

    // Initialize the LED heartbeat timer
    GPIO_ConfigurePinForOutput (LED_1_GREEN_GPIO);
    tx_timer_create(&my_timer, "myTimer", timer_expired, 0, TX_TIMER_TICKS_PER_SECOND, TX_TIMER_TICKS_PER_SECOND, TX_AUTO_ACTIVATE);
}
