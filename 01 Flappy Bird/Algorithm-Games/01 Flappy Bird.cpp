#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <cstdlib>
#include <random>

using namespace std;

random_device rd;
mt19937_64 gen(rd());

class Bird {
private:
	double x;
	double y;
	double vy = 0;

	const int radius = 15;

	const double gravity = 0.6;
	const double jump_velocity = -7.0;

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

	// Access for x, y, and radius
	double get_x() const {
		return x;
	}

	double get_y() const {
		return y;
	}

	int get_radius() const {
		return radius;
	}
};

class Pipe {
private:
	// Basic properties
	double up_left_x;
	int top_height;
	int window_height;
	
	const int w = 60;
	const int gap = 190;

	const double speed = 3.0;

public:
	Pipe(double start_x, int win_height) {
		up_left_x = start_x;
		window_height = win_height;

		reset(start_x);
	}

	// Resetting the pipe's hieght from 90 to max, randomly
	void reset(double new_x) {
		up_left_x = new_x;
		int min_height = 80;
		int max_height = window_height - gap - 80;
		
		uniform_int_distribution<int> dist(min_height, max_height);
		top_height = min_height + dist(gen);
	}

	// Updating every frame
	void update() {
		up_left_x -= speed;
	}

	// Drawing the pipe
	void draw() const {
		setfillcolor(GREEN);
		solidrectangle(static_cast<int>(up_left_x), 0, static_cast<int>(up_left_x + w), top_height);
		solidrectangle(static_cast<int>(up_left_x), top_height + gap, static_cast<int>(up_left_x + w), window_height);
	}

	// Checking if the pipe has flown out of the left side
	bool is_out_of_screen() const {
		return (up_left_x + w) < 0;
	}

	// Checking for collisions with bird
	bool check_collisions(const Bird& bird) const {
		double bx = bird.get_x();
		double by = bird.get_y();
		double br = bird.get_radius();

		if (bx + br > up_left_x && bx - br < up_left_x + w) {
			if (by - br < top_height || by + br > top_height + gap) {
				return true;
			}
		}

		return false;
	}

	// Access for up_left_x
	double get_up_left_x() const {
		return up_left_x;
	}
};

// Key Press Helper
inline bool isKeyPressed(int virtualKey) {
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

int main(void) {
	// The RNG engine
	uniform_int_distribution<int> dist(0, 100);
	int random_number = dist(gen);
	const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600; // Set width and height

	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT); // Initialize screen
	BeginBatchDraw(); // Batches

	Bird bird(150, 200);
	Pipe pipe(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (true) {
		// Input
		if (isKeyPressed(VK_SPACE)) {
			bird.jump();
		}

		// Updating
		bird.update();
		pipe.update();

		if (pipe.is_out_of_screen()) {
			pipe.reset(WINDOW_WIDTH);
		}

		// Out of screen or hit pipe
		if (bird.get_y() + bird.get_radius() >= WINDOW_HEIGHT || bird.get_y() - bird.get_radius() <= 0 || pipe.check_collisions(bird)) {
			break; 
		}

		cleardevice();

		bird.draw();
		pipe.draw();

		FlushBatchDraw();

		// Setting the FPS
		Sleep(20);
	}
	EndBatchDraw();
	closegraph();


	return 0;
}