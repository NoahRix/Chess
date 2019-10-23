/*******************************************************************
Programmer:    Noah Rix

Date started:  October 1, 2019

Synopsis:      This applet is a chess game interface using SFML.
			   The current version of this applet allows the user
			   to play as white against the black opponent. As of
			   now the AI is completely random.  
********************************************************************/
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

/*******************************************************************
Class:        ChessPiece

Children:     Pawn, Bishop, Knight, Rook, Queen, King.

Use:          This is the base class for all chess pieces. 
              It holds the SFML fields that are responsible for the 
			  piece sprites. Every chess piece will be loaded from the
			  same png image but from a different bounded region of that
			  image. Every chess piece type will be determined by a two 
			  character string where the first character is the piece 
			  type and the second is the piece color. For example, 'pw'
			  refers to a white pawn and 'rb' refers to a black rook.
			

Key Members:  One string named type; type holds the values for the piece
              type and color. 

			  One integer named materialValue; materialValue is a
			  numerical value for each piece. This is used for determining 
			  best moves.

			  One vector of sfml intger pairs (sf::vector2i) named path;
			  path holds what essentially are directions where the piece 
			  can move. These values will be passed into a recursive 
			  function later on.

********************************************************************/

class ChessPiece {
protected:
	int materialValue;
	string type = "  ";
	sf::Vector2f imagePos;
	sf::Vector2f pos;
	sf::Vector2f squareSize;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect area;
	vector<sf::Vector2i> path;
public:
	ChessPiece() {
		area = sf::IntRect(0, 0, 1, 1);
		image.loadFromFile("Sprites/Pieces/tilemap.png");
	}
	sf::Vector2f getPosition() {
		return pos;
	}
	sf::Vector2f getSquareSize() {
		squareSize = sf::Vector2f(75, 75);
		return squareSize;
	}
	string getType() {
		return type;
	}
	vector<sf::Vector2i> getPath() {
		return path;
	}
	void setPostion(sf::Vector2f _pos) {
		cout << "image pos" << imagePos.x << ", " << imagePos.y << endl;
		cout << "pos" << _pos.x << ", " << _pos.y << endl;
		imagePos = sf::Vector2f(_pos.x * getSquareSize().x, _pos.y * getSquareSize().y);
		pos = _pos;
	}
	sf::Sprite getSprite() {
		sprite.setPosition(imagePos);
		sprite.setOrigin(-10, -10);
		texture.loadFromImage(image, area);
		sprite.setTexture(texture);
		return sprite;
	}
	sf::FloatRect getGlobalBounds() {
		return sprite.getGlobalBounds();
	}
};

class Pawn : public ChessPiece {
public:
	Pawn(string _type, sf::Vector2f _pos, sf::Vector2f _squareSize) {
		materialValue = 0;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * _squareSize.x, _pos.y * _squareSize.y);
		type = _type;

		if (_type == "pw")
			area = sf::IntRect(225, 0, 45, 45);
		if (_type == "pb")
			area = sf::IntRect(225, 45, 45, 45);
	}
};

class Bishop : public ChessPiece {
public:
	Bishop(string _type, sf::Vector2f _pos, sf::Vector2f _squareSize) {
		materialValue = 3;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * _squareSize.x, _pos.y * _squareSize.y);
		type = _type;

		path.push_back(sf::Vector2i(1, 1));
		path.push_back(sf::Vector2i(-1, 1));
		path.push_back(sf::Vector2i(1, -1));
		path.push_back(sf::Vector2i(-1, -1));

		if (_type == "bw")
			area = sf::IntRect(90, 0, 45, 45);
		if (_type == "bb")
			area = sf::IntRect(90, 45, 45, 45);
	}
};

class Knight : public ChessPiece {
public:
	Knight(string _type, sf::Vector2f _pos, sf::Vector2f squareSize) {
		materialValue = 3;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * squareSize.x, _pos.y * squareSize.y);
		type = _type;

		path.push_back(sf::Vector2i(1, 2));
		path.push_back(sf::Vector2i(1, -2));
		path.push_back(sf::Vector2i(-1, 2));
		path.push_back(sf::Vector2i(-1, -2));
		path.push_back(sf::Vector2i(2, 1));
		path.push_back(sf::Vector2i(2, -1));
		path.push_back(sf::Vector2i(-2, 1));
		path.push_back(sf::Vector2i(-2, -1));

		if (_type == "nw")
			area = sf::IntRect(135, 0, 45, 45);
		if (_type == "nb")
			area = sf::IntRect(135, 45, 45, 45);
	}
};

