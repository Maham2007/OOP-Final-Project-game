#ifndef POSITION_H
#define POSITION_H

class Position {
public:
	int x, y;
	int getX() { return x; }
	int getY() { return y; }
	Position(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}
};
#endif