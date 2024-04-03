// ©2024, VoKoPro (Volod'ko project)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>

//settings
const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float INITIAL_SPEED = 100.f; // pix per seconds
const float PLAYER_SIZE = 20.f;
const float ACCELERATION = 20.f; // pix per seconds per seconds
const int NUM_APPLES = 20;
const float APPLE_SIZE = 20.f;
 
bool startGame()
{
	//randomaiser
	int seed = (int)time(nullptr);
	srand(seed);

	//init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apple game!");
	float playerX = SCREEN_WIDTH / 2.f;
	float playerY = SCREEN_HEIGHT / 2.f;
	float playerSpeed = INITIAL_SPEED;
	int playerDirection = 0; // 0 - right, 1 - up, 2 - left, 3 - down

	//player shape
	sf::RectangleShape playerShape;
	playerShape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	playerShape.setFillColor(sf::Color::Red);
	playerShape.setOrigin(PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f);
	playerShape.setPosition(playerX, playerY);

	//init apples
	float appleX[NUM_APPLES];
	float appleY[NUM_APPLES];
	bool isAppleEaten[NUM_APPLES];
	sf::CircleShape appleShape[NUM_APPLES];

	//apple shape
	for (int i = 0; i < NUM_APPLES; ++i)
	{
		isAppleEaten[i] = false;
		appleX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
		appleY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;
		appleShape[i].setRadius(APPLE_SIZE / 2);
		appleShape[i].setFillColor(sf::Color::Green);
		appleShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
		appleShape[i].setPosition(appleX[i], appleY[i]);
	}

	int numEatenApples = 0;

	//game clock
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	//main loop
	while (window.isOpen())
	{
		//time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//keyboard commands
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerDirection = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerDirection = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			playerDirection = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			playerDirection = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerDirection = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerDirection = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerDirection = 3;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			playerDirection = 3;
		}

		//acceleration with time
		//playerSpeed += ACCELERATION * deltaTime;

		//player moove

		if (playerDirection == 0)
		{
			playerX += playerSpeed * deltaTime;
		}
		else if (playerDirection == 1)
		{
			playerY -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 2)
		{
			playerX -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 3)
		{
			playerY += playerSpeed * deltaTime;
		}



		//check on boards
		if (playerX - PLAYER_SIZE / 2.f < 0.f || playerX + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			playerY - PLAYER_SIZE / 2.f < 0.f || playerY + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
		{
			window.close();
			Sleep(3000);
			startGame();
		}

		// check eaten square

		//for (int i = 0; i < NUM_APPLES; ++i)
		//{
		//	if (!isAppleEaten[i])
		//	{
		//		float dx = fabs(playerX - appleX[i]);
		//		float dy = fabs(playerY - appleY[i]);
		//		if (dx <= (APPLE_SIZE + PLAYER_SIZE) / 2.f &&
		//			dy <= (APPLE_SIZE + PLAYER_SIZE) / 2.f)
		//		{
		//			isAppleEaten[i] = true;
		//			++numEatenApples;
		//		}
		//	}
		//}

		// check eaten circle

		for (int i = 0; i < NUM_APPLES; ++i)
		{
			if (!isAppleEaten[i])
			{
				float scuareDistance = fabs((playerX - appleX[i]) * (playerX - appleX[i]) +
					(playerY - appleY[i]) * (playerY - appleY[i]));
				float scuareRadiusSum = (APPLE_SIZE + PLAYER_SIZE) * (APPLE_SIZE + PLAYER_SIZE) / 4;

				if (scuareDistance <= scuareRadiusSum)
				{
					isAppleEaten[i] = true;
					++numEatenApples;
					playerSpeed += ACCELERATION;
					// respawn apple
					isAppleEaten[i] = false;
					appleX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
					appleY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;
					appleShape[i].setRadius(APPLE_SIZE / 2);
					appleShape[i].setFillColor(sf::Color::Blue);
					appleShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
					appleShape[i].setPosition(appleX[i], appleY[i]);
				}
			}
		}

		//canceled game if num eaten apples == NUM_APPLES

		//if (numEatenApples == NUM_APPLES)
		//{
		//	window.close();
		//	break;
		//}

		window.clear();
		playerShape.setPosition(playerX, playerY);
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			if (!isAppleEaten[i])
			{
				window.draw(appleShape[i]);
			}

		}
		window.draw(playerShape);
		window.display();
	}

	return 0;
}
int main()
{
	startGame();
	return 0;
}
