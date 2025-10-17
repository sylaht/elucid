#include <stdio.h>
#include <time.h>
#include "file_utils.h"

void create_note_for_today()
{
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char filename[64];
  strftime(filename, sizeof(filename), "notes/%Y-%m-%d.txt", t);

  FILE *f = fopen(filename, "a");
  if (!f)
  {
    perror("Failed to create note file");
    return;
  }

  fprintf(f, "--- New note in %02d/%02d/%04d ---\n",
          t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
  fclose(f);

  printf("Note created successfully in %s\n", filename);
}