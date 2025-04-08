#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *testFile;

int gridWidth, gridHeight, sqSize, nFrames;
long headerEnd;

// Pointer to the array of frames that will be initialized in the heap
uint8_t *frames = NULL;

uint8_t FetchData() {
  uint8_t data = 0;
  int buffer;

  while ((buffer = fgetc(testFile)) != EOF) {
    if (buffer == ',' || buffer == ':') {
      return data;
    }
    data = data * 10 + (buffer - '0');
  }

  puts("EOF");

  return data;
}

void clear() { printf("\033[2J\033[H"); }

void printColorTerminal(int r, int g, int b) {
  printf("\033[48;2;%d;%d;%dm ", r, g, b);
}

void loop() {
  int frameIdx = 0;

  for (int currentFrame = 0; currentFrame < nFrames; currentFrame++) {
    int imgIndex = 0;

    for (int n = gridWidth / 2; n > 0; n--) {
      for (int i = gridHeight; i > 0; i--) {
        int r = frameIdx;
        printColorTerminal(frames[r], frames[r + 1], frames[r + 2]);
        imgIndex++;
        frameIdx += 3;
      }
      puts("");

      for (int i = gridHeight; i > 0; i--) {
        int r = frameIdx;
        printColorTerminal(frames[r], frames[r + 1], frames[r + 2]);
        imgIndex++;
        frameIdx += 3;
      }
      puts("");
    }

    clear();
    usleep(200000);
  }
}

int main() {
  testFile = fopen("graph-sd.txt", "r");

  if (testFile == NULL) {
    puts("Error opening file");
    return 1;
  }

  puts("Reading test.txt:");

  gridWidth = FetchData();
  printf("width:     %2d\n", gridWidth);
  gridHeight = FetchData();
  printf("height:    %2d\n", gridHeight);
  sqSize = FetchData();
  printf("sqSize:    %2d\n", sqSize);
  nFrames = FetchData();
  printf("nFrames:   %2d\n", nFrames);

  // Allocates in heap an array of n bytes. (sizeof(char) is 1 (byte))
  // So it doesn't really mean much in here but it's good practice to put it
  unsigned long length = gridWidth * gridHeight * nFrames * 3 * sizeof(char);
  frames = malloc(length);

  // Read every byte into the buffer
  for (int i = 0; i < length; i++) {
    frames[i] = FetchData();
  }

  // We don't need to have it open anymore, let's close it
  fclose(testFile);

  while (1) {
    loop();
  }

  // Every time you stop using something that has been malloc'd you gotta free
  // it. Obviously we're never reaching here cuz it's intifinite, but yknow.
  free(frames);

  return 0;
}
