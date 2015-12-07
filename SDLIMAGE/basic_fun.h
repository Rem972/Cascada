# ifndef _BASIC_FUN_
# define _BASIC_FUN_
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

void init_sdl(void);
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void convert_to_grey(SDL_Surface *img);
int *image_to_array(char *path);

#endif
