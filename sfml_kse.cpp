#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

const float PI = 3.14159265f;

enum class ColType
{
    None,
    PlayerAsteroid,
    ProjectileAsteroid
};

class IDummyClass
{
public:
    virtual sf::FloatRect getBounds() const = 0;
};

class Projectile : public IDummyClass
{
public:
	Projectile(sf::Vector2f sPos, float sRot, float pSpeed)
	    : pos(sPos), vel(0.f, 0.f), rotat(sRot), sp(pSpeed), size(5.f)
	{
        projectile.setRadius(size);
        projectile.setFillColor(sf::Color::Red);
        projectile.setOrigin(size, size);
        projectile.setPosition(pos);
    }

    void update(sf::Time dTime)
    {
        sf::Vector2f dir(std::cos(rotat * PI / 180.f), std::sin(rotat * PI / 180.f));
        pos += dir * sp * dTime.asSeconds();
        projectile.setPosition(pos);
    }

    void draw(sf::RenderWindow& window) const
    {
		window.draw(projectile);
	}

    sf::Vector2f getPos() const
    {
        return pos;
    }

    float getSize() const
    {
        return size;
    }

    sf::FloatRect getBounds() const override
    {
		sf::FloatRect bnds;
		bnds.left = pos.x - size;
		bnds.top = pos.y - size;
		bnds.width = size * 2.f;
		bnds.height = size * 2.f;
		return bnds;
	}

    void serialize(std::ofstream& file) const
	{
		file.write(reinterpret_cast<const char*>(&pos), sizeof(sf::Vector2f));
		file.write(reinterpret_cast<const char*>(&rotat), sizeof(float));
		file.write(reinterpret_cast<const char*>(&sp), sizeof(float));
		file.write(reinterpret_cast<const char*>(&size), sizeof(float));
	}

	void deserialize(std::ifstream& file)
	{
		file.read(reinterpret_cast<char*>(&pos), sizeof(sf::Vector2f));
		file.read(reinterpret_cast<char*>(&rotat), sizeof(float));
		file.read(reinterpret_cast<char*>(&sp), sizeof(float));
		file.read(reinterpret_cast<char*>(&size), sizeof(float));

		projectile.setRadius(size);
		projectile.setOrigin(size, size);
		projectile.setPosition(pos);
	}

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float rotat;
    float sp;
    float size;
    sf::CircleShape projectile;
};

class Player : public IDummyClass
{
public:
    Player() : pos(400.f, 500.f), vel(0.f, 0.f), rotat(0.f), speed(5.f), rotatspeed(180.f), size(20.f)
    {
        ship.setPointCount(3);
        ship.setPoint(0, sf::Vector2f(0.f, -size * 2.f));
        ship.setPoint(1, sf::Vector2f(-size, size * 2.f));
        ship.setPoint(2, sf::Vector2f(size, size * 2.f));
        ship.setFillColor(sf::Color::White);
        ship.setOutlineThickness(1.f);
        ship.setOutlineColor(sf::Color::Green);
        ship.setOrigin(0.f, size * 0.5f);
        ship.setPosition(pos);
    }

    void resetPos()
    {
		pos = sf::Vector2f(400.f, 500.f);
		vel = sf::Vector2f(0.f, 0.f);
		rotat = 0.f;
		ship.setPosition(pos);
		ship.setRotation(rotat);
    }

    sf::FloatRect getBounds() const override
    {
		sf::FloatRect bnds;
		bnds.left = pos.x - size;
		bnds.top = pos.y - size;
		bnds.width = size * 2.f;
		bnds.height = size * 2.f;
		return bnds;
    }

    void update(sf::Time dTime, std::vector<Projectile>& projectiles)
    {
        commInput(dTime, projectiles);
        pos += vel * dTime.asSeconds();
        retToWindow();
        ship.setPosition(pos);
        ship.setRotation(rotat);
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(ship);
    }

    void spawnProjectile(std::vector<Projectile>& projectiles)
    {
		sf::Vector2f projPos = pos;
		float projRotat;

		if (shootDir == ShootDirEnum::Right)
		{
			projRotat = rotat + 0.0f;

			sf::Vector2f relPos(0.f, 0.f);
			sf::Transform tform;
			tform.rotate(rotat);
			projPos += tform.transformPoint(relPos);
		}
		else if (shootDir == ShootDirEnum::Left)
		{
			projRotat = rotat - 180.0f;

			sf::Vector2f relPos2(0.f, 0.f);
			sf::Transform tform;
			tform.rotate(rotat);
			projPos += tform.transformPoint(relPos2);
		}
		else
		{
			projRotat = rotat - 90.0f;

			sf::Vector2f relPos3(30.f, -size + 20);
			sf::Transform tform;
			tform.rotate(projRotat);
			projPos += tform.transformPoint(relPos3);
		}

		float projSpeed = 200.f;
		projectiles.emplace_back(projPos, projRotat, projSpeed);
	}