class Rook : public ChessPiece {
public:
	Rook(string _type, sf::Vector2f _pos, sf::Vector2f _squareSize) {
		materialValue = 5;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * _squareSize.x, _pos.y * _squareSize.y);
		type = _type;

		path.push_back(sf::Vector2i(1, 0));
		path.push_back(sf::Vector2i(0, 1));
		path.push_back(sf::Vector2i(-1, 0));
		path.push_back(sf::Vector2i(0, -1));

		if (_type == "rw")
			area = sf::IntRect(180, 0, 45, 45);
		if (_type == "rb")
			area = sf::IntRect(180, 45, 45, 45);
	}
};

class Queen : public ChessPiece {
public:
	Queen(string _type, sf::Vector2f _pos, sf::Vector2f _squareSize) {
		materialValue = 9;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * _squareSize.x, _pos.y * _squareSize.y);
		type = _type;

		path.push_back(sf::Vector2i(1, 1));
		path.push_back(sf::Vector2i(-1, 1));
		path.push_back(sf::Vector2i(1, -1));
		path.push_back(sf::Vector2i(-1, -1));
		path.push_back(sf::Vector2i(1, 0));
		path.push_back(sf::Vector2i(0, 1));
		path.push_back(sf::Vector2i(-1, 0));
		path.push_back(sf::Vector2i(0, -1));

		if (_type == "qw")
			area = sf::IntRect(45, 0, 45, 45);
		if (_type == "qb")
			area = sf::IntRect(45, 45, 45, 45);
	}
};

class King : public ChessPiece {
public:
	King(string _type, sf::Vector2f _pos, sf::Vector2f _squareSize) {
		materialValue = 3;
		pos = _pos;
		imagePos = sf::Vector2f(_pos.x * _squareSize.x, _pos.y * _squareSize.y);
		type = _type;
	
		path.push_back(sf::Vector2i(1, 1));
		path.push_back(sf::Vector2i(-1, 1));
		path.push_back(sf::Vector2i(1, -1));
		path.push_back(sf::Vector2i(-1, -1));
		path.push_back(sf::Vector2i(1, 0));
		path.push_back(sf::Vector2i(0, 1));
		path.push_back(sf::Vector2i(-1, 0));
		path.push_back(sf::Vector2i(0, -1));

		if (_type == "kw")
			area = sf::IntRect(0, 0, 45, 45);
		if (_type == "kb")
			area = sf::IntRect(0, 45, 45, 45);
	}
};

/*******************************************************************
Class:        Tile

Children:     None.

Use:          This class is the essential building block of the chessboard.
			  Tile tells you what piece it's currently holding, it's own unique
			  position, if it's occupied, and if it's valid for move decisons. 


Key Members:  One string named type; type holds the values for the piece
			  type and color.

			  One integer named materialValue; materialValue is a
			  numerical value for each piece. This is used for determining
			  best moves.

			  One vector of sfml intger pairs (sf::vector2i) named path;
			  path holds what essentially are directions where the piece
			  can move. These values will be passed into a recursive
			  function later on.
********************************************************************/
class Tile {
public:
	ChessPiece piece;
	sf::RectangleShape square;
	sf::Vector2i position;
	sf::Color temp;
	bool hasValidMove = false;
	bool isOccupied;
	bool isValid = false;
	
	Tile() {
		cout << "Tile created as empty." << endl;
	}
	void setPiece(ChessPiece _piece) {
		piece = _piece;
	}
	void setPieceByType(const string &type, sf::Vector2f pos, sf::Vector2f squareSize) {
		isOccupied = true;
		if (type == "pw") {
			piece = Pawn(type, pos, squareSize);
		}
		if (type == "pb") {
			piece = Pawn(type, pos, squareSize);
		}
		if (type == "rw") {
			piece = Rook(type, pos, squareSize);
		}
		if (type == "rb") {
			piece = Rook(type, pos, squareSize);
		}
		if (type == "nw") {
			piece = Knight(type, pos, squareSize);
		}
		if (type == "nb") {
			piece = Knight(type, pos, squareSize);
		}
		if (type == "bw") {
			piece = Bishop(type, pos, squareSize);
		}
		if (type == "bb") {
			piece = Bishop(type, pos, squareSize);
		}
		if (type == "qw") {
			piece = Queen(type, pos, squareSize);
		}
		if (type == "qb") {
			piece = Queen(type, pos, squareSize);
		}
		if (type == "kw") {
			piece = King(type, pos, squareSize);
		}
		if (type == "kb") {
			piece = King(type, pos, squareSize);
		}
		if (piece.getType() == "  ")
			isOccupied = false;
	}
	sf::Vector2i getPosition() {
		return position;
	}
	void setPostion(sf::Vector2i _position) {
		position = _position;
	}
	ChessPiece getPiece() {
		return piece;
	}
};

