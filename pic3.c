#include <stdio.h>
#include <unistd.h>

FILE *testFile;

int gridWidth, gridHeight, sqSize, nFrames;
long headerEnd;

int FetchData() {
  int data = 0;
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
  for (int currentFrame = 0; currentFrame < nFrames; currentFrame++) {
    int imgIndex = 0;

    for (int n = gridWidth / 2; n > 0; n--) {
      for (int i = gridHeight; i > 0; i--) {
        printColorTerminal(FetchData(), FetchData(), FetchData());
        imgIndex++;
      }
      puts("");

      for (int i = gridHeight; i > 0; i--) {
        printColorTerminal(FetchData(), FetchData(), FetchData());
        imgIndex++;
      }
      puts("");
    }

    clear();
    usleep(200000);
  }

  fseek(testFile, headerEnd, SEEK_SET);
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

  headerEnd = ftell(testFile);
  printf("headerEnd: %2ld\n", headerEnd);

  while (1) {
    loop();
  }

  return 0;
}
