//g++ -lX11 -Wall -o xledd xledd.cpp

#include <unistd.h>
#include <X11/XKBlib.h>

bool capsLock = false, numLock = false, scrollLock = false;
int capsLed = 1, numLed = 2, scrollLed = 3;
int capsMask = 2, numMask = 16;

Display *dpy;

void set_led (int led, bool led_mode) {
    XKeyboardControl values;
    values.led_mode = led_mode;
    values.led = led;
    XChangeKeyboardControl (dpy, KBLed | KBLedMode, &values);
}

void led_trigger (int led, bool &led_mode, bool led_mode_new) {
    if (led_mode != led_mode_new) {
        led_mode = led_mode_new;
        set_led (led, led_mode);
    }
}

int main() {
    daemon (0, 0);
    dpy = XOpenDisplay (NULL);
    XkbStateRec xkbState;
    set_led (capsLed, capsLock);
    set_led (numLed, numLock);
    set_led (scrollLed, scrollLock);
    while (true) {
        XkbGetState (dpy, XkbUseCoreKbd, &xkbState);
        if ((xkbState.locked_mods & capsMask) == 0) {
            led_trigger (capsLed, capsLock, false);
        } else {
            led_trigger (capsLed, capsLock, true);
        }
        if ((xkbState.locked_mods & numMask) == 0) {
            led_trigger (numLed, numLock, false);
        } else {
            led_trigger (numLed, numLock, true);
        }
        if (xkbState.group == 0) {
            led_trigger (scrollLed, scrollLock, false);
        } else {
            led_trigger (scrollLed, scrollLock, true);
        }
        usleep (100*1000);
    }
    XCloseDisplay(dpy);
    return 0;
}
