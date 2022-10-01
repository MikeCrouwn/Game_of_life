#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define string_max 25
#define colum_max 80

void input_zero(char **a, int string, int colum);
void output(char **a, int string, int colum);
int check_neighbours(char **a, int i_center, int j_center);
void new_generation(char **a, char **b, int string, int colum);
int check_alive(char **a, int string, int colum);

int main() {
  char **arr_original;
  char **arr_new_generation;
  arr_original = (char **)malloc(string_max * colum_max * sizeof(char) +
                                 string_max * sizeof(char *));
  char *point_1 = (char *)(arr_original + string_max);
  for (int i = 0; i < string_max; i++)
    *(arr_original + i) = point_1 + colum_max * i;
  arr_new_generation = (char **)malloc(string_max * colum_max * sizeof(char) +
                                       string_max * sizeof(char *));
  char *point_2 = (char *)(arr_new_generation + string_max);
  for (int i = 0; i < string_max; i++)
    *(arr_new_generation + i) = point_2 + colum_max * i;

  input_zero(arr_original, string_max, colum_max);
  if (!freopen("/dev/tty", "r", stdin)) {
    perror("/dev/tty");
    exit(1);
  }
  initscr();
  noecho();
  timeout(0);
  for (int i = 0; i < string_max; i++) {
    for (int j = 0; j < colum_max; j++) {
      check_neighbours(arr_original, i, j);
    }
  }

  char input = 't';
  int speed = 200000;
  do {
    input = getch();
    if (input == 'Z' || input == 'z') {
      speed += 50000;
    } else if (input == 'X' || input == 'x') {
      speed -= 50000;
    }
    if (speed <= 0) {
      speed = 15000;
    }
    new_generation(arr_original, arr_new_generation, string_max, colum_max);
    char **temp = arr_original;
    arr_original = arr_new_generation;
    arr_new_generation = temp;
    if (check_alive(arr_new_generation, string_max, colum_max) == 0) {
      break;
    }
    output(arr_new_generation, string_max, colum_max);
    clear();
    usleep(speed);
  } while (input != 'q');

  free(arr_original);
  free(arr_new_generation);
  endwin();
  return 0;
}

void input_zero(char **a, int string, int colum) {
  for (int i = 0; i < string; i++) {
    for (int j = 0; j < colum; j++) {
      scanf("%c ", &a[i][j]);
    }
  }
}

void output(char **a, int string, int colum) {
  for (int i = 0; i < string; i++) {
    for (int j = 0; j < colum; j++) {
      if (j < colum - 1) {
        printw("%c ", a[i][j]);
      } else {
        printw("%c", a[i][j]);
      }
    }
    if (i < string - 1) {
      printw("\n");
    }
  }
  printw("\nPress 'q' to exit game.\n");
  printw("Press 'z' to decrease speed game.\n");
  printw("Press 'x' to decrease speed game.\n");
  refresh();
}

int check_neighbours(char **a, int i_center, int j_center) {
  int alive = 0;
  for (int i = i_center - 1; i <= i_center + 1; i++) {
    for (int j = j_center - 1; j <= j_center + 1; j++) {
      if (i == i_center && j == j_center) {
      } else {
        int k = (i < 0) ? (i + string_max) : (i > 24) ? (i - string_max) : i;
        int m = (j < 0) ? (j + colum_max) : (j > 79) ? (j - colum_max) : j;
        if (a[k][m] == '#') {
          alive++;
        }
      }
    }
  }
  return alive;
}

void new_generation(char **a, char **b, int string, int colum) {
  for (int i = 0; i < string; i++) {
    for (int j = 0; j < colum; j++) {
      b[i][j] = a[i][j];
      int alive = check_neighbours(a, i, j);
      if (a[i][j] == '.' && alive == 3) {
        b[i][j] = '#';
      } else if (a[i][j] == '#' && alive < 2) {
        b[i][j] = '.';
      } else if (a[i][j] == '#' && alive > 3) {
        b[i][j] = '.';
      } else {
        b[i][j] = a[i][j];
      }
    }
  }
}

int check_alive(char **a, int string, int colum) {
  int alive = 0;
  for (int i = 0; i < string; i++) {
    for (int j = 0; j < colum; j++) {
      if (a[i][j] == '#') {
        alive++;
      }
    }
  }
  return alive;
}
