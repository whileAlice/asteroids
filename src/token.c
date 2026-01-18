#include "token.h"

#include "error.h"

#include <stdio.h>
#include <string.h>

char*
read_next_line (FILE* fp, char* buf, const char* filename, int* line_count)
{
   *line_count += 1;

   char* line = fgets (buf, sizeof (buf), fp);
   if (line == NULL)
      ERROR_RETURN (NULL, "couldn't read line %d from file '%s'", *line_count,
                    filename);

   char* newline = strchr (line, '\n');
   if (newline != NULL)
      *newline = '\0';

   return line;
}