    void commInput(sf::Time dTime, std::vector<Projectile>& projectiles)
    {
    	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    	{
			rotat -= rotatspeed * dTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rotat += rotatspeed* dTime.asSeconds();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			sf::Vector2f dir(-std::cos((rotat+90) * PI / 180.f), -std::sin((rotat+90) * PI / 180.f));
			pos += dir * (speed * 20) * dTime.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			sf::Vector2f dir(std::cos((rotat+90) * PI / 180.f), std::sin((rotat+90) * PI / 180.f));
			pos += dir * (speed * 20) * dTime.asSeconds();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket))
		{
			shootDir = ShootDirEnum::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket))
		{
			shootDir = ShootDirEnum::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backslash))
		{
			shootDir = ShootDirEnum::Default;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			spawnProjectile(projectiles);
		}
	}

    void retToWindow()
    {
        if (pos.x > 800.f) pos.x = 0.f;
        else if (pos.x < 0.f) pos.x = 800.f;

        if (pos.y > 600.f) pos.y = 0.f;
        else if (pos.y < 0.f) pos.y = 600.f;
    }

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float rotat;
    float speed;
    float rotatspeed;
    float size;
    sf::ConvexShape ship;

    enum class ShootDirEnum
	{
		Default,
		Right,
		Left
    };
    ShootDirEnum shootDir = ShootDirEnum::Default;
};

class Asteroid : public IDummyClass
{
public:
    Asteroid(sf::Vector2f sPos, float sRot, float sSpeed) : pos(sPos), rotat(sRot), speed(sSpeed), size(40.f)
    {
        asteroid.setSize(sf::Vector2f(size * 2.f, size * 2.f));
        asteroid.setFillColor(sf::Color::White);
        asteroid.setOutlineThickness(1.f);
        asteroid.setOutlineColor(sf::Color::Red);
        asteroid.setOrigin(size, size);
        asteroid.setPosition(pos);
    }

    bool operator==(const Asteroid& other) const
	{
		return this->pos == other.pos && this->rotat == other.rotat && this->speed == other.speed && this->size == other.size;
	}

    void setSize(const float& newSize)
    {
        size = newSize;
        asteroid.setSize(sf::Vector2f(size * 2.f, size * 2.f));
        asteroid.setOrigin(size, size);
    }

    void splitAsteroid(std::vector<Asteroid>& asteroids)
    {
		if (size > 10.f)
		{
			int numSplit = std::rand() % 6;

			float dAst = size * 2.5f;

			for (int i = 0; i < numSplit; ++i)
			{
				float smSize = size * 0.5f;

				sf::Vector2f smPos((i - numSplit / 2.0f) * dAst, 0.f);

				float randDir = static_cast<float>(std::rand() % 90 - 225);
				asteroids.emplace_back(pos + smPos, randDir, speed);
				asteroids.back().setSize(smSize);
			}
		}
	}

    void update(sf::Time dTime)
    {
        sf::Vector2f dir(std::cos(rotat * PI / 180.f), std::sin(rotat * PI / 180.f));
        pos += dir * speed * dTime.asSeconds();

        if (pos.x < 0) pos.x = 800;
        else if (pos.x > 800) pos.x = 0;

        if (pos.y < 0) pos.y = 600;
        else if (pos.y > 600) pos.y = 0;

        asteroid.setPosition(pos);
        asteroid.setRotation(rotat);
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(asteroid);
    }

    sf::Vector2f getPos() const
    {
        return pos;
    }

    float getSize() const
    {
        return size;
    }

    sf::FloatRect getBounds() const override
    {
        sf::FloatRect bnds;
        bnds.left = pos.x - size;
        bnds.top = pos.y - size;
        bnds.width = size * 2.f;
        bnds.height = size * 2.f;
        return bnds;
    }

    void serialize(std::ofstream& file) const
	{
		file.write(reinterpret_cast<const char*>(&pos), sizeof(sf::Vector2f));
		file.write(reinterpret_cast<const char*>(&rotat), sizeof(float));
		file.write(reinterpret_cast<const char*>(&speed), sizeof(float));
		file.write(reinterpret_cast<const char*>(&size), sizeof(float));
	}

