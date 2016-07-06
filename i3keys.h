// A visual key represents the visual key for rendering of the keyboard layout
typedef struct {
  int id;       // unique key number used for assigning logical key layout
  int row;      // row of key from the top
  float size;   // factor of the width of a key, where 1 is a square key
  int x;        // automatic
  int y;        // automatic
  int width;    // automatic
  int height;   // automatic
} i3k_key;

int key_margin = 20;
int key_size = 80;

static i3k_key keys[] = {
  { .id = 1, .row = 0, .size = 1 },
  { .id = 2, .row = 0, .size = 1 },
  { .id = 3, .row = 0, .size = 1.5 },
  { .id = 4, .row = 0, .size = 0.5 },
  { .id = 5, .row = 0, .size = 1 },
  { .id = 6, .row = 0, .size = 1 },
  { .id = 7, .row = 1, .size = 1 },
  { .id = 8, .row = 1, .size = 1 },
  { .id = 9, .row = 1, .size = 2 },
  { .id = 10, .row = 1, .size = 1 },
  { .id = 11, .row = 1, .size = 1 },
  { .id = 12, .row = 2, .size = 6 },
};
