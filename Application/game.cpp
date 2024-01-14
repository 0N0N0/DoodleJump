#include "../Engine/Framework/Engine.h"
#include "../Engine/Utils/Timer.h"
#include "../External/include/SFML/Graphics.hpp"

int main(int argc, char *argv[])
{
	if (argv[1] == NULL)
	{
		window_width = 600;
		window_height = 800;
	}
	else
	{
		std::string window_resolution = std::string(argv[1]);
		size_t pos = window_resolution.find('x');
		std::string width_str = window_resolution.substr(0, pos);
		std::string height_str = window_resolution.substr(pos + 1);
		window_width = std::stoi(width_str);
		window_height = std::stoi(height_str);
	}

	std::cout << "Width: " << window_width << std::endl;
	std::cout << "Height: " << window_height << std::endl;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Doodle Jump");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	MyFramework framework;
	framework.PreInit(window_width, window_height);
	framework.Init();
	framework.run(window);

	return 0;
}
