// mouse.c

#include "types.h"
#include "defs.h"
#include "x86.h"
#include "mouse.h"
#include "traps.h"

// Wait until the mouse controller is ready for us to send a packet
void 
mousewait_send(void) 
{
    while ((inb(MSSTATP) & 0x02) != 0); // keep checking until bit 1 of status port becomes clear
}

// Wait until the mouse controller has data for us to receive
void 
mousewait_recv(void) 
{
    while ((inb(MSSTATP) & 0x01) == 0); // keep checking until bit 0 of status port becomes set
}

// Send a one-byte command to the mouse controller, and wait for it
// to be properly acknowledged
void 
mousecmd(uchar cmd) 
{
    mousewait_send();
    outb(MSSTATP, PS2MS); // the next byte is for the mouse
    mousewait_send();
    outb(MSDATAP, cmd);   // send the command byte to data port
    mousewait_recv();
    inb(MSDATAP); // receive the acknowledgement
}

void
mouseinit(void)
{
    mousewait_send(); // Wait until the controller can receive a control packet.
    outb(MSSTATP, 0xA8); // tells the PS/2 controller to enable the mouse.
    mousewait_send();
    outb(MSSTATP, 0x20); // selects the Compaq Status byte as the data we want to retrieve.
    mousewait_recv();
    uchar sb = inb(MSDATAP); // read status byte
    sb = sb | 0x02; // specifies that interrupts should be enabled.
    mousewait_send();
    outb(MSSTATP, MSDATAP); // tells the controller we are about to send it the modified status byte.
    mousewait_send();
    outb(MSDATAP, sb); // send modified status byte
    mousecmd(0xF6); // selects "default settings" for the mouse
    mousecmd(0xF4); // tells the mouse to activate and start sending us interrupts.
    ioapicenable(12, 0); // receive the mouse interrupt (IRQ12) on CPU 0
    cprintf("Mouse has been initialized\n");
}

void
mouseintr(void)
{
    mousewait_recv();
    while(inb(0x64) & 0x01){
        // receive the first byte and print accordingly
        uchar b = inb(MSDATAP);
        if((b & 0x01)){
            cprintf("LEFT\n"); // if bit 0 Left button was clicked
        }
        else if((b & 0x02)){
            cprintf("RIGHT\n"); // if bit 1 Right button was clicked
        }
        else if((b & 0x04)){
            cprintf("MID\n"); // if bit 2 MID button was clicked
        }
        // there are more meanings for bit but we just need to handle basic cases
        // ignore if bit is otherwise

        // accept 2 more remaining bytes from the mouse
        for(int i=0; i<2; i++){
            mousewait_recv();
            inb(MSDATAP);
        }
    }
    
}