class Chessboard {
private:
	const int color[8][8] = {
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
	};
	const string piecePlacement[8][8] = {
		{"rb","nb","bb","qb","kb","bb","nb","rb"},
		{"pb","pb","pb","pb","pb","pb","pb","pb"},
		{"  ","  ","  ","  ","  ","  ","  ","  "},
		{"  ","  ","  ","  ","  ","  ","  ","  "},
		{"  ","  ","  ","  ","  ","  ","  ","  "},
		{"  ","  ","  ","  ","  ","  ","  ","  "},
		{"pw","pw","pw","pw","pw","pw","pw","pw"},
		{"rw","nw","bw","qw","kw","bw","nw","rw"}
	};

public:
	int iter = 0;
	int turn = 0;
	char turnColor;
	bool hasValidMove;
	vector<vector<Tile>> tiles;
	vector<Tile> currentValidMoves;
	vector<Tile> randomValidMoves;
	vector<Tile> tileSelection;
	vector<Tile> oppTiles;
	vector<Tile> row;
	Tile temp;
	sf::Vector2f selectedPosition;
	sf::Clock clock;
	sf::Time time;

	Chessboard() {
	}
	Chessboard(unsigned int height) {
		int scalar = height / 8;
		sf::Vector2i squareSize = sf::Vector2i(scalar, scalar);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
				row.push_back(temp);
			tiles.push_back(row);
		}