	void deserialize(std::ifstream& file)
	{
		file.read(reinterpret_cast<char*>(&pos), sizeof(sf::Vector2f));
		file.read(reinterpret_cast<char*>(&rotat), sizeof(float));
		file.read(reinterpret_cast<char*>(&speed), sizeof(float));
		file.read(reinterpret_cast<char*>(&size), sizeof(float));

		asteroid.setSize(sf::Vector2f(size * 2.f, size * 2.f));
		asteroid.setOrigin(size, size);
		asteroid.setPosition(pos);
		asteroid.setRotation(rotat);
	}

private:
    sf::Vector2f pos;
    float rotat;
    float speed;
    float size;
    sf::RectangleShape asteroid;
};

class Renderer
{
public:
    Renderer(sf::RenderWindow& win, int& lives, int& points)
        : window(win), lives(lives), points(points) {}

    void render(const Player& player, const std::vector<Asteroid>& asteroids, const std::vector<Projectile>& projectiles) const
    {
        window.clear();
        player.draw(window);
        drawAsteroids(asteroids);
        drawProjectiles(projectiles);
        drawHUD();
        window.display();
    }

private:
    sf::RenderWindow& window;
    int& lives;
    int& points;

    void drawAsteroids(const std::vector<Asteroid>& asteroids) const
    {
        for (const auto& asteroid : asteroids)
        {
            asteroid.draw(window);
        }
    }

    void drawProjectiles(const std::vector<Projectile>& projectiles) const
    {
        for (const auto& projectile : projectiles)
        {
            projectile.draw(window);
        }
    }

    void drawHUD() const
    {
        sf::Font font;
        if (!font.loadFromFile("/home/artem/eclipse-workspace/sfml_kse/Arial.ttf")) {}

        sf::Text lCount;
        lCount.setFont(font);
        lCount.setString("Lives: " + std::to_string(lives));
        lCount.setCharacterSize(20);
        lCount.setFillColor(sf::Color::White);
        lCount.setPosition(window.getSize().x - 150.f, 20.f);
        window.draw(lCount);

        sf::Text pCount;
        pCount.setFont(font);
        pCount.setString("Points: " + std::to_string(points));
        pCount.setCharacterSize(20);
        pCount.setFillColor(sf::Color::White);
        pCount.setPosition(window.getSize().x - 150.f, 50.f);
        window.draw(pCount);
    }
};



class Game
{
public:
    Game() : window(sf::VideoMode(800, 600), "Asteroids Game"), player(), asteroids(), projectiles()
    {
        window.setFramerateLimit(60);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        spawnAsteroids();
    }

    void run()
    {
    	sf::Clock clock;
		Renderer renderer(window, lives, points);

		char opt;
		std::cout << "do you want to load the game from file?: (y/n)" << std::endl;

		std::cin >> opt;
		if (opt == 'y' 	)
		{
			loadGame("/home/artem/eclipse-workspace/sfml_kse/config.txt");
		}

		while (window.isOpen())
		{
			dTime = clock.restart();
			procEvent();
			update();
			renderer.render(player, asteroids, projectiles);
			auto cTime = std::chrono::steady_clock::now();
			if (cTime - lastSaveTime>= std::chrono::seconds(10))
			{
				saveGame("/home/artem/eclipse-workspace/sfml_kse/config.txt");
				lastSaveTime = cTime;
			}
		}
}

private:
    std::chrono::steady_clock::time_point lastSaveTime = std::chrono::steady_clock::now();
    int lives = 3;
    int points = 0;
    sf::Clock astSpawnClock;
    sf::Time astSpawnTimer = sf::seconds(5.f);

    void procEvent()
    {
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		player.commInput(dTime, projectiles);
	}

    void update()
    {
        player.update(dTime, projectiles);
        updateAsteroids();
        updateProjectiles();
        checkCollisions();

        if (astSpawnClock.getElapsedTime() > astSpawnTimer)
        {
			spawnAsteroids();
			astSpawnClock.restart();
		}
    }

    void spawnAsteroids()
    {
    	const int numAster = 3;

		for (int i = 0; i < numAster; ++i)
		{
			asteroids.emplace_back(sf::Vector2f(std::rand() % 800, -40.f), 90, static_cast<float>(std::rand() % 100 + 50));
		}
	}

    void updateAsteroids()
    {
        for (auto& asteroid : asteroids)
        {
            asteroid.update(dTime);
        }
    }

