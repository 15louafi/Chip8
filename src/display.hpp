#include <cstdint>

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class Display
{
public:
	Display(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
	~Display();
	void Update(void const* buffer, int pitch);

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};