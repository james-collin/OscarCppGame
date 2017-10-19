#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include <vector>
#include <cstdlib>
#include <iostream>
#include<math.h>
#include "wtypes.h"
#include <chrono>
#include <thread>

using namespace sf;

class Bullet
{
public:
	Sprite shape;

	Bullet(Texture *texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);

		this->shape.setScale(2.f, 2.f);

		this->shape.setPosition(pos);
	}
	~Bullet() {}
};

class TheShip
{
public:
	Sprite shape;
	Texture *texture;

	int HP;
	int HPMax;

	std::vector<Bullet> bullets;
	std::vector<Bullet> bulletsLeft;
	std::vector<Bullet> bulletsRight;

	TheShip(Texture *texture)
	{
		this->HPMax = 100;
		this->HP = this->HPMax;

		this->texture = texture;
		this->shape.setTexture(*texture);

		this->shape.setScale(2.f, 2.f);

	}

	~TheShip() {}


};

class Invader
{
public:
	Sprite shape;

	int HP;
	int HPMax;
	bool leftWall;
	bool rightWall;
	bool leftSide;
	std::vector<Bullet> bullets;

	Invader(Texture *texture, Vector2u windowSize)
	{
		this->HPMax = rand() % 3 + 1;
		this->HP = this->HPMax;
		this->leftWall = false;
		this->rightWall = false;
		this->shape.setTexture(*texture);

		this->shape.setScale(1.5f, 1.5f);

		this->shape.setPosition((windowSize.y, rand() % int(windowSize.x - this->shape.getGlobalBounds().width)), -60.f);
	}

	

	~Invader() {}
};


// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}



