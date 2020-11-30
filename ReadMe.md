# Azure RTOS GUIX driver for AlfaZeta Flip-Dot boards XY5 on Azure Sphere MT3620 development kit

This repo contains a sample GUIX display driver for a the 28x14 Flip-Dot panel from AlfaZeta ([Flip-Dot boards XY5](https://flipdots.com/en/products-services/flip-dot-boards-xy5/)) to be used on an Azure Sphere MT3620 development kit. The repository also contains a simple emulator in the form of a Windows application that allows testing the display driver without the actual Flip-Dot hardware.

## Prerequisites

### Hardware

#### Azure Sphere MT3620 develpoment kit

In order to follow this tutorial along you will need an [Azure Sphere MT3620 development kit](https://www.mediatek.com/products/AIoT/mt3620) from MediaTek. Note that you can certainly try this out on some other board that supports Azure RTOS ThreadX.

![Azure Sphere MT3620 development kit](Assets/MT3620.jpg)

#### Flip-Dot panel

The Flip-Dot panel used is from the Polish company [AlfaZeta](https://flipdots.com/en/home/) and is a 28x14 magnetic flip dots panel called [Flip-Dot Boards XY5](https://flipdots.com/en/products-services/flip-dot-boards-xy5/).

![Flip-dot panel with MT3620](Assets/Flipdot-and-MT3620.jpg)

The Flip-Dot panel is controlled via a wire protocol over RS485, and you need to connect the board's GND , 5V and TXD0 pins to the Flip-Dot controller as indicated in the hardware documentation. Here is the MT3620 pinout for reference:

![MT3620 pintout](Assets/MT3620-pinout.png)

#### ... Or a Serial-USB adapter to use the Flip-Dot panel emulator

If you don't have the chance to have one of these panels at hand, there is an emulator in the form of a Windows application that you can use connecting the MT3620 to your PC using a Serial-USB adapter. This allows testing the exact same code without the actual panel.

![FLip-Dot emulator](Assets/emulator.jpg)

In order to use the Flip-Dot emulator, get it from [Mike Hall's GitHub repo](https://github.com/mikehall-ms/flipdot-emulator). To send commands to the emulator from the device serial port, you will need a **Serial to USB adapter**. We recommend using one that uses an FTDI chip as these are way less problematic if you are running Windows 10 on your development machine. Here is a [good one from DSD Tech](https://www.amazon.com/gp/product/B07BBPX8B8/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1) which works perfectly and costs less than $10:

![DSD Tech Serial-USB adapter](Assets/Serial-USB-adapter.jpg)

Connect the MT3620 board's GND and TXD0 pins to the GND and RXD pins of the adapter.

### Software

In order to develop for Azure Sphere and deploy an Azure RTOS application with a GUIX UI you will need a PC running Windows or Linux and install Visual Studio 2019 or Visual Studio Code and the Azure Sphere SDK.

- Azure Sphere development setup: Follow the instructions in the [QuickStart](https://docs.microsoft.com/azure-sphere/install/overview) 
- Real Time application development setup: Follow the instructions in this [tutorial](https://docs.microsoft.com/azure-sphere/install/qs-real-time-application)
- Azure RTOS GUIX Studio (for Windows only): If you want to customize the UI, follow the instructions in the [GUIX GitHub repository](https://github.com/azure-rtos/guix) to install the tool.
- In order to clone this repository, you can do it directly from Visual Studio or Visual Studio code, or install [Git](https://git-scm.com/downloads) on your machine.

## Clone-Deploy-Run

The current repository contains all the code you need to just deploy and run.
Make sure you went through the hardware and software requirements above:

- Software:
  - You have Visual Studio or VS Code installed as well as the Azure Sphere SDK
  - You went through the Real Time application development setup
  - You have [Git](https://git-scm.com/downloads) installed on your machine
- Hardware
  - You connected the MT3620 board serial port to the Flip-Dot panel as instructed in the panel docs
  - OR... You connected the MT3620 board serial port to your Windows PC through the Serial-USB adapter, got the Flip-Dot emulator, run it and select the Serial port corresponding to your serial-USB adapter port (you can find which port it is running the command ```mode``` in a command prompt or looking in the Device Manager).

To clone the repository and its submodules, type the following command (it is assuming you have git installed on your machine)

```bash
git clone https://github.com/olivierbloch/Azure-RTOS-GUIX-Flip-Dot-Display-Driver --recursive
```

Then compile and deploy the application hitting ```F5```

## Do it all from scratch

If you want to do it all from scratch, you can follow the steps in [this blog post](https://aka.ms/olivier.blog/my-first-guix-driver).
