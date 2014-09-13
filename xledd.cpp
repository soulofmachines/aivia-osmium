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
        switch (xkbState.locked_mods & capsMask) {
        case 0:
            led_trigger (capsLed, capsLock, false);
            break;
        default:
            led_trigger (capsLed, capsLock, true);
            break;
        }
        switch (xkbState.locked_mods & numMask) {
        case 0:
            led_trigger (numLed, numLock, false);
            break;
        default:
            led_trigger (numLed, numLock, true);
            break;
        }
        switch (xkbState.group) {
        case 0:
            led_trigger (scrollLed, scrollLock, false);
            break;
        default:
            led_trigger (scrollLed, scrollLock, true);
            break;
        }
        nanosleep (&ts, NULL);
    }
    XCloseDisplay(dpy);
    return 0;
}
