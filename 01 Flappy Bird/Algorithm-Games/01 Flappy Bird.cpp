#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <cstdlib>
#include <random>

using namespace std;

random_device rd;
mt19937_64 generator(rd());

class Bird {
private:
	double x;
	double y;
	double vy = 0;
	int radius = 15;

	double gravity = 0.6;
	double jump_velocity = -8.5;

public:
	Bird(double start_x, double start_y) {
		x = start_x;
		y = start_y;
	}

	// Visual update every frame
	void update() {
		vy += gravity;
		y += vy;
	}

	// Player press key to jump
	void jump() {
		vy = jump_velocity;
	}

	// Re-draw screen every frame
	void draw() const {
		setfillcolor(YELLOW);
		solidcircle(static_cast<int>(x), static_cast<int>(y), radius);
	}

	/*
	Because X, Y, and radius are
	all private, we cannot access
	them through bird.x, bird.y,
	and bird.radius.
	*/
	double getX() const {
		return x;
	}

	double getY() const {
		return y;
	}

	int getRadius() const {
		return radius;
	}
};

inline bool isKeyPressed(int virtualKey) {
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

int main(void) {
	// The RNG engine
	uniform_int_distribution<int> dist(0, 100);
	int random_number = dist(generator);
	const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600; // Set width and height

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT); // Initialize screen
	BeginBatchDraw(); // Batches

	Bird bird(150, 200);

	while (true)
	{
		// Input
		if (isKeyPressed(VK_SPACE))
		{
			bird.jump();
		}

		// updating
		bird.update();

		// Out of screen or hit pipe
		if (bird.getY() + bird.getRadius() >= WINDOW_HEIGHT || bird.getY() - bird.getRadius() <= 0)
		{
			break;
		}

		cleardevice();
		bird.draw();
		FlushBatchDraw();

		Sleep(20);// FPS (FPS is about 50-ish, maybe)
	}
	EndBatchDraw();
	closegraph();


	return 0;
}