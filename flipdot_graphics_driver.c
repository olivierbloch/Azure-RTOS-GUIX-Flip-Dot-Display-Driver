/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "flipdot_graphics_driver.h"
#include "UART.h"

// Serial port stuff
#define FLIPDOT_UART MT3620_UNIT_ISU0
UART *driver = NULL;

// Flipdot hardware stuff
typedef struct {
	unsigned char frameStart;
	unsigned char command;
	unsigned char address;
	unsigned char data[28];
	unsigned char frameEnd;
} flipFrame;

flipFrame frame;

const static unsigned char cmd_sendToDisplay = 0x83;

unsigned char bFlipDisp[56];  // the logical display buffer.
unsigned char bOutBuffer[56];  // the vertical stripe reverse horizontal flip-disc display buffer (aka format expected by FlipDot controller).

const unsigned int mask[] = { 128,64,32,16,8,4,2,1 }; // Mask used to shift data
const unsigned int getMask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
const unsigned int setMask[] = { 64, 32, 16, 8, 4, 2, 1 };

// Utility function to send frame to Flipdot over serial connection
void send_to_flipdot(unsigned char *messageToSend, unsigned int bytesToSend)
{
	UART_Write(driver, messageToSend, bytesToSend );
	tx_thread_sleep(3);
}

// map the pixel x,y display to reverse vertical stripes (format expected by the Flipdot controller).
void map_logical_display_to_physical_display(void)
{
	int dispPtr = 0;
	int tray = 0;
	// do this in two sections since display 0 and 1 are each 28 bytes.
	for (tray = 0; tray < 2; tray++)
	{
		int x = 0;
		// now vertical stripe the pixels for each panel.
		for (x = 27; x > -1; x--)    // walk the map backwards.
		{
			unsigned int outByte = 0x00; // the vertical stripe
			int y = 0;
			for (y = 0; y < 7; y++)
			{
				int bytePos = x / 8; // get the byte offset for this stripe
				int bitPos = x % 8; // get the bit offset for this stripe
				int buffPos = (y * 4) + bytePos;
				unsigned int bByte = bFlipDisp[(28 * tray) + buffPos];
				unsigned int iSet = bByte & getMask[bitPos];
				if (iSet != 0)
				{
					outByte |= setMask[y];
				}
			}
			bOutBuffer[dispPtr++] = (unsigned char)outByte;
		}
	}
}

// Main buffer toggle function for the driver
static void flipdot_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
	// Copy canvas to logical diplay buffer
	memcpy(bFlipDisp, canvas->gx_canvas_memory, 56);

	// Map to physical display
	map_logical_display_to_physical_display();

	// Send to flipdot
	// top display
	frame.address = 0x00;
	// copy the top display data.
	memcpy(frame.data, bOutBuffer, 28);
	// write the data
//	send_to_flipdot((unsigned char*)&frame, sizeof(frame));
	UART_Write(driver, (unsigned char*)&frame, sizeof(frame) );
	tx_thread_sleep(3);

	// bottom display
	frame.address = 0x01;
	// copy the top display data.
	memcpy(frame.data, (bOutBuffer)+28, 28);
	// write the data
//	send_to_flipdot((unsigned char*)&frame, sizeof(frame));
	UART_Write(driver, (unsigned char*)&frame, sizeof(frame) );
}

// Driver setup
UINT flipdot_graphics_driver_setup(GX_DISPLAY *display)
{
    // Init serial port
	driver = UART_Open(FLIPDOT_UART, 57600, UART_PARITY_NONE, 1, NULL);

    // Init frame buffer
	memset(bFlipDisp, 0x00, 56);

	// setup the basic display frame
	frame.frameStart = 0x80;
	frame.command = cmd_sendToDisplay;
	frame.frameEnd = 0x8f;
	frame.address = 0x00; // top display

    // perform standard function pointer setup
    _gx_display_driver_monochrome_setup(display, GX_NULL, flipdot_buffer_toggle);

	return GX_SUCCESS;
}



