#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "file_utils.h"

void handle_command(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Elucid - CLI for notes\n");
    printf("Usage: elucid <command>\n");
    printf("Commands available: new, list, view, edit, delete\n");
    return;
  }

  const char *command = argv[1];

  if (strcmp(command, "new") == 0)
  {
    create_note_for_today();
  }
  else if (strcmp(command, "list") == 0)
  {
    list_notes();
  }
  else if (strcmp(command, "view") == 0)
  {
    if (argc < 3)
    {
      printf("Usage: elucid view <date>\n");
      return;
    }
    view_note_for_date(argv[2]);
  }
  else if (strcmp(command, "edit") == 0)
  {
    if (argc < 3)
    {
      printf("Usage: elucid edit <date>\n");
      return;
    }
    edit_note_for_date(argv[2]);
  }
  else if (strcmp(command, "delete") == 0)
  {
    printf("Deleting note...\n");
  }
  else if (strcmp(command, "help") == 0)
  {
    printf("Commands available: new, list, view, edit, delete\n");
  }
  else
  {
    printf("Unknown command: %s\n", command);
    printf("Use 'elucid help' to see the available commands.\n");
  }
}
