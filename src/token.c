
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
read_next_line (FILE* fp, char* buf, const char* filename, int* line_count)
{
   *line_count += 1;
   char* line   = fgets (buf, sizeof (buf), fp);
   if (line == NULL)
   {
      printf ("ERROR: couldn't read line %d from file %s\n", *line_count,
              filename);
      exit (1);
   }

   char* newline = strchr (line, '\n');
   if (newline != NULL)
   {
      *newline = '\0';
   }

   return line;
}
