#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "i3keys.h"

// X11
Display *display;
Window window;
int screen;

// Dimensions
int height;
int width;
int total_keyboard_width;
int total_keyboard_height;

// Colors
GC gc;
Colormap colormap;
XColor color_background;
XColor color_focused;
XColor color_focused_text;
XColor color_unfocused;
XColor color_unfocused_text;

// Font
XFontStruct *font;
int font_height;

// Switch
int switch_height = 30;

// State
i3k_key *last_key;
i3k_key *modifier_key;
int modifier_mask;
int open = 1;

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
    y = switch_height + key_margin + key->row * (key_margin + key_size);

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
  total_keyboard_height = max_rows * (key_margin + key_size) + key_margin + switch_height;

  // Calculate offset from left to center keyboard
  keyboard_offset_from_left = (width - (total_keyboard_width + key_margin)) / 2;

  // Apply offset to center the keyboard
  key = keys;
  while(key < keyEnd) {
    key->x = key->x + keyboard_offset_from_left;
    key++;
  }

}

// Renders each key based on its position and modifier status
static void render_keys() {
  // Draw keys
  i3k_key *key = keys;
  i3k_key *keyEnd = keys + sizeof(keys)/sizeof(keys[0]);
  while(key < keyEnd) {

    // Draw key background
    XColor key_color = key->is_pressed ? color_focused : color_unfocused;
    XSetForeground(display, gc, key_color.pixel);
    XFillRectangle(display, window, gc, key->x, key->y, key->width, key->height);

    // Draw text
    XColor font_color = key->is_pressed ? color_focused_text : color_unfocused_text;
    XSetForeground(display, gc, font_color.pixel);
    char *text = modifier_mask == ShiftMask ? key->shift_text : key->text;
    int text_width = XTextWidth(font, text, strlen(text));
    int x = key->x + ((key->width - text_width) / 2);
    int y = key->y + ((key->height + font_height) / 2);
    XDrawString(display, window, gc, x, y, text, strlen(text));

    key++;
  }
}

// Renders the switch to open/close the keyboard
static void render_switch() {
    XSetForeground(display, gc, color_focused.pixel);
    XFillRectangle(display, window, gc, 0, 0, width, switch_height);
}

// Try to find key from button event
static i3k_key* find_key_from_button(XButtonEvent *event) {
  i3k_key *key = keys;
  i3k_key *keyEnd = keys + sizeof(keys)/sizeof(keys[0]);
  while(key < keyEnd) {
    if(
        event->x >= key->x &&
        event->x <= (key->x + key->width) &&
        event->y >= key->y &&
        event->y <= (key->y + key->height)
      ){
      return key;
    }

    key++;
  }
  return NULL;
}

// Expose is called to draw and redraw
static void expose(XEvent *event) {
  calculate_layout_dimensions();
  render_switch();
  render_keys();
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

// Button press is called on touch or click down
static void button_press(XButtonEvent *event) {
  // Switch
  if(event->y <= switch_height) {
    if(open == 1) {
      open = 0;
      XMoveResizeWindow(display, window, 0, 0, width, switch_height);
    } else {
      open = 1;
      XMoveResizeWindow(display, window, 0, 0, width, total_keyboard_height);
    }
    return;
  }

  // Key
  i3k_key *key = find_key_from_button(event);
  if(key == NULL) {
    return;
  }

  // Handle modifier key press
  if(key->modifier_mask){

    // Deselect potential last modifier key
    if(modifier_key != NULL && modifier_key != key){
      modifier_key->is_pressed = False;
      modifier_mask = None;
      modifier_key = NULL;
    }

    // Select
    if(modifier_mask == None) {
      key->is_pressed = True;
      modifier_mask = key->modifier_mask;
      modifier_key = key;
    } else {
      // Unselect pressed modifier
      key->is_pressed = False;
      modifier_mask = None;
      modifier_key = NULL;
    }

    // Handle normal key press
  } else {
    // Highlight key
    key->is_pressed = True;
    last_key = key;

    // Send press to XServer
    send_key(KeyPress, key->keycode, modifier_mask);

    // Reset modifier
    if(modifier_key != NULL){
      modifier_key->is_pressed = False;
      modifier_mask = None;
    }
  }

  // Rerender
  render_keys();
}

// Button release is called on touch up or release
static void button_release(XButtonEvent *event) {
  i3k_key *key = last_key;
  if(last_key == NULL) {
    return;
  }

  // Unhighlight key
  key->is_pressed = False;

  // Send release to XServer
  send_key(KeyRelease, key->keycode, modifier_mask);
  last_key = NULL;

  // Rerender
  render_keys();
}

// Setup Colors
static void setup_colors(){
  // Colormap
  colormap = DefaultColormap(display, 0);

  // Colors
  color_background = color_from_hex("#1B1D1E");
  color_unfocused = color_from_hex("#303030");
  color_unfocused_text = color_from_hex("#ffffff");
  color_focused = color_from_hex("#00B7FF");
  color_focused_text = color_from_hex("#ffffff");
  
  // Setup Graphics Context
  gc = DefaultGC(display, screen);
}

// Setup Fonts
static void setup_fonts(){
  // Setup Font
  font = XLoadQueryFont(display, key_font);
  if (font == NULL) {
    fprintf(stderr, "Cannot load font %s\n", key_font);
    exit(1);
  }
  XSetFont(display, gc, font->fid);
  font_height = font->ascent + font->descent;
}

// Setup Window
static void setup_window(){
  // Width is the full window width
  width = DisplayWidth(display, screen);

  // Calculate keyboard dimensions and layout for total_keyboard_height
  calculate_layout_dimensions();

  // Create window
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, total_keyboard_height, 0, None, color_background.pixel);
}

// Setup Window Properties
static void setup_window_properties(){
  // Set window name
  Atom type = XInternAtom(display, "_NET_WM_NAME", False);
  Atom value = XInternAtom(display, "i3keys", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Set window type to DOCK
  type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
  XChangeProperty(display, window, type, XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);

  // Position at bottom of window
  unsigned long strut[12];
  memset(&strut, 0, sizeof(strut));
  strut[3] = total_keyboard_height; // bottom
  type = XInternAtom(display, "_NET_WM_STRUT_PARTIAL", False);
  XChangeProperty(display, window, type, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)strut, 12);
  
  // Show the window
  XMapWindow(display, window);
  XFlush(display);
}

// Start Event Loop
static void start_event_loop(){
  // Select kind of events we want to receive
  XSelectInput(display, window, ExposureMask | ButtonPressMask | ButtonReleaseMask);

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

  // Setup
  setup_colors();
  setup_fonts();
  setup_window();
  setup_window_properties();

  // Loop
  start_event_loop();

  // Close connection to X server
  XCloseDisplay(display);

  return 0;
}