    void updateProjectiles()
    {
        for (auto& projectile : projectiles)
        {
            projectile.update(dTime);
        }

        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) {
                return p.getPos().x < 0 || p.getPos().x > 800 ||
                       p.getPos().y < 0 || p.getPos().y > 600;
            }), projectiles.end());
    }

    void checkCollisions()
    {
    	for (auto projIter = projectiles.begin(); projIter != projectiles.end();)
    	{
			for (auto astIter = asteroids.begin(); astIter != asteroids.end();)
			{
				sf::FloatRect projBnds = projIter->getBounds();
				sf::FloatRect astBnds = astIter->getBounds();

				if (projBnds.intersects(astBnds))
				{
					float astSize = astIter->getSize();

					sf::Vector2f desPos = astIter->getPos();
					astIter->splitAsteroid(asteroids);

					projIter = projectiles.erase(projIter);

					auto asIsDestr = std::find_if(asteroids.begin(), asteroids.end(), [desPos](const Asteroid& a)
					{
						return a.getPos() == desPos;
					});

					if (asIsDestr != asteroids.end())
					{
						astIter = std::find_if(asteroids.begin(), asteroids.end(), [desPos](const Asteroid& a)
						{
							return a.getPos() == desPos;
						});

						if (astIter != asteroids.end())
						{
							astIter = asteroids.erase(astIter);
						}
					}

					if (astSize >= 10.f)
					{
						points += 10;
					}
					else
					{
						int smAsterPts = std::rand() % 10;
						points += smAsterPts;
					}

					break;
				}
				else
				{
					++astIter;
				}
			}

			if (projIter != projectiles.end())
			{
				++projIter;
			}
		}

        sf::FloatRect checkSBnds = player.getBounds();
        for (auto astIter = asteroids.begin(); astIter != asteroids.end();)
        {
            sf::FloatRect astBnds = astIter->getBounds();

            if (checkSBnds.intersects(astBnds))
            {

                player.resetPos();
                lives--;

                if (lives <= 0)
                {
                    std::cout << "Player lost!" << std::endl;
                    std::cout << "You scored: " << points << std::endl;
                    window.close();
                }

                astIter = asteroids.erase(astIter);
                break;
            }
            else
            {
                ++astIter;
            }
        }
    }

    void resetScene()
    {
		asteroids.clear();
		projectiles.clear();
		spawnAsteroids();

		player.resetPos();
    }

    void saveGame(const std::string& fname)
	{
		std::ofstream file(fname, std::ios::binary);
		if (file.is_open())
		{
			int numAsteroids = asteroids.size();
			file.write(reinterpret_cast<const char*>(&numAsteroids), sizeof(int));
			for (const auto& asteroid : asteroids)
			{
				asteroid.serialize(file);
			}

			int numProjectiles = projectiles.size();
			file.write(reinterpret_cast<const char*>(&numProjectiles), sizeof(int));
			for (const auto& projectile : projectiles)
			{
				projectile.serialize(file);
			}

			file.write(reinterpret_cast<char*>(&lives), sizeof(int));
			file.write(reinterpret_cast<char*>(&points), sizeof(int));
			file.close();
		}
		else
		{
			std::cerr << "save failed" << std::endl;
		}
	}

	void loadGame(const std::string& fname)
	{
		std::ifstream file(fname, std::ios::binary);
		if (file.is_open())
		{
			int numAsteroids = 0;
			file.read(reinterpret_cast<char*>(&numAsteroids), sizeof(int));
			asteroids.clear();
			for (int i = 0; i < numAsteroids; ++i)
			{
				Asteroid asteroid(sf::Vector2f(0.f, 0.f), 0.f, 0.f);
				asteroid.deserialize(file);
				asteroids.push_back(asteroid);
			}

			int numProjectiles = 0;
			file.read(reinterpret_cast<char*>(&numProjectiles), sizeof(int));
			projectiles.clear();
			for (int i = 0; i < numProjectiles; ++i)
			{
				Projectile projectile(sf::Vector2f(0.f, 0.f), 0.f, 0.f);
				projectile.deserialize(file);
				projectiles.push_back(projectile);
			}

			file.read(reinterpret_cast<char*>(&lives), sizeof(int));
			file.read(reinterpret_cast<char*>(&points), sizeof(int));
			file.close();
		}
		else
		{
			std::cerr << "load failed" << std::endl;
		}
	}

    sf::RenderWindow window;
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<Projectile> projectiles;
    sf::Time dTime;
};

int main()
{
    Game game;
    game.run();
    return 0;
}
