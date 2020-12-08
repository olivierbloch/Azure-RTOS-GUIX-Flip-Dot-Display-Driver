/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

// Standard headers
#include <stdio.h>
#include <stdbool.h>

// ThreadX and GUIX headers
#include "tx_api.h"
#include "gx_api.h"
#include "flipdot_guix_resources.h"
#include "flipdot_guix_specifications.h"
#include "flipdot_graphics_driver.h"

// Timer for ticker refresh
#define         CLOCK_TIMER         20
static bool  ticker_on = false;

// GuiX resources
#define         GUIX_THREAD_STACK_SIZE 4096
#define         GUIX_THREAD_PRIORITY   4

GX_WINDOW_ROOT  *root;
TX_THREAD       guix_thread;
UCHAR           guix_thread_stack[GUIX_THREAD_STACK_SIZE];

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
            // Call default event process
            return gx_window_event_process(window, event_ptr);

        case GX_EVENT_TIMER:
            // If the timer id is our clock timer, change what's on the display
            if (event_ptr->gx_event_payload.gx_event_timer_id == CLOCK_TIMER)
            {
                gx_multi_line_text_view_text_set(&main_window.main_window_text_view, ticker_on?"Hello World .":"Hello World");
                ticker_on = !ticker_on;
            }
            break;

        default:
            return gx_window_event_process(window, event_ptr);
    }
    return GX_SUCCESS;
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
}
