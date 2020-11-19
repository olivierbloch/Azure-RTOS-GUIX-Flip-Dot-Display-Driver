#include "flipdot_graphics_driver.h"

#include "mt3620_lib/UART.h"
#include "mt3620_lib/Print.h"

typedef unsigned char byte;

// 
void send_to_flipdot(byte *messageToSend, unsigned int bytesToSend);
void map_logical_display_to_physical_display(void);
void send_to_display(void);

#define FLIPDOT_UART MT3620_UNIT_ISU0
UART *driver = NULL;

typedef struct {
	byte frameStart;
	byte command;
	byte address;
	byte data[28];
	byte frameEnd;
} flipFrame;

const static byte cmd_sendToDisplay = 0x83;

byte bFlipDisp[56];  // the logical display buffer.
byte bOutBuffer[56];  // the vertical stripe reverse horizontal flip-disc display buffer.

							 // need to shift
const unsigned int mask[] = { 128,64,32,16,8,4,2,1 };

const unsigned int getMask[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
const unsigned int setMask[] = { 64, 32, 16, 8, 4, 2, 1 };

flipFrame frame;
int PixelPointer = 0;
int PixelLength = 0;

static void flipdot_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
	// Copy canvas to logical diplay buffer
	memcpy(bFlipDisp, canvas->gx_canvas_memory, 56);

	// Map to physical display
	map_logical_display_to_physical_display();

	// Send to flipdot
	send_to_display();
}

UINT flipdot_graphics_driver_setup(GX_DISPLAY *display)
{
    // Init frame buffer
	memset(bFlipDisp, 0x00, 56);

	// setup the basic display frame
	frame.frameStart = 0x80;
	frame.command = cmd_sendToDisplay;
	frame.frameEnd = 0x8f;
	frame.address = 0x00; // top display

	PixelPointer = 0;	// point to first pixel of the array.
	PixelLength = 0;	// no string (yet).

    // Init serial port
	driver = UART_Open(FLIPDOT_UART, 57600, UART_PARITY_NONE, 1, NULL);

    // perform standard function pointer setup
    _gx_display_driver_monochrome_setup(display, GX_NULL, flipdot_buffer_toggle);

	return GX_SUCCESS;
}

void send_to_display(void)
{
	// top display
	frame.address = 0x00;
	// copy the top display data.
	memcpy(frame.data, bOutBuffer, 28);
	// write the data
	send_to_flipdot((byte*)&frame, sizeof(frame));

	// bottom display
	frame.address = 0x01;
	// copy the top display data.
	memcpy(frame.data, (bOutBuffer)+28, 28);
	// write the data
	send_to_flipdot((byte*)&frame, sizeof(frame));
}

// map the pixel x,y display to reverse vertical stripes.
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
			bOutBuffer[dispPtr++] = (byte)outByte;
		}
	}
}

void send_to_flipdot(byte *messageToSend, unsigned int bytesToSend)
{
	UART_Write(driver, messageToSend, bytesToSend );
	tx_thread_sleep(3);
}