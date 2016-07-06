// A visual key represents the visual key for rendering of the keyboard layout
typedef struct {
  int id;       // unique key number used for assigning logical key layout
  int row;      // row of key from the top
  float size;   // factor of the width of a key, where 1 is a square key
  int x;        // automatic
  int y;        // automatic
  int width;    // automatic
  int height;   // automatic
  char *text;
  int keycode;
  char *shift_text;
  int shift_keycode;
} i3k_key;

int key_margin = 20;
int key_size = 80;
char *key_font = "-misc-droid sans-medium-r-normal--0-0-0-0-p-0-iso10646-1";

static i3k_key keys[] = {
  { .id = 1, .row = 0, .size = 1, .text = "Q", .keycode = XK_Q },
  { .id = 2, .row = 0, .size = 1, .text = "W", .keycode = XK_W },
  { .id = 3, .row = 0, .size = 1, .text = "E", .keycode = XK_E },
};
