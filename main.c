#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include "i3keys.h"

Display *display;
Window window;
int screen;
// Dimensions
int height;
int width;
int total_keyboard_width;
int total_keyboard_height;
// Colors
Colormap colormap;
XColor color_background;
XColor color_focused;
XColor color_focused_text;
XColor color_unfocused;
XColor color_unfocused_text;

// Create an XColor from a hex code such as #1B1D1E
static XColor color_from_hex(const char *code) {
  XColor color;
  XParseColor(display, colormap, code, &color);
  XAllocColor(display, colormap, &color);

  return color;
}

// Calculate the pixel key width and heights, x and y from the abstract
// key layout definition. Further centers the keyboard in the middle of the screen
static void calculate_layout_dimensions() {
  int x = 0;
  int y = 0;
  int last_row = -1;
  int max_rows = 0;
  int keyboard_offset_from_left = 0;

  i3k_key *key = keys;
  i3k_key *keyEnd = keys + sizeof(keys)/sizeof(keys[0]);
  while(key < keyEnd) {

    // Calculate starting x
    if(last_row != key->row) {
      x = 0;
      max_rows++;
    }
    last_row = key->row;
    x += key_margin;

    // Calculate starting y
    y = key_margin + key->row * (key_margin + key_size);
    
    // Set values to key layout
    key->x = x;
    key->y = y;
    key->width = key->size * key_size + (key->size-1) * key_margin;
    key->height = key_size;

    // Set next X starting point
    x += key->width;

    // Track maximum total keyboard width for positioning it in the middle
    if(x > total_keyboard_width) {
      total_keyboard_width = x;
    }
    
    key++;
  }

  // Total keyboard height based on rows
  total_keyboard_height = max_rows * (key_margin + key_size) + key_margin;

  // Calculate offset from left to center keyboard
  keyboard_offset_from_left = (width -total_keyboard_width) / 2;
  
  // Apply offset to center the keyboard
  key = keys;
  while(key < keyEnd) {
    key->x = key->x + keyboard_offset_from_left;
    key++;
  }

}

// Renders each key based on its position and modifier status
static void render_keys() {
  // Set foreground color for keys
  XSetForeground(display, DefaultGC(display, screen), color_unfocused.pixel);

  // Draw key backgrounds
  i3k_key *key = keys;
  i3k_key *keyEnd = keys + sizeof(keys)/sizeof(keys[0]);
  while(key < keyEnd) {
    XFillRectangle(display, window, DefaultGC(display, screen), key->x, key->y, key->width, key->height);
    key++;
  }
}

// Expose is called to draw and redraw
static void expose(XEvent *event) {
  render_keys();
  printf("Expose\n");
}

// Send a key event where the type is KeyPress or KeyRelease, keycodes are defined in X11/keysym, e.g. XK_g
// and the keymask defines the modifier states, e.g. ShiftMask or LockMask or ControlMask
static void send_key(int type, int keycode, int keymask) {

  // Select target window from current focus
  Window focus;
  int revert;
  XGetInputFocus(display, &focus, &revert);

  // Create key event
  XKeyEvent event;
  event.display     = display;
  event.window      = focus;
  event.root        = window;
  event.subwindow   = None;
  event.time        = CurrentTime;
  event.x           = 1;
  event.y           = 1;
  event.x_root      = 1;
  event.y_root      = 1;
  event.same_screen = True;
  event.keycode     = XKeysymToKeycode(display, keycode);
  event.state       = keymask;
  event.type        = type;

  // Send event
  long event_mask = type == KeyPress ? KeyPressMask : KeyReleaseMask;
  XSendEvent(event.display, event.window, True, event_mask, (XEvent *)&event);
}

// button press is called on touch or click down
static void button_press(XButtonEvent *event) {
  printf("Mouse down in %d, %d\n", event->x, event->y);

  send_key(KeyPress, XK_g, None);
}

// button release is called on touch up or release
static void button_release(XButtonEvent *event) {
  printf("Mouse up in %d, %d\n", event->x, event->y);
  
  send_key(KeyRelease, XK_g, None);
}

// Clear and redraw full contents
static void reexpose(){
  XClearWindow(display, window);
  expose(NULL);
}


// Main
int main() {

  // Open connection to X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  screen = DefaultScreen(display);
  colormap = DefaultColormap(display, 0);

  // Setup Colors
  color_background = color_from_hex("#1B1D1E");
  color_unfocused = color_from_hex("#303030");
  color_unfocused_text = color_from_hex("#ffffff");
  color_focused = color_from_hex("#4c7899");
  color_focused_text = color_from_hex("#4c7899");

  // Width is the full window width
  width = DisplayWidth(display, screen);

  // Calculate keyboard dimensions and layout
  calculate_layout_dimensions();

  // Create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, total_keyboard_height, 0, None, color_background.pixel);

  // Set window type to DOCK
  Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Select kind of events we want to receive
  XSelectInput(display, window, ExposureMask | ButtonPressMask | ButtonReleaseMask);

  // Show the window
  XMapWindow(display, window);
  XFlush(display);

  // Event loop
  XEvent event;
  while(1) {
    XNextEvent(display, &event);

    // Draw or redraw window
    if(event.type == Expose) {
      expose(&event);
      continue;
    }
    
    // Handle mouse button press
    if(event.type == ButtonPress) {
      button_press((XButtonEvent*)&event);
      continue;
    }
    
    // Handle mouse button release
    if(event.type == ButtonRelease) {
      button_release((XButtonEvent*)&event);
      continue;
    }
  }

  // Close connection to X server
  XCloseDisplay(display);

  return 0;
}
