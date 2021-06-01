#include "Core/Application.h"

struct Cell {
	bool alive;
	int x, y;

	Cell(int x, int y, bool alive)
		: x(x), y(y), alive(alive) { }
	Cell() { }
};

class GameOfLife : public Ember::Application {
public:
	void OnCreate() { 
		for (int x = 0; x < GRID_WIDTH; x++) {
			for (int y = 0; y < GRID_HEIGHT; y++) {
				cell_grid[x][y] = Cell(x, y, false);
				cell_grid_next[x][y] = Cell(x, y, false);
			}
		}
	}

	virtual ~GameOfLife() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		if (play) {
			if (delay_counter % DELAY == 0) {
				//The rules of life go here...

				for (int x = 0; x < GRID_WIDTH; x++) {
					for (int y = 0; y < GRID_HEIGHT; y++) {
						int neighbor_count = 0;

						for (int neighbor = 0; neighbor < 8; neighbor++) 
							if(x + NEIGHBOR_OFFSETS[neighbor].x >= 0 && x + NEIGHBOR_OFFSETS[neighbor].x < GRID_WIDTH)
								if (y + NEIGHBOR_OFFSETS[neighbor].y >= 0 && y + NEIGHBOR_OFFSETS[neighbor].y < GRID_HEIGHT)
									if (cell_grid[(x + NEIGHBOR_OFFSETS[neighbor].x)][(y + NEIGHBOR_OFFSETS[neighbor].y)].alive)
										neighbor_count++;

						bool new_state = false;
						bool current_state = cell_grid[x][y].alive;

						if (current_state && neighbor_count < 2)
							new_state = false;
						if(current_state && (neighbor_count == 2 || neighbor_count == 3))
							new_state = true;
						if (current_state && neighbor_count > 3)
							new_state = false;
						if (!current_state && neighbor_count == 3)
							new_state = true;

						//cell_grid[x][y].alive = new_state;
						cell_grid_next[x][y].alive = new_state;
					}
				}

				for (int x = 0; x < GRID_WIDTH; x++) {
					for (int y = 0; y < GRID_HEIGHT; y++) {
						cell_grid[x][y].alive = cell_grid_next[x][y].alive;
						cell_grid_next[x][y].alive = false;
					}
				}

				delay_counter = 0;
			}
			delay_counter++;
		}

		for (int y = 0; y < GRID_HEIGHT; y++) {
			for (int x = 0; x < GRID_WIDTH; x++) {
				Uint8 color = (cell_grid[x][y].alive) ? 0 : 255;
				renderer->Rectangle(Ember::Rect({ x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT }), { color, color, color, 255 });
				renderer->Border(Ember::Rect({ x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT }), { 20, 20, 20, 255 });
			}
		}

		renderer->Show();

	}

	bool KeyboardEvent(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == Ember::EmberKeyCode::Return && keyboard.pressed) {
			play = !play;
		}
		if (keyboard.scancode == Ember::EmberKeyCode::C && keyboard.pressed) {
			for (int x = 0; x < GRID_WIDTH; x++) {
				for (int y = 0; y < GRID_HEIGHT; y++) {
					cell_grid[x][y].alive = false;
				}
			}

			play = false;
		}

		return false;
	}

	bool MouseButtonEvent(Ember::MouseButtonEvents& mouse) {
		int selected_x = events->MousePosition().x / CELL_WIDTH;
		int selected_y = events->MousePosition().y / CELL_HEIGHT;

		if (selected_x >= 0 && selected_y >= 0 && selected_x < GRID_WIDTH && selected_y < GRID_HEIGHT && mouse.clicked && mouse.button_id == Ember::ButtonIds::LeftMouseButton) {
			cell_grid[selected_x][selected_y].alive = !cell_grid[selected_x][selected_y].alive;
		}

		return false;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		dispatch.Dispatch<Ember::MouseButtonEvents>(EMBER_BIND_FUNC(MouseButtonEvent));
		dispatch.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(KeyboardEvent));
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	static const int GRID_WIDTH = 128;
	static const int GRID_HEIGHT = 80;

	const int CELL_WIDTH = 16;
	const int CELL_HEIGHT = 16;

	Cell cell_grid[GRID_WIDTH][GRID_HEIGHT];
	Cell cell_grid_next[GRID_WIDTH][GRID_HEIGHT];
	bool play = false;
	int delay_counter = 0;
	const int DELAY = 10;

	const Ember::IVec2 NEIGHBOR_OFFSETS[8] = {
		{ 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 },
		{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }
	};
};

int main(int argc, char** argv) {
	GameOfLife game_of_life;
	game_of_life.Initialize("Game of Life", false, 1024, 768);

	game_of_life.Run();

	return 0;
}