Pour compiler:

-Sans lbift:
gcc -I minilibx_macos test.c -L minilibx_macos -lmlx -framework OpenGL -framework AppKit

-Avec libft:
gcc -I minilibx_macos -I libft/includes test.c -L minilibx_macos -L libft -lft -lmlx -framework OpenGL -framework AppKit