		initiateBoard(scalar);
		
	}
	
	bool getHasValidMove() {
		return hasValidMove;
	}
	bool inBounds(int i, int j, int x, int y) {
		return (i + x >= 0 && i + x <= 7 && j + y >= 0 && j + y <= 7);
	}
	bool isOpposing(ChessPiece p1, ChessPiece p2) {
		return (p1.getType()[1] != p2.getType()[1]);
	}
	bool isOpposingPiece(ChessPiece p1, ChessPiece p2) {
		return (p1.getType()[1] != p2.getType()[1]);
	}
	bool mouseClickedonTile(Tile &t, sf::FloatRect mouse) {
		bool mouseOnPiece = mouse.intersects(t.piece.getSprite().getGlobalBounds());
		bool mouseOnTile = mouse.intersects(t.square.getGlobalBounds());
		return (mouseOnPiece || mouseOnTile);
	}
	bool tracebackPeerSupport(Tile &tile) {
		calculateValidMoves(tile);
		return false;
	}
	sf::Vector2i getPath(Tile &tile, int &n) {
		return tile.piece.getPath()[n];
	}
	void calculateValidMoves(Tile tile) {
		int limitedDistance = 8;
		int i = tile.getPosition().x;
		int j = tile.getPosition().y;
		currentValidMoves.clear();
		validateBoard();

		switch (tile.piece.getType()[0]) {
			case 'p': {
				if (tile.piece.getType()[1] == 'b') {
					if (tiles[i][j].piece.getPosition().y == 1) {
						limitedDistance = 2;
						displayValidMoves(i, j, 0, 1, limitedDistance, tiles, tile.piece);
					}
					else {
						limitedDistance = 1;
						displayValidMoves(i, j, 0, 1, limitedDistance, tiles, tile.piece);
					}
				}
				if (tile.piece.getType()[1] == 'w') {
					if (tiles[i][j].piece.getPosition().y == 6) {
						limitedDistance = 2;
						displayValidMoves(i, j, 0, -1, limitedDistance, tiles, tile.piece);
					}
					else {
						limitedDistance = 1;
						displayValidMoves(i, j, 0, -1, limitedDistance, tiles, tile.piece);
					}
				}
				tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				break;
			}
			case 'b': {
				if (tile.piece.getPosition().x == i && tile.piece.getPosition().y == j) {
					for (int n = 0; n < tile.piece.getPath().size(); n++) {
						limitedDistance = 7;
						displayValidMoves(i, j, getPath(tile, n).x, getPath(tile, n).y, limitedDistance, tiles, tile.piece);
					}
					tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				}
				break;
			}
			case 'n': {
				if (tile.piece.getPosition().x == i && tile.piece.getPosition().y == j) {
					for (int n = 0; n < tile.piece.getPath().size(); n++) {
						limitedDistance = 1;
						displayValidMoves(i, j, getPath(tile, n).x, getPath(tile, n).y, limitedDistance, tiles, tile.piece);
					}
					tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				}
				break;
			}
			case 'r': {
				if (tile.piece.getPosition().x == i && tile.piece.getPosition().y == j) {
					for (int n = 0; n < tile.piece.getPath().size(); n++) {
						limitedDistance = 7;
						displayValidMoves(i, j, getPath(tile, n).x, getPath(tile, n).y, limitedDistance, tiles, tile.piece);
					}
					tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				}
				break;
			}
			case 'q': {
				if (tile.piece.getPosition().x == i && tile.piece.getPosition().y == j) {
					for (int n = 0; n < tile.piece.getPath().size(); n++) {
						limitedDistance = 7;
						displayValidMoves(i, j, getPath(tile, n).x, getPath(tile, n).y, limitedDistance, tiles, tile.piece);
					}
					tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				}
				break;
			}
			case 'k': {
				if (tile.piece.getPosition().x == i && tile.piece.getPosition().y == j) {
					for (int n = 0; n < tile.piece.getPath().size(); n++) {
						limitedDistance = 1;
						displayValidMoves(i, j, getPath(tile, n).x, getPath(tile, n).y, limitedDistance, tiles, tile.piece);
					}
					tiles[i][j].square.setFillColor(sf::Color(0, 255, 255));
				}
				break;
			}
		}
	}
	void clearHighlightedTiles(){
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				if (color[i][j] == 1)
					tiles[i][j].square.setFillColor(sf::Color::White);
				else
					tiles[i][j].square.setFillColor(sf::Color(120, 120, 120));
			}
		}
	void computeRandomMove() {
		int rp;
		time = clock.getElapsedTime();
		validateOppTiles(oppTiles);
		rp = rand() % oppTiles.size();

		if (time >= sf::milliseconds(0)) {
			rp++;
			validateBoard();
			clearHighlightedTiles();
			calculateValidMoves(oppTiles[rp]);
			if (currentValidMoves.size() > 0) {
				int r = rand() % currentValidMoves.size();
				movePiece(oppTiles[rp], currentValidMoves[r]);
				currentValidMoves.clear();
				clearHighlightedTiles();
				oppTiles[rp].square.setFillColor(sf::Color(0, 255, 255));
				currentValidMoves[r].square.setFillColor(sf::Color(0, 255, 0));
			}
			clock.restart();
		}
		rp = rand() % oppTiles.size();
		if (iter >= oppTiles.size()) 
			iter = 0;
	}
	void displayValidMoves(int i, int j, int x, int y, int &distance, vector<vector<Tile>> &tiles, ChessPiece piece) {
		i += x;
		j += y;
		if (piece.getType()[0] != 'n' && piece.getType()[0] != 'p' && inBounds(i, j, 0, 0) && distance > 0) {
			if (!tiles[i][j].isOccupied) {
				fillGreen(tiles[i][j]);
				distance--;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
			if (tiles[i][j].isOccupied && isOpposing(piece, tiles[i][j].piece)) {
				fillYellow(tiles[i][j]);
				distance = 0;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
			if (tiles[i][j].isOccupied && !isOpposing(piece, tiles[i][j].piece)) {
				fillRed(tiles[i][j]);
				distance = 0;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
		}
		if (piece.getType()[0] == 'n' && inBounds(i, j, 0, 0) && distance > 0) {
			if (!tiles[i][j].isOccupied) {
				fillGreen(tiles[i][j]);
				distance--;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
			if (tiles[i][j].isOccupied && !isOpposing(piece, tiles[i][j].piece)) {
				fillRed(tiles[i][j]);
				distance = 0;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
			if (tiles[i][j].isOccupied && isOpposing(piece, tiles[i][j].piece)) {
				fillYellow(tiles[i][j]);
				distance = 0;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
		}
		if (piece.getType()[0] == 'p' && inBounds(i, j, 0, 0) && distance > 0) {
			if (!tiles[i][j].isOccupied) {
				fillGreen(tiles[i][j]);
				distance--;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
			if (i + 1 >= 0 && i + 1 <= 7)
				if ((tiles[i + 1][j].isOccupied) && isOpposing(piece, tiles[i + 1][j].piece)) {
					fillYellow(tiles[i + 1][j]);
				}
			if (i - 1 >= 0 && i - 1 <= 7)
				if ((tiles[i - 1][j].isOccupied) && isOpposing(piece, tiles[i - 1][j].piece)) {
					fillYellow(tiles[i - 1][j]);
				}
			if (tiles[i][j].isOccupied) {
				fillRed(tiles[i][j]);
				distance = 0;
				displayValidMoves(i, j, x, y, distance, tiles, piece);
			}
		}
	}
	void draw(sf::RenderWindow &window) {
		for (int i = 0; i < 8; i++)
			for (int k = 0; k < 8; k++) {
				window.draw(tiles[i][k].square);
				window.draw(tiles[i][k].piece.getSprite());
			}
	}
	void fillGreen(Tile &t) {
		t.square.setFillColor(sf::Color::Green);
		t.isValid = true;
		hasValidMove = true;
		currentValidMoves.push_back(t);
		}
	void fillRed(Tile &t) {
		t.square.setFillColor(sf::Color::Red);
		t.isValid = false;
		hasValidMove = false;
	}
	void fillYellow(Tile &t) {
		t.square.setFillColor(sf::Color::Yellow);
		t.isValid = true;
		hasValidMove = true;
		currentValidMoves.push_back(t);
	}
	void filterOutInvalidMoves(vector<Tile> in, vector<Tile> &out) {
		for(int i = 0; i < in.size(); i++){
			currentValidMoves.clear();
			calculateValidMoves(in[i]);
			if (currentValidMoves.size() > 0)
				out.push_back(in[i]);
		}
	}
	void filterValid(vector<Tile> & in, vector<Tile> &out) {
		for (int i = 0; i < in.size(); i++) {
			calculateValidMoves(in[i]);
			if (currentValidMoves.size() <= 0) {
				out.push_back(in[i]);
			}
		}
	}
	void highlightValidTiles() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				if (tiles[i][j].isValid) {
					tiles[i][j].square.setFillColor(sf::Color::Transparent);
				}
			}
	}
	//Set chessboard tiles along with their positions, sprite positions, piece positions and coorisponding colors.
	void initiateBoard(const int &scalar) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				clearHighlightedTiles();
				tiles[i][j].setPostion(sf::Vector2i(i, j));
				tiles[i][j].square.setSize(sf::Vector2f(scalar, scalar));
				tiles[i][j].square.setPosition(sf::Vector2f(i * scalar, j * scalar));
				tiles[i][j].setPieceByType(piecePlacement[j][i], sf::Vector2f(tiles[i][j].getPosition()), tiles[i][j].square.getSize());
			}
	}
	void movePiece(Tile &t1, Tile &t2) {
		sf::Vector2f newPosition = sf::Vector2f(tiles[t2.getPosition().x][t2.getPosition().y].getPosition());
		if (isOpposingPiece(t1.piece, t2.piece)) {
			swap(tiles[t2.getPosition().x][t2.getPosition().y].piece, tiles[t1.getPosition().x][t1.getPosition().y].piece);
			tiles[t2.getPosition().x][t2.getPosition().y].piece.setPostion(newPosition);
			tiles[t2.getPosition().x][t2.getPosition().y].isOccupied = true;
			tiles[t1.getPosition().x][t1.getPosition().y].isOccupied = false;
			tiles[t1.getPosition().x][t1.getPosition().y].setPiece(ChessPiece());
			validateBoard();
			turn++;
		}
	}
	void onClick(sf::Vector2i mCoord) {
		sf::FloatRect mouse;
		mouse = sf::FloatRect(mCoord.x, mCoord.y, 1, 1);

		if (turn % 2 == 0)
			turnColor = 'w';
		else
			turnColor = 'b';

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (mouseClickedonTile(tiles[i][j], mouse)) {
					clearHighlightedTiles();
					tileSelection.push_back(tiles[i][j]);
					calculateValidMoves(tiles[i][j]);
				}

		if (tileSelection.size() == 2) {
			clearHighlightedTiles();
			if (tileSelection[0].piece.getType()[1] == turnColor && tileSelection[1].isValid) {
				validateBoard();
				movePiece(tileSelection[0], tileSelection[1]);
			}
			tileSelection.clear();
		}
	}
	void validateBoard() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				tiles[i][j].isValid = false;
	}
	void validateOppTiles(vector<Tile> &_tiles) {
		_tiles.clear();
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (tiles[i][j].piece.getType()[1] == 'b')
					_tiles.push_back(tiles[i][j]);
	}
};

int main() {
	unsigned int height = 600;
	unsigned int width = 800;
	bool isClicked = false;
	Chessboard chessboard(height);

	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(width, height), "window");
	window.setFramerateLimit(30);
	while (window.isOpen()) {
		sf::Vector2i mCoord = sf::Mouse::getPosition(window);
		window.clear();
		chessboard.draw(window);
		if (chessboard.turn % 2 == 1) {
			chessboard.computeRandomMove();
		}
		window.display();
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				chessboard.onClick(mCoord);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}