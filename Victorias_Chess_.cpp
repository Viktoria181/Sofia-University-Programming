#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctime>

using namespace std;

const unsigned int SIZE = 8;
const bool SHOW_INDICES = false;
const bool DEBUG = false;

const unsigned int SIZE_SQ = (SIZE * SIZE);

bool game = true;
bool playerTurn = true;

int board[SIZE_SQ];
int safeBoard[SIZE_SQ];
int compass[8];

int indexRookA = -1;
int indexRookB = -1;
int indexPlayerKing = -1;
int indexEnemyKing = -1;

void initArray(int arr[], int size, int num) {
	for (int i = 0; i < size; i++) {
		arr[i] = num;
	}
}

void logArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		cout << "[" << i << "]: " << arr[i] << endl;
	}
}

int findIndex(int row, int col) {
	return (row * SIZE + col);
}

int findCol(int index) {
	return (index % SIZE);
}

int findRow(int index) {
	return floor(index / SIZE);
}

bool isPlayerBetweenRookAndTarget(int rookIndex, int targetIndex) {
	int playerRow = findRow(indexPlayerKing);
	int playerCol = findCol(indexPlayerKing);

	int targetRow = findRow(targetIndex);
	int targetCol = findCol(targetIndex);

	int rookRow = findRow(rookIndex);
	int rookCol = findCol(rookIndex);

	if ((targetCol == rookCol) && (targetCol == playerCol)) {
		if ((playerRow < targetRow) && (playerRow > rookRow)) {
			return true;
		}
		else if ((playerRow > targetRow) && (playerRow < rookRow)) {
			return true;
		}
		else {
			return false;
		}
	}
	else if ((targetRow == rookRow) && (targetRow == playerRow)) {
		if ((playerCol < targetCol) && (playerCol > rookCol)) {
			return true;
		}
		else if ((playerCol > targetCol) && (playerCol < rookCol)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool rookFlankCollision(int rookIndex, int targetIndex)
{
	int rookRow = findRow(rookIndex);
	int rookCol = findCol(rookIndex);

	int targetRow = findRow(targetIndex);
	int targetCol = findCol(targetIndex);

	if (rookIndex < 0) {
		return false;
	}

	if (rookIndex == targetIndex) {
		return false;
	}

	if ((rookRow == targetRow) || (rookCol == targetCol)) {
		if (isPlayerBetweenRookAndTarget(rookIndex, targetIndex)) {
			if (DEBUG) {
				cout << "\nDEBUG: Player is between Rook and Enemy\n";
			}
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

bool compassCollision(int indexA, int indexB) {
	int rowA = findRow(indexA);
	int colA = findCol(indexA);

	int rowB = findRow(indexB);
	int colB = findCol(indexB);

	// same
	if (indexA == indexB) {
		return true;
	}

	// North
	if (rowA == (rowB + 1) && colA == colB) {
		return true;
	}

	// South
	if (rowA == (rowB - 1) && colA == colB) {
		return true;
	}

	// East
	if (rowA == rowB && colA == (colB - 1)) {
		return true;
	}

	// West
	if (rowA == rowB && colA == (colB + 1)) {
		return true;
	}

	// North-West
	if (rowA == (rowB + 1) && colA == (colB + 1)) {
		return true;
	}

	// North-East
	if (rowA == (rowB + 1) && colA == (colB - 1)) {
		return true;
	}

	// South-West
	if (rowA == (rowB - 1) && colA == (colB + 1)) {
		return true;
	}

	// South-East
	if (rowA == (rowB - 1) && colA == (colB - 1)) {
		return true;
	}

	return false;
}

void generateRooks() {
	indexRookA = rand() % SIZE_SQ;
	indexRookB = rand() % SIZE_SQ;

	while (indexRookA == indexRookB) {
		indexRookB = rand() % SIZE_SQ;

		if (DEBUG) {
			cout << "DEBUG: Reloading Rook" << endl;
		}
	}
}

void generatePlayerKing() {
	indexPlayerKing = rand() % SIZE_SQ;

	while ((indexPlayerKing == indexRookA) || (indexPlayerKing == indexRookB)) {
		indexPlayerKing = rand() % SIZE_SQ;

		if (DEBUG) {
			cout << "DEBUG: Reloading Player King" << endl;
		}
	}
}

int getSafeBoardSize() {
	unsigned int counter = 0;

	for (int i = 0; i < SIZE_SQ; i++) {
		if (safeBoard[i] > 0) {
			counter++;
		}
	}

	return counter;
}

void endGame()
{
	game = false;
}

void generateSafeZone()
{
	unsigned int counter = 0;

	for (int i = 0; i < SIZE_SQ; i++) {
		if (board[i] != indexRookA && board[i] != indexRookB) {
			if (!rookFlankCollision(indexRookA, board[i])) {
				if (!rookFlankCollision(indexRookB, board[i])) {
					if (!compassCollision(board[i], indexPlayerKing)) {
						safeBoard[counter] = board[i];
						counter++;
					}
				}
			}
		}
	}

	if (getSafeBoardSize() <= 0) {
		cout << "\n\nThere are no safe zones!\nExiting...\n";
		endGame();
	}
}

void generateEnemyKing()
{
	indexEnemyKing = safeBoard[rand() % getSafeBoardSize()];
}

void generateBoard()
{
	for (int i = 0; i < SIZE_SQ; i++) {
		board[i] = i;
	}

	generateRooks();

	generatePlayerKing();

	generateSafeZone();

	generateEnemyKing();
}

void displayColumnIndices()
{
	cout << endl
		<< "  ";
	for (int colInfo = 0; colInfo < SIZE; colInfo++) {
		cout << "  " << colInfo;
	}
	cout << endl;
}

void displayUpperBorder()
{
	cout << "  ";
	for (int upperBorder = 0; upperBorder < SIZE; upperBorder++) {
		cout << "---";
	}
	cout << "-" << endl;
}

void displayBoard()
{
	for (int row = 0; row < SIZE; row++) {
		cout << row << " ";

		for (int c = 0; c < SIZE; c++) {
			if (board[c + SIZE * row] == indexRookA) {
				cout << "| A";
			}
			else if (board[c + SIZE * row] == indexRookB) {
				cout << "| B";
			}
			else if (board[c + SIZE * row] == indexPlayerKing) {
				cout << "| K";
			}
			else if (board[c + SIZE * row] == indexEnemyKing) {
				cout << "| E";
			}
			else {
				if (SHOW_INDICES) {
					if (board[c + SIZE * row] < 10)
					{
						cout << "| " << board[c + SIZE * row];
					}
					else
					{
						cout << "|" << board[c + SIZE * row];
					}
				}
				else {
					cout << "|  ";
				}
			}
		}

		cout << "|" << endl
			<< "  ";

		for (int horizontals = 0; horizontals < SIZE; horizontals++) {
			cout << "---";
		}
		cout << "-" << endl;
	}

	cout << endl;
}

void display() {
	displayColumnIndices();

	displayUpperBorder();

	displayBoard();
}

bool boardOverflow(int targetRow, int targetCol) {
	if ((targetRow < 0) || (targetRow > (SIZE - 1))) {
		return true;
	}

	if ((targetCol < 0) || (targetCol > (SIZE - 1))) {
		return true;
	}

	return false;
}

bool bothAxesMovement(bool colChange, bool rowChange) {
	if (colChange && rowChange) {
		cout << "Error: can't change both axes at once!" << endl;

		return true;
	}
	else {
		return false;
	}
}

bool sameLocationMoveAttempt(int fromRow, int toRow, int fromCol, int toCol) {
	if ((fromRow == toRow) && (fromCol == toCol)) {
		cout << "Error: can't move to the same location!" << endl;

		return true;
	}
	else {
		return false;
	}
}

int findSteps(int from, int to) {
	return abs(from - to);
}

bool isPositionBlocked(int row, int col) {
	int position = board[findIndex(row, col)];

	if (position == indexRookA) {
		return true;
	}
	else if (position == indexRookB) {
		return true;
	}
	else if (position == indexPlayerKing) {
		return true;
	}
	else if (position == indexEnemyKing) {
		return true;
	}
	else {
		return false;
	}
}

bool moveRook(int rookIndex, int destinationRow, int destinationCol) {
	int originRow = -1;
	int originCol = -1;

	originRow = findRow(rookIndex);
	originCol = findCol(rookIndex);

	int steps = 0;

	char direction = 'X';

	bool positionBlocked = false;

	bool horizontalChange = (originCol != destinationCol);
	bool verticalChange = (originRow != destinationRow);

	if (boardOverflow(destinationRow, destinationCol)) {
		return false;
	}

	if (bothAxesMovement(horizontalChange, verticalChange)) {
		return false;
	}

	if (sameLocationMoveAttempt(originRow, destinationRow, originCol, destinationCol)) {
		return false;
	}

	if (horizontalChange) {
		steps = findSteps(originCol, destinationCol);

		if (originCol > destinationCol) {
			direction = 'L';
		}
		else {
			direction = 'R';
		}

		if (DEBUG) {
			cout << "DEBUG: horizontal change" << endl;
		}
	}
	else {
		steps = findSteps(originRow, destinationRow);

		if (originRow > destinationRow) {
			direction = 'U';
		}
		else {
			direction = 'D';
		}
		if (DEBUG) {
			cout << "DEBUG: vertical change" << endl;
		}
	}

	if (DEBUG) {
		cout << "DEBUG: direction: " << direction << endl;
		cout << "DEBUG: steps taken: " << steps << endl;
	}

	for (int i = 1; i <= steps; i++) {
		switch (direction) {
		case 'U':
			if (isPositionBlocked(originRow - i, originCol)) {
				positionBlocked = true;
			}
			break;
		case 'D':
			if (isPositionBlocked(originRow + i, originCol)) {
				positionBlocked = true;
			}
			break;
		case 'L':
			if (isPositionBlocked(originRow, originCol - i)) {
				positionBlocked = true;
			}
			break;
		case 'R':
			if (isPositionBlocked(originRow, originCol + i)) {
				positionBlocked = true;
			}
			break;
		default:
			cout << "Error: invalid direction" << endl;
			break;
		}

		if (positionBlocked) {
			break;
		}
	}

	if (positionBlocked) {
		cout << "Error: something is blocking the path" << endl;
		return false;
	}
	else {
		board[findIndex(originRow, originCol)] = findIndex(originRow, originCol);

		if (rookIndex == indexRookA) {
			indexRookA = findIndex(destinationRow, destinationCol);
		}
		else if (rookIndex == indexRookB) {
			indexRookB = findIndex(destinationRow, destinationCol);
		}

		return true;
	}
}

bool movePlayerKing(int targetRow, int targetCol) {
	int destinationIndex = findIndex(targetRow, targetCol);

	if (!compassCollision(indexPlayerKing, destinationIndex)) {
		cout << "Error: target destination out of reach" << endl;
		return false;
	}
	else {
		if ((destinationIndex == indexRookA) || (destinationIndex == indexRookB)) {
			cout << "Error: collision with rook detected" << endl;
			return false;
		}
		else if (compassCollision(indexEnemyKing, destinationIndex)) {
			cout << "Error: collision with enemy king detected" << endl;
			return false;
		}
		else {
			indexPlayerKing = destinationIndex;

			return true;
		}
	}
}

bool movePiece(char pieceChar, int targetRow, int targetCol) {
	switch (pieceChar) {
	case 'a':
	case 'A':
		if (indexRookA >= 0) {
			return moveRook(indexRookA, targetRow, targetCol);
		}
		else {
			cout << "Error: rook A is dead" << endl;
			return false;
		}
		break;

	case 'b':
	case 'B':
		if (indexRookB >= 0) {
			return moveRook(indexRookB, targetRow, targetCol);
		}
		else {
			cout << "Error: rook B is dead" << endl;
			return false;
		}
		break;

	case 'k':
	case 'K':
		return movePlayerKing(targetRow, targetCol);
		break;

	default:
		cout << "Error: movePiece invalid case" << endl;
		return false;
		break;
	}
}

void generateCompassAround(int targetIndex) {
	int row = findRow(targetIndex);
	int col = findCol(targetIndex);

	initArray(compass, 8, -1);

	if (!boardOverflow(row - 1, col - 1)) {
		compass[0] = findIndex(row - 1, col - 1);
	}

	if (!boardOverflow(row - 1, col)) {
		compass[1] = findIndex(row - 1, col);
	}

	if (!boardOverflow(row - 1, col + 1)) {
		compass[2] = findIndex(row - 1, col + 1);
	}

	if (!boardOverflow(row, col + 1)) {
		compass[3] = findIndex(row, col + 1);
	}

	if (!boardOverflow(row + 1, col + 1)) {
		compass[4] = findIndex(row + 1, col + 1);
	}

	if (!boardOverflow(row + 1, col)) {
		compass[5] = findIndex(row + 1, col);
	}

	if (!boardOverflow(row + 1, col - 1)) {
		compass[6] = findIndex(row + 1, col - 1);
	}

	if (!boardOverflow(row, col - 1)) {
		compass[7] = findIndex(row, col - 1);
	}

	if (DEBUG) {
		cout << "generateCompassAround(" << targetIndex << "): " << endl;
		logArray(compass, 8);
	}
}

bool isRookDefended(int rookIndex) {
	if (compassCollision(rookIndex, indexPlayerKing)) {
		return true;
	}

	if ((indexRookA <= 0) || (indexRookB <= 0)) {
		return false;
	}

	if (rookIndex == indexRookA) {
		if (rookFlankCollision(indexRookB, rookIndex)) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (rookIndex == indexRookB) {
		if (rookFlankCollision(indexRookA, rookIndex)) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

bool isIndexAttacked(int targetIndex) {
	if (isRookDefended(targetIndex)) {
		return true;
	}
	else if (rookFlankCollision(indexRookA, targetIndex)) {
		if (DEBUG) {
			cout << "Index attacked by rook A" << endl;
		}
		return true;
	}
	else if (rookFlankCollision(indexRookB, targetIndex)) {
		if (DEBUG) {
			cout << "Index attacked by rook B" << endl;
		}
		return true;
	}
	else if (compassCollision(targetIndex, indexPlayerKing)) {
		if (DEBUG) {
			cout << "Index attacked by player king" << endl;
		}
		return true;
	}
	else {
		return false;
	}
}

void markCompassSafety() {
	for (int c = 0; c < 8; c++) {
		if (isIndexAttacked(compass[c])) {
			compass[c] = -1;
		}
	}

	if (DEBUG) {
		cout << "markCompassSafety(): " << endl;
		logArray(compass, 8);
	}
}

int compassSafeZonesCount() {
	int safeZoneCount = 0;

	for (int c = 0; c < 8; c++) {
		if (compass[c] >= 0) {
			safeZoneCount++;
		}
	}

	return safeZoneCount;
}

void moveEnemyKing(int targetIndex) {
	indexEnemyKing = targetIndex;

	if (DEBUG) {
		cout << "Enemy King to " << targetIndex << endl;
	}

	cout << "Enemy move: " << endl;
	display();
}

void killRook(int rookIndex) {
	if (rookIndex == indexRookA) {
		indexRookA = -1;

		if (DEBUG) {
			cout << "Rook A kill" << endl;
		}
	}
	else if (rookIndex == indexRookB) {
		indexRookB = -1;

		if (DEBUG) {
			cout << "Rook B kill" << endl;
		}
	}
}

void logicEnemyKing() {
	generateCompassAround(indexEnemyKing);

	markCompassSafety();

	int numSafeZones = compassSafeZonesCount();
	if (DEBUG) {
		cout << "safe zones: " << numSafeZones << endl;
	}

	if (numSafeZones <= 0) {
		cout << "\n\nCheckmate!\nGame Over.\nExiting...\n";
		endGame();
		return;
	}

	// priority if a rook can be killed
	for (int k = 0; k < 8; k++) {
		if (compass[k] >= 0) {
			if ((compass[k] == indexRookA) || (compass[k] == indexRookB)) {
				if (!isRookDefended(compass[k])) {
					killRook(compass[k]);
					moveEnemyKing(compass[k]);
					return;
				}
			}
		}
	}

	// lesser priority, move at random
	int choice = rand() % numSafeZones;
	int counter = 0;
	for (int c = 0; c < 8; c++) {
		if (compass[c] >= 0) {
			if (choice == counter) {
				moveEnemyKing(compass[c]);

				if (DEBUG) {
					cout << "moving king randomly" << endl;
					cout << "compass[" << c << "]: " << compass[c] << endl;
				}

				break;
			}
			else {
				counter++;
			}
		}
	}
}

bool inputOverload() {
	if (cin.fail()) {
		cin.clear();

		cout << "\n\nINPUT ERROR!\nExiting...\n";
		endGame();

		return true;
	}
	else {
		return false;
	}
}

bool getPlayerInput() {
	char piece = 'X';
	int row = -1;
	int col = -1;

	cout << "select piece: ";
	cin >> piece;
	if (inputOverload()) {
		return false;
	}

	cout << "select row: ";
	cin >> row;
	if (inputOverload()) {
		return false;
	}

	cout << "select column: ";
	cin >> col;
	if (inputOverload()) {
		return false;
	}

	if (movePiece(piece, row, col)) {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	srand(time(NULL));

	initArray(board, SIZE_SQ, 0);
	initArray(safeBoard, SIZE_SQ, 0);
	initArray(compass, 8, -1);

	generateBoard();

	display();

	while (game) {
		if (getPlayerInput()) {
			cout << "\nPlayer move: \n";
			display();

			logicEnemyKing();
		}
	}
}