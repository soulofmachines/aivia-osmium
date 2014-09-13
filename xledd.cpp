//g++ -lX11 -Wall -o xledd xledd.cpp

#include <X11/XKBlib.h>
#include <time.h>

Display *dpy;

bool capsLock = false, numLock = false, scrollLock = false;
int capsLed = 1, numLed = 2, scrollLed = 3;
int capsMask = 2, numMask = 16;

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
    dpy = XOpenDisplay (NULL);
    XkbStateRec xkbState;
    struct timespec ts = {0, 100000000};
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
        nanosleep (&ts, NULL);
    }
    XCloseDisplay(dpy);
    return 0;
}
