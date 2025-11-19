#include <stdio.h>
#include <time.h>
#include "file_utils.h"
#include <dirent.h>
#include <stdlib.h>

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

void list_notes()
{
  DIR *dir = opendir("notes");
  if (!dir)
  {
    perror("Failed to open notes directory");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_name[0] == '.')
      continue;
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
}

void view_note_for_date(const char *date)
{
  char filename[64];
  snprintf(filename, sizeof(filename), "notes/%s.txt", date);

  FILE *f = fopen(filename, "r");
  if (!f)
  {
    perror("Failed to open note file");
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), f))
  {
    printf("%s", line);
  }

  fclose(f);
}

void edit_note_for_date(const char *date) 
{
  char filename[64];
  snprintf(filename, sizeof(filename), "notes/%s.txt", date);

  char *editor = getenv("EDITOR");
  if (!editor) editor = "vim";

  char cmd[120];
  snprintf(cmd, sizeof(cmd), "%s %s", editor, filename);
  system(cmd);
}