#include <stdio.h>
#include <time.h>
#include "file_utils.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

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
  if (!editor)
    editor = "vim";

  char cmd[120];
  snprintf(cmd, sizeof(cmd), "%s %s", editor, filename);
  system(cmd);
}

void delete_note_for_date(const char *date)
{
  char filename[64];
  snprintf(filename, sizeof(filename), "notes/%s.txt", date);

  if (remove(filename) == 0)
  {
    printf("Note deleted successfully\n");
  }
  else
  {
    perror("Failed to delete note file");
  }
}

void search_notes(const char *keyword)
{
  DIR *dir = opendir("notes");
  if (!dir)
  {
    perror("Failed to open notes directory");
    return;
  }

  struct dirent *entry;
  char path[128], line[512];
  int found_any = 0;

  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_name[0] == '.')
      continue;

    snprintf(path, sizeof(path), "notes/%s", entry->d_name);
    FILE *f = fopen(path, "r");
    if (!f)
      continue;

    while (fgets(line, sizeof(line), f))
    {
      if (strstr(line, keyword))
      {
        printf("[%s] %s", entry->d_name, line);
        found_any = 1;
      }
    }

    fclose(f);
  }

  closedir(dir);

  if (!found_any)
  {
    printf("No occurrences of \"%s\" found.\n", keyword);
  }
}
void list_notes_by_tag(const char *tag)
{
  DIR *dir = opendir("notes");
  if (!dir)
  {
    perror("Failed to open notes directory");
    return;
  }

  struct dirent *entry;
  char path[128], line[256];
  int found = 0;

  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_name[0] == '.')
      continue;

    snprintf(path, sizeof(path), "notes/%s", entry->d_name);
    FILE *f = fopen(path, "r");
    if (!f)
      continue;

    for (int i = 0; i < 3 && fgets(line, sizeof(line), f); i++)
    {
      if (strstr(line, tag))
      {
        printf("[%s]\n", entry->d_name);
        found = 1;
        break;
      }
    }

    fclose(f);
  }

  closedir(dir);

  if (!found)
    printf("No notes found with the tag %s\n", tag);
}