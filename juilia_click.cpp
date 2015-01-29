#include <SDL2/SDL.h>

/*declare structure containing all relevant mouse information */

struct mouseStructure
{
    int counter;
    double x_position;
    double y_position;
}


int clickEvent(mouseStructure &structure)
{
    int quit = 0;
    SDL_Event e;

    while (!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) /* if someone left clicks */
            {
                structure.counter = structure.counter + 1;
                e.button.x = structure.x_position;
                e.button.y = structure.y_position;
                
                /*increase the counter by 1 and record the x and y position */
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) /* if someone right clicks */
            {
                structure.counter = structure.counter + 1;
                e.button.x = structure.x_position;
                e.button.y = structure.y_position;
                
                /*decrease counter by 1 and record the x and y position */
            }
            return structure;
        }
    }
}