int main()
{

	srand(time(NULL));


	/*RenderWindow �r som ett tomt papper och det �r d�r som man kommer m�la allting p�.
	Den l�nga parantesen �r windows konstruktor och den vita texten window �r dess namn och
	man kan s�klart d�pa den till vad man vill. Style::Default �r vilken typ utav screen
	som man vi ska dyka upp. window.setFramerateLimite() �r till f�r att s�tt en gr�ns f�r
	frameraten s� inte datorn g�r s�nder*/
	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	RenderWindow window(VideoMode(horizontal, vertical), "Shoot_Em_Up_Spel", Style::Default);
	window.setFramerateLimit(60);
	// Text init
	Font font;
	font.loadFromFile("Fonts/space age.ttf");

	// Textures init 
	Texture shipTex;
	shipTex.loadFromFile("Invadersspelet/Ship.png");


	Texture invaderTex;
	invaderTex.loadFromFile("Invadersspelet/Invader.png");


	Texture bulletTex;
	bulletTex.loadFromFile("Invadersspelet/Bullet.png");

	Texture explosionTex;
	explosionTex.loadFromFile("Invadersspelet/Explosion.png");



	// UI initiala
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::Red);
	scoreText.setPosition((horizontal-scoreText.getCharacterSize()*10), vertical-80);

	// Ship init
	int score = 0;
	TheShip ship(&shipTex);

	ship.shape.setPosition(horizontal/2,vertical/2);
	int shootTimer = 20;
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(30);
	hpText.setFillColor(Color::Red);
	hpText.setPosition(25.f, 10.f);

	// Invader init
	int invaderSpawnTimer = 0;
	std::vector<Invader> invaders;
	int shootTimerInvader = 50;
	
	



	/*while boolien �r sann n�r f�nstret/window �r �ppet allts� n�r spelet k�rs.
	N�r f�nstret �r �ppet s� �r pollEvent till f�r att ta emot/catcha inputen fr�n
	anv�ndaren �ven om anv�ndaren skulle trycka bort rutan med krysset l�ngst uppe i v�nstra
	h�rnet och d� tar if statmentet emot den h�ndelsen. Man skulle �ven kunna skriva
	if(event.type == Event::KeyPressed och d� skulle en specifik tangent st�nga ner f�nstret*/
	// Gjorde nu �ven s� att om hp �r mindre eller lika med 0 s� slutar spelet men det g�r den �ven om man skulle trycka p� Escape � 
	while (window.isOpen() && ship.HPMax > 0)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			// Detta �r f�r om man vill sluta tidigare �
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
		}

		//Update
		/*H�r Kollar man saker som exempelvis om W har tryckts in och karakt�ren r�r sig s�
		uppdaterar man karakt�rens position/koordinaterna. Men man vill inte koda i h�r om man man vill g�ra n�got som �r stilla
		exempelvis en stilla cirkel om man skulle skriva in en r�relse av den h�r s� blir den en linje eftersom den hela
		tiden byter position. Men eftersom vi har window.clear(); s� blir det inte s� utan cirkeln r�r sig ist�llet*/
		//"�"


		// Moving the ship
		if (Keyboard::isKeyPressed(Keyboard::Up))
			ship.shape.move(0.f, -10.f);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			ship.shape.move(-10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::Down))
			ship.shape.move(0.f, 10.f);
		if (Keyboard::isKeyPressed(Keyboard::Right))
			ship.shape.move(10.f, 0.f);


		// Collicion with the walls
		// Left wall
		if (ship.shape.getPosition().x <= 0)
			ship.shape.setPosition(0.f, ship.shape.getPosition().y);
		// Right wall
		if (ship.shape.getPosition().x >= window.getSize().x - ship.shape.getGlobalBounds().width)
			ship.shape.setPosition(window.getSize().x - ship.shape.getGlobalBounds().width, ship.shape.getPosition().y);
		//  The roof
		if (ship.shape.getPosition().y <= 0)
			ship.shape.setPosition(ship.shape.getPosition().x, 0.f);
		// The floor
		if (ship.shape.getPosition().y >= window.getSize().y - ship.shape.getGlobalBounds().height)
			ship.shape.setPosition(ship.shape.getPosition().x, window.getSize().y - ship.shape.getGlobalBounds().height);


		// This is for shooting the bullets
		if (shootTimer < 20)
			shootTimer++;

		// The controlls for shooting the bullets with the ship
		if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 20)
		{
			//set ship shooting from center
			Vector2f v = ship.shape.getPosition();
			v.x += 25;
			ship.bullets.push_back(Bullet(&bulletTex, v ));


			ship.bulletsLeft.push_back(Bullet(&bulletTex, v));
			ship.bulletsRight.push_back(Bullet(&bulletTex, v));
			shootTimer = 0; // Reset timern
		}




		for (size_t i = 0; i < ship.bullets.size(); i++)
		{
			// Move
			ship.bullets[i].shape.move(0.f, -10.f);
			

			// Taking away the bullets when they go outside the screen
			if (ship.bullets[i].shape.getPosition().y > window.getSize().y)
			{
				ship.bullets.erase(ship.bullets.begin() + i);
				
			}




			// Collision between the bullets and the invaders

			for (size_t k = 0; k < invaders.size(); k++)
			{
				if (ship.bullets[i].shape.getGlobalBounds().intersects(invaders[k].shape.getGlobalBounds()))
				{
					
					score += 100;
					invaders[k].shape.setTexture(explosionTex);
					window.draw(invaders[k].shape);
					window.display();
					
					ship.bullets.erase(ship.bullets.begin() + i);
					
					invaders.erase(invaders.begin() + k);
					
					break;
					
					
				}

			}
		}

		//LEFT BULLETS
		for (size_t i = 0; i < ship.bulletsLeft.size(); i++)
		{
			// Move
			
			ship.bulletsLeft[i].shape.move(-3.f, -10.f);
			

			// Taking away the bullets when they go outside the screen
			if (ship.bulletsLeft[i].shape.getPosition().y > window.getSize().y)
			{
				
				ship.bulletsLeft.erase(ship.bulletsLeft.begin() + i);
				
			}




			// Collision between the bullets and the invaders

			for (size_t k = 0; k < invaders.size(); k++)
			{
				if (ship.bulletsLeft[i].shape.getGlobalBounds().intersects(invaders[k].shape.getGlobalBounds()))
				{
					score += 100;
					invaders[k].shape.setTexture(explosionTex);
					window.draw(invaders[k].shape);
					window.display();
					invaders.erase(invaders.begin() + k);
					ship.bulletsLeft.erase(ship.bulletsLeft.begin() + i);
					break;
				}

			}
		}
		//RIGHT BULLETS
		for (size_t i = 0; i < ship.bulletsRight.size(); i++)
		{
			// Move
			
			ship.bulletsRight[i].shape.move(3.f, -10.f);

			// Taking away the bullets when they go outside the screen
			if (ship.bulletsRight[i].shape.getPosition().y > window.getSize().y)
			{
				
				ship.bulletsRight.erase(ship.bulletsRight.begin() + i);
			}




			// Collision between the bullets and the invaders

			for (size_t k = 0; k < invaders.size(); k++)
			{
				if (ship.bulletsRight[i].shape.getGlobalBounds().intersects(invaders[k].shape.getGlobalBounds()))
				{
					score += 100;
					invaders[k].shape.setTexture(explosionTex);
					window.draw(invaders[k].shape);
					window.display();
					invaders.erase(invaders.begin() + k);
					ship.bulletsRight.erase(ship.bulletsRight.begin() + i);
					break;
				}

			}
		}




		// This is for the invaders
		if (invaderSpawnTimer < 70)
			invaderSpawnTimer++;

		if (invaderSpawnTimer >= 70)
		{
			Invader i(&invaderTex, window.getSize());
			//choose from which side invaders come
			int l = rand() % 100;
			if (l > 50)
				i.leftSide = true;
			else
				i.leftSide = false;
			invaders.push_back(i);
			invaderSpawnTimer = 0; // Reset timer
			
		}
		for (size_t i = 0; i < invaders.size(); i++)
		{

			

			//check side from where invaders come
			if(invaders[i].leftSide==true)
			{
				if (invaders[i].leftWall == false)
					invaders[i].shape.move(-5.f, 1.5);
				else
					invaders[i].shape.move(5.f, 1.5);

				if (invaders[i].shape.getPosition().x <= 0)
					invaders[i].leftWall = true;

				// Right wall
				if (invaders[i].shape.getPosition().x >= window.getSize().x - invaders[i].shape.getGlobalBounds().width)
					invaders[i].leftWall = false;
			}
			else
			{
				if (invaders[i].rightWall == false)
					invaders[i].shape.move(5.f, 1.5);
				else
					invaders[i].shape.move(-5.f, 1.5);

				if (invaders[i].shape.getPosition().x <= 0)
					invaders[i].rightWall = false;

				// Left wall
				if (invaders[i].shape.getPosition().x >= window.getSize().x - invaders[i].shape.getGlobalBounds().width)
					invaders[i].rightWall = true;
			}
			



			if (invaders[i].shape.getPosition().y > window.getSize().y + invaders[i].shape.getGlobalBounds().height)
				invaders.erase(invaders.begin() + i);
			
			//colision invaders with ship
			if (invaders[i].shape.getGlobalBounds().intersects(ship.shape.getGlobalBounds()))
			{
				ship.HPMax -= 5;
				score += 100;
				invaders.erase(invaders.begin() + i);



			}
		}

		//This is for shooting bullets with invaders
		if (shootTimerInvader < 50)
			shootTimerInvader++;

		//adding bullets
		if ( shootTimerInvader >= 50)
		{
			
			for (size_t i = 0; i < invaders.size(); i++)
			{
				Vector2f v = invaders[i].shape.getPosition();
				v.x += 25;
				v.y += 35;
				invaders[i].bullets.push_back(Bullet(&bulletTex, v));
			}
			shootTimerInvader = 0; // Reset timern
		}

		
		for (size_t i = 0; i < invaders.size(); i++)
			for (size_t j = 0; j < invaders[i].bullets.size(); j++)
			{
				// Move
				
					if(invaders[i].leftWall==false)
						invaders[i].bullets[j].shape.move(3.f, 5.f);
					else
						invaders[i].bullets[j].shape.move(-3.f, 5.f);
				
				// Taking away the bullets when they go outside the screen
				if ((invaders[i].bullets[j].shape.getPosition().y > window.getSize().y)|| (invaders[i].bullets[j].shape.getPosition().x < 0)|| (invaders[i].bullets[j].shape.getPosition().x > window.getSize().x))
					invaders[i].bullets.erase(invaders[i].bullets.begin() + j);
				
		


			}
				
				//colision between  invaders bullet and ship
		for (size_t i = 0; i < invaders.size(); i++)
			for (size_t j = 0; j < invaders[i].bullets.size(); j++)
				if (invaders[i].bullets[j].shape.getGlobalBounds().intersects(ship.shape.getGlobalBounds()))
				{
					ship.HPMax -= 5;
					invaders[i].bullets.erase(invaders[i].bullets.begin() + j);
					break;
				}
		



		// UI Update
		scoreText.setString("Score: " + std::to_string(score));
		hpText.setString("Life: " + std::to_string(ship.HPMax));


		//Draw
		/*Denna �r f�r att r�nsa det som var i den tidigare framen s� att man inte m�lar p� varandra hela tiden.
		H�r kan man ocks� g�ra s� att bakgrunden f�r en annan f�rg gen om att skriva in i dess parantes
		och d� clearar den sk�rmen med f�rgen r�d*/
		window.clear();

		//Draw stuff
		/*H�r m�lar man karakt�ren i sin nya position*/

		window.draw(ship.shape);

		for (size_t i = 0; i < ship.bullets.size(); i++)
		{
			window.draw(ship.bullets[i].shape);
		}

		for (size_t i = 0; i < ship.bulletsLeft.size(); i++)
		{
			window.draw(ship.bulletsLeft[i].shape);
		}

		for (size_t i = 0; i < ship.bulletsRight.size(); i++)
		{
			window.draw(ship.bulletsRight[i].shape);
		}



		for (size_t i = 0; i < invaders.size(); i++)
		{
			window.draw(invaders[i].shape);
			for (size_t j = 0; j < invaders[i].bullets.size(); j++)
				window.draw(invaders[i].bullets[j].shape);
		}



		// UI
		window.draw(scoreText);
		window.draw(hpText);




		//Finished drawing
		/*H�r �r n�r man �r f�rdig och vill displaya det som man har gjort */
		window.display();
	}


	return 0;

}