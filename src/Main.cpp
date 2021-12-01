// #include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

bool check();

const int M = 20;
const int N = 10;

int board[M][N] = { 0 };

struct point
{
	int x, y;
} a[4], b[4];

int figures[7][4] = {
	1, 3, 5, 7, // I
	2,
	4,
	5,
	7, // Z
	3,
	5,
	4,
	6, // S
	3,
	5,
	4,
	7, // T
	2,
	3,
	5,
	7, // L
	3,
	5,
	7,
	6, // J
	2,
	3,
	4,
	5, // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return 0;
		else if (board[a[i].y][a[i].x])
			return 0;
	return 1;
}

int main()
{
	srand(time(0));

	// opening game window
	RenderWindow window(VideoMode(320, 480), "A Game of Tetris");

	// loading tetris pieces image
	Texture t1;
	t1.loadFromFile("images/tiles.png");

	Sprite s(t1);

	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;
	float timer = 0, delay = 0.4;

	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}

			// checking key strokes
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Up)
					rotate = true;
				else if (e.key.code == Keyboard::Left)
					dx = -1;
				else if (e.key.code == Keyboard::Right)
					dx = 1;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		// Moving left to right
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];

		// Rotating Pieces
		if (rotate)
		{
			point p = a[1]; // finding center
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}

		// Game Tick
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check())
			{
				for (int i = 0; i < 4; i++)
					board[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		// check for a tetris
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])
					count++;
				field[k][j] = field[i][j];
			}
			if (count < N)
				k--;
		}

		dx = 0;
		rotate = 0;
		delay = 0.3;

		// background
		window.clear(Color::Black);
		window.draw(background);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(28, 31); //offset
			window.draw(s);
		}

		window.draw(frame);
		window.display();
	}

	return 0;
}
