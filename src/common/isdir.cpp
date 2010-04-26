#include <sys/types.h>
#include <sys/stat.h>

#if !defined S_ISDIR && defined S_IFDIR
# define S_ISDIR(Mode) (((Mode) & S_IFMT) == S_IFDIR)
#endif

/* If PATH is an existing directory or symbolic link to a directory,
   return nonzero, else 0.  */
int	isdir (const char *path)
{
  struct stat stats;

  return stat (path, &stats) == 0 && S_ISDIR (stats.st_mode);
}
