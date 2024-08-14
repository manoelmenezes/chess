#include <iostream>
#include <exception>
#include <map>
#include<array>
#include <cmath>

using namespace std;

enum class Color {B, W};
enum class Piece {K, Q, R, H, B, P};

class Position {
    int row, column;
  public:
    Position(int row, int column) {
      this->row = row;
      this->column = column;
    }
    int getRow() {
      return row;
    }
    int getColumn() {
      return column;
    }
};
class Board;

class MoveResult {
  public:
    bool canMove;
    bool promotion;
    bool enpassant;
    bool capturedEnpassant;
    bool smallCastle;
    bool bigCastle;
    MoveResult(bool canMove, bool promotion, bool enpassant, bool capturedEnpassant, bool smallCastle, bool bigCastle) {
      this->canMove = canMove;
      this->promotion = promotion;
      this->enpassant = enpassant;
      this->capturedEnpassant = capturedEnpassant;
      this->smallCastle = smallCastle;
      this->bigCastle = bigCastle;
    }
};

class Result {
  public:
    bool promotion;
    bool check;
    bool checkmate;
    bool canMove;
    Result(bool promotion, bool check, bool checkmate, bool canMove) {
      this->promotion = promotion;
      this->check = check;
      this->checkmate = checkmate;
      this->canMove = canMove;
    }
};

class MoveCalculator {
  public:
    virtual MoveResult canMove(Board* board, Position& from, Position& to) = 0;
};

class BPiece {
    Piece piece;
    Color color;
  public:
    BPiece(Piece piece, Color color) {
      this->piece = piece;
      this->color = color;
    }
    Piece getPiece() {
      return piece;
    }
    Color getColor() {
      return color;
    }
    void setPiece(Piece p) {
      piece = p;
    }

    /*
     * At this moment:
     * 1. from and to positions are valid.
     * 2. from position is not empty.
     * 3. from and to position are different.
     * 4. from position is the current player.
     * 5. from position is the current piece (this object).
     * So this method validates if the piece at 'from' position
     * can move to 'to' position.
     */

    MoveResult canMove(Board* b, Position& from, Position& to);

};

class Board {
    BPiece* b[8][8];
    Color current;
    Position enPassant = Position(-1, -1);
    bool blackKingMoved = false;
    bool blackLeftRookMoved = false;
    bool blackRightRookMoved = false;
    bool whiteKingMoved = false;
    bool whiteLeftRookMoved = false;
    bool whiteRightRookMoved = false;
    Position promotion = Position(-1, -1);
    bool checkmate = false;
    std::map<Piece, MoveCalculator*> calculators;
    Position validatePosition(string p) {
      if (p.size() != 2) {
        throw logic_error("Position must have two chars, e.g., a2!");
      }
      char column = p[0];
      if (column < 'a' || column > 'h') {
        throw logic_error("Column must be in [a,h]!");
      }
      char row = p[1];
      if (row < '1' || row > '8') {
        throw logic_error("Row must be in [1,8]!");
      }
      return Position('8' - row, column - 'a');
    }
    void init();
  public:
    Board() {

      enPassant = Position(-1, -1);
      current = Color::W; 

      b[0][0] = new BPiece(Piece::R, Color::B);
      b[0][1] = new BPiece(Piece::H, Color::B);
      b[0][2] = new BPiece(Piece::B, Color::B);
      b[0][3] = new BPiece(Piece::Q, Color::B);
      b[0][4] = new BPiece(Piece::K, Color::B);
      b[0][5] = new BPiece(Piece::B, Color::B);
      b[0][6] = new BPiece(Piece::H, Color::B);
      b[0][7] = new BPiece(Piece::R, Color::B);

      b[1][0] = new BPiece(Piece::P, Color::B);
      b[1][1] = new BPiece(Piece::P, Color::B);
      b[1][2] = new BPiece(Piece::P, Color::B);
      b[1][3] = new BPiece(Piece::P, Color::B);
      b[1][4] = new BPiece(Piece::P, Color::B);
      b[1][5] = new BPiece(Piece::P, Color::B);
      b[1][6] = new BPiece(Piece::P, Color::B);
      b[1][7] = new BPiece(Piece::P, Color::B);

      b[2][0] = nullptr;
      b[2][1] = nullptr;
      b[2][2] = nullptr;
      b[2][3] = nullptr;
      b[2][4] = nullptr;
      b[2][5] = nullptr;
      b[2][6] = nullptr;
      b[2][7] = nullptr;
      
      b[3][0] = nullptr;
      b[3][1] = nullptr;
      b[3][2] = nullptr;
      b[3][3] = nullptr;
      b[3][4] = nullptr;
      b[3][5] = nullptr;
      b[3][6] = nullptr;
      b[3][7] = nullptr;
      
      b[4][0] = nullptr;
      b[4][1] = nullptr;
      b[4][2] = nullptr;
      b[4][3] = nullptr;
      b[4][4] = nullptr;
      b[4][5] = nullptr;
      b[4][6] = nullptr;
      b[4][7] = nullptr;
     
      b[5][0] = nullptr;
      b[5][1] = nullptr;
      b[5][2] = nullptr;
      b[5][3] = nullptr;
      b[5][4] = nullptr;
      b[5][5] = nullptr;
      b[5][6] = nullptr;
      b[5][7] = nullptr;

      b[6][0] = new BPiece(Piece::P, Color::W);
      b[6][1] = new BPiece(Piece::P, Color::W);
      b[6][2] = new BPiece(Piece::P, Color::W);
      b[6][3] = new BPiece(Piece::P, Color::W);
      b[6][4] = new BPiece(Piece::P, Color::W);
      b[6][5] = new BPiece(Piece::P, Color::W);
      b[6][6] = new BPiece(Piece::P, Color::W);
      b[6][7] = new BPiece(Piece::P, Color::W);

      b[7][0] = new BPiece(Piece::R, Color::W);
      b[7][1] = new BPiece(Piece::H, Color::W);
      b[7][2] = new BPiece(Piece::B, Color::W);
      b[7][3] = new BPiece(Piece::Q, Color::W);
      b[7][4] = new BPiece(Piece::K, Color::W);
      b[7][5] = new BPiece(Piece::B, Color::W);
      b[7][6] = new BPiece(Piece::H, Color::W);
      b[7][7] = new BPiece(Piece::R, Color::W);

      init();
    }

    ~Board() {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
	    if (b[i][j] != nullptr) {
              delete b[i][j];
	    }
	}
      }
    }

    MoveResult canMovePiece(Piece& p, Position& from, Position& to) {
      return calculators.at(p)->canMove(this, from, to);
    }
   
    void setBlackKingMoved() {
       blackKingMoved = true;
    }

    bool getBlackKingMoved() {
      return blackKingMoved;
    }

    void setBlackLeftRookMoved() {
       blackLeftRookMoved = true;
    }

    bool getBlackLeftRookMoved() {
      return blackLeftRookMoved;
    }

    void setBlackRightRookMoved() {
       blackRightRookMoved = true;
    }

    bool getBlackRightRookMoved() {
      return blackRightRookMoved;
    }


    void setWhiteKingMoved() {
       whiteKingMoved = true;
    }

    bool getWhiteKingMoved() {
      return whiteKingMoved;
    }

    void setWhiteLeftRookMoved() {
       whiteLeftRookMoved = true;
    }

    bool getWhiteLeftRookMoved() {
      return whiteLeftRookMoved;
    }

    void setWhiteRightRookMoved() {
       whiteRightRookMoved = true;
    }

    bool getWhiteRightRookMoved() {
      return whiteRightRookMoved;
    }

    void setEnpassant(Position p) {
      enPassant = p;
    }

    Position getEnpassant() {
      return enPassant;
    }

    BPiece* get(Position& p) {
      return b[p.getRow()][p.getColumn()];
    }

    bool canExecuteSmallCastle() {
      bool moved = getCurrent() == Color::W ? getWhiteKingMoved() || getWhiteRightRookMoved() : getBlackKingMoved() || getBlackRightRookMoved();
      if (moved) {
	 return false;
      }
      int r = 0;
      if (getCurrent() == Color::W) {
        r = 7;
      }
      for (int i = 0; i < 8; i++) {
	for (int j = 0; j < 8; j++) {
	  
	  Position from = Position(i, j);
	  BPiece * bp = b[i][j];
	  if (bp != nullptr && bp->getColor() != getCurrent()) {
	    current = getCurrent() == Color::W ? Color::B : Color::W;
	    for (int c = 4; c <= 6; c++) { 
		  Position to = Position(r, c);
		  if ((i != r || j != c) && bp->canMove(this, from, to).canMove) {
			  return false;
		  }          
	    }
	    current = getCurrent() == Color::W ? Color::B : Color::W;
	  }
	}
      }
      return true;
    }

    bool canExecuteBigCastle() {
      bool moved = getCurrent() == Color::W ? getWhiteKingMoved() || getWhiteLeftRookMoved() : getBlackKingMoved() || getBlackLeftRookMoved();
      if (moved) {
	 return false;
      }
      int r = 0;
      if (getCurrent() == Color::W) {
        r = 7;
      }
      for (int i = 0; i < 8; i++) {
	for (int j = 0; j < 8; j++) {
	  
	  Position from = Position(i, j);
	  BPiece * bp = b[i][j];
	  if (bp != nullptr && bp->getColor() != getCurrent()) {
	    current = getCurrent() == Color::W ? Color::B : Color::W;
	    for (int c = 2; c <= 4; c++) { 
		  Position to = Position(r, c);
		  if ((i != r || j != c) && bp->canMove(this, from, to).canMove) {
                    return false;
		  }          
	    }
	    current = getCurrent() == Color::W ? Color::B : Color::W;
	  }
	}
      }
      return true;
    }
    
    bool canMove(Position fP, Position tP) {
      if (fP.getRow() == tP.getRow() && fP.getColumn() == tP.getColumn()) {
        return false;
      }
      
	BPiece* bcopy[8][8];
	for (int i = 0; i < 8; i++) {
	  for (int j = 0; j < 8; j++) {
	    bcopy[i][j] = b[i][j];
	  }
	}

	BPiece * bp = b[fP.getRow()][fP.getColumn()];
	MoveResult mr = bp->canMove(this, fP, tP);
	bool result = mr.canMove;
	if (result) {
	  if (mr.capturedEnpassant) {
	    int add = current == Color::W ? 1 : -1;
	    b[tP.getRow() + add][tP.getColumn()] = nullptr;
	  } 
	  if (mr.smallCastle) {
            if (current == Color::W) {
              b[7][6] = b[7][4];
  	      b[7][4] = nullptr;
	      b[7][5] = b[7][7];
              b[7][7] = nullptr;      
	    } else {	     
              b[0][6] = b[0][4];
  	      b[0][4] = nullptr;
	      b[0][5] = b[0][7];
              b[0][7] = nullptr;      
	    }	    
	  } else if (mr.bigCastle) {
            if (current == Color::W) {
              b[7][2] = b[7][4];
  	      b[7][4] = nullptr;
	      b[7][3] = b[7][0];
              b[7][0] = nullptr;      
	    } else {	     
              b[0][2] = b[0][4];
  	      b[0][4] = nullptr;
	      b[0][3] = b[0][0];
              b[0][0] = nullptr;      
	    }	    
	  } else {	  
	    b[tP.getRow()][tP.getColumn()] = b[fP.getRow()][fP.getColumn()];
            b[fP.getRow()][fP.getColumn()] = nullptr;
	  }
	  result = !isCurrentInCheck();
	}
        
	    for (int i = 0; i < 8; i++) {
	      for (int j = 0; j < 8; j++) {
	        b[i][j] = bcopy[i][j];
	      }
	    }
	return result;
    }

    Result move(string from, string to) {
	if (checkmate) {
	  throw logic_error("Checkmate! The game is over! No more move allowed!");
	}
        if (promotion.getRow() != -1) {
	   throw logic_error("Promotion needs to be executed first!");
	}	
	Position fP = validatePosition(from);
	Position tP = validatePosition(to);
	if (from.compare(to) == 0) {
	  throw logic_error("From and To must be different!");
	}
	if (b[fP.getRow()][fP.getColumn()] == nullptr) {
	  throw logic_error("From position is empty!");
	}
	BPiece* bp = b[fP.getRow()][fP.getColumn()];
	if (bp->getColor() != current) {
	  throw logic_error("From position is not the current player!");
	}
	// copying is required because the current player is not allowed
	// to but itself in check. So we make the move, verify if it is
	// in check, if true we restore the board.
    
	BPiece* bcopy[8][8];
	for (int i = 0; i < 8; i++) {
	  for (int j = 0; j < 8; j++) {
	    bcopy[i][j] = b[i][j];
	  }
	}

	MoveResult mr = bp->canMove(this, fP, tP);
	if (mr.canMove) {
	  enPassant = Position(-1, -1);
	  if (mr.enpassant) {
	    enPassant = Position(tP.getRow(), tP.getColumn());
	  } else if (mr.capturedEnpassant) {
	    int add = current == Color::W ? 1 : -1;
	    b[tP.getRow() + add][tP.getColumn()] = nullptr;
	  } 
	  if (mr.smallCastle) {
            if (current == Color::W) {
              b[7][6] = b[7][4];
  	      b[7][4] = nullptr;
	      b[7][5] = b[7][7];
              b[7][7] = nullptr;     
	    } else {	     
              b[0][6] = b[0][4];
  	      b[0][4] = nullptr;
	      b[0][5] = b[0][7];
              b[0][7] = nullptr;     
	    }	

	  } else if (mr.bigCastle) {
            if (current == Color::W) {
              b[7][2] = b[7][4];
  	      b[7][4] = nullptr;
	      b[7][3] = b[7][0];
              b[7][0] = nullptr;      
	    } else {	     
              b[0][2] = b[0][4];
  	      b[0][4] = nullptr;
	      b[0][3] = b[0][0];
              b[0][0] = nullptr;      
	    }	    
	  } else {	  
	    b[tP.getRow()][tP.getColumn()] = b[fP.getRow()][fP.getColumn()];
            b[fP.getRow()][fP.getColumn()] = nullptr;
	  }
	  if (mr.promotion) {
	    promotion = Position(tP.getRow(), tP.getColumn());
	  }
	  // Verify if current put itself in check, in case true, restore the board from the copy.
	  if (isCurrentInCheck()) {
	    for (int i = 0; i < 8; i++) {
	      for (int j = 0; j < 8; j++) {
	        b[i][j] = bcopy[i][j];
	      }
	    }
	    promotion = Position(-1, -1);
	    enPassant = Position(-1, -1);

	    return Result(false, false, false, false);

	  } else {
	    
	    if (getCurrent() == Color::W && !getWhiteKingMoved() && fP.getRow() == 7 && fP.getColumn() == 4) {
              setWhiteKingMoved();
	    }

	    if (getCurrent() == Color::W && !getWhiteLeftRookMoved() && fP.getRow() == 7 && fP.getColumn() == 0) {
              setWhiteLeftRookMoved();
	    }	    
		  
	    if (getCurrent() == Color::W && !getWhiteRightRookMoved() && fP.getRow() == 7 && fP.getColumn() == 7) {
              setWhiteRightRookMoved();
	    }

	    if (getCurrent() == Color::B && !getBlackKingMoved() && fP.getRow() == 0 && fP.getColumn() == 4) {
              setBlackKingMoved();
	    }	    
	    
	    if (getCurrent() == Color::B && !getBlackLeftRookMoved() && fP.getRow() == 0 && fP.getColumn() == 0) {
              setBlackLeftRookMoved();
	    }	    
	    
	    if (getCurrent() == Color::B && !getBlackRightRookMoved() && fP.getRow() == 0 && fP.getColumn() == 7) {
              setBlackRightRookMoved();
	    }	    
	    // current is not in check, so we can allow the move and change the current.
            current = current == Color::W ? Color::B : Color::W;
	  }
          	  
	}
	bool check = isCurrentInCheck();
	checkmate = isCurrentInCheckmate(check);
	return Result(mr.promotion, check, checkmate, true);
    }

    bool isCurrentInCheck() {
      int row = -1;
      int col = -1;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
	  BPiece * bp = b[i][j];
	  if (bp != nullptr && bp->getColor() == getCurrent() && bp->getPiece() == Piece::K) {
	    row = i;
            col = j;
		  break;
	  }
	}
      }
      Position p = Position(row, col);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
	   Position r = Position(i, j);
	   BPiece * bp = get(r);
	   if (bp != nullptr && bp->getColor() != getCurrent()) {
		   current = getCurrent() == Color::W ? Color::B : Color::W;
		   bool canMove = bp->canMove(this, r, p).canMove;
		   current = getCurrent() == Color::W ? Color::B : Color::W; 
		   if (canMove) {
    	     return true;	   
		   }
	   } 
	}
      }

      return false;
    }

    /*
     * Current is in checkmate if it cannot move any of its piece.
     */
    bool isCurrentInCheckmate(bool check) {
      if (!check) {
        return false;
      }
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
	  Position p = Position(i, j);
	  BPiece *bp = get(p);
	  if (bp != nullptr && bp->getColor() == getCurrent()) {
            for (int r = 0; r < 8; r++) {
	      for (int c = 0; c < 8; c++) {
	        Position to = Position(r, c);
		BPiece * op = get(to);
		if (op == nullptr || op->getColor() != getCurrent()) {
		  bool result = canMove(p, to);
		  if (result) {
		    return false;
		  }
		}
	      }
	    }	    
	  }
	}
      }
      return true;
    }

    /*
     * When promote is called the current player has already been changed.
     * So the current player is the one that needs to be verified against 
     * check/checkmate.
     */
    Result promote(Piece p) {
      if (p != Piece::Q && p != Piece::R && p != Piece::H && p != Piece::B) {
         throw logic_error("Promotion needs to be to Queuen, Rook, Knight or Bishop!");
      }
      if (promotion.getRow() == -1) {
        throw logic_error("There is no pawn to be promoted!");
      }
      BPiece * bp = get(promotion);
      bp->setPiece(p);
      promotion = Position(-1, -1);
      bool check = isCurrentInCheck();
      return Result(false, check, isCurrentInCheckmate(check), true);
    }

    void print() {
      for (int i = 0; i < 8; i++) {
	cout << 8 - i << " ";
	for (int j = 0; j < 8; j++) {
	  BPiece* p = b[i][j];
	  if (p == nullptr) {
	    cout << "." << " ";
	    continue;
	  }
	  if (p->getColor() == Color::W) {
	    if (p->getPiece() == Piece::K) {
	      cout << "\u2654" << " ";
	    }
	    if (p->getPiece() == Piece::Q) {
	      cout << "\u2655" << " ";
	    }
	    if (p->getPiece() == Piece::R) {
	      cout << "\u2656" << " ";
	    }
	    if (p->getPiece() == Piece::B) {
	      cout << "\u2657" << " ";
	    }
	    if (p->getPiece() == Piece::H) {
              cout << "\u2658" << " ";
	    } 
	    if (p->getPiece() == Piece::P) {
              cout << "\u2659" << " ";
	    }
	  } else {
            if (p->getPiece() == Piece::K) {
	      cout << "\u265A" << " ";
	    }
	    if (p->getPiece() == Piece::Q) {
	      cout << "\u265B" << " ";
	    }
	    if (p->getPiece() == Piece::R) {
	      cout << "\u265C" << " ";
	    }
	    if (p->getPiece() == Piece::B) {
	      cout << "\u265D" << " ";
	    }
	    if (p->getPiece() == Piece::H) {
              cout << "\u265E" << " ";
	    } 
	    if (p->getPiece() == Piece::P) {
              cout << "\u265F" << " ";
	    } 
	  }
	}
	cout << '\n';
      }
      cout << "  ";
      for (char c = 'a'; c <= 'h'; c++) {
	      cout << c << " ";
      }
      cout << '\n';
    }

    Color getCurrent() {
      return current;
    }
};


class PawnMoveCalculator : public MoveCalculator {
  public:
    /*
     * 1. Pawn can move only forward in the same column, except when
     * capturing a piece, it can move one square in diagonal to left
     * or to right. In addition, en passant, allows a pawn to capture
     * a pawn that is beside it, when the pawn being captured moved for
     * the first time two squares at once.
     * 2. Pawn can move only one square at a time, except the first move
     * allows a Pawn to move two squares.
     */
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      // can move one position?
      int add = board->getCurrent() == Color::W ? -1 : 1;
      Position one = Position(from.getRow() + add, from.getColumn());
      BPiece* bp = board->get(one);
      if (bp == nullptr && to.getRow() == one.getRow() && to.getColumn() == one.getColumn()) {
        return MoveResult(true, to.getRow() == 0 || to.getRow() == 7, false, false, false, false);
      }
      // can move two positions?
      Position two = Position(from.getRow() + 2 * add, from.getColumn());
      BPiece * bpTwo = board->get(two);
      int initialRow = board->getCurrent() == Color::W ? 6 : 1;
      if (bp == nullptr && bpTwo == nullptr && from.getRow() == initialRow && to.getRow() == two.getRow() && to.getColumn() == two.getColumn()) {
        return MoveResult(true, false, true, false, false, false);
      }
      // can capture?
      if (from.getColumn() - 1 >= 0) {
        Position cap = Position(from.getRow() + add, from.getColumn() - 1);
	BPiece * c = board->get(cap);
	if (c != nullptr && c->getColor() != board->getCurrent() && to.getRow() == cap.getRow() && to.getColumn() == cap.getColumn()) {
	  return MoveResult(true, to.getRow() == 0 || to.getRow() == 7, false, false, false, false);
	}
	// enpassant
	if (c == nullptr && to.getRow() == cap.getRow() && to.getColumn() == cap.getColumn()) {
	  Position enPassant = board->getEnpassant();
	  return MoveResult(enPassant.getRow() != -1 && enPassant.getRow() == from.getRow() && enPassant.getColumn() == from.getColumn() - 1, false, false, true, false, false);
	}
      }
      if (from.getColumn() + 1 < 8) {
        Position cap = Position(from.getRow() + add, from.getColumn() + 1);
	BPiece * c = board->get(cap);
	if (c != nullptr && c->getColor() != board->getCurrent() && to.getRow() == cap.getRow() && to.getColumn() == cap.getColumn()) {
          return MoveResult(true, to.getRow() == 0 || to.getRow() == 7, false, false, false, false);
	}
	// enpassant
	if (c == nullptr && to.getRow() == cap.getRow() && to.getColumn() == cap.getColumn()) {
	  Position enPassant = board->getEnpassant();
	  return MoveResult(enPassant.getRow() != -1 && enPassant.getRow() == from.getRow() && enPassant.getColumn() == from.getColumn() + 1, false, false, true, false, false);
	} 
      }
      
      return MoveResult(false, false, false, false, false, false);
    }
};

class BishopMoveCalculator : public MoveCalculator {
  public:
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
          for (int r = from.getRow() + i, c = from.getColumn() + j; r >= 0 && r < 8 && c >= 0 && c < 8; r += i, c += j) {
            Position p = Position(r, c);
	    BPiece * bp = board->get(p);
            if (r == to.getRow() && c == to.getColumn() && (bp == nullptr || bp->getColor() != board->getCurrent())) {
	      return MoveResult(true, false, false, false, false, false);	
	    }
	    if (r == to.getRow() && c == to.getColumn()) {
	      return MoveResult(false, false, false, false, false, false);
	    }
	    if (bp != nullptr) {
	      break;
	    }
          }
	}
      }

      return MoveResult(false, false, false, false, false, false);
    }
};

class HorseMoveCalculator : public MoveCalculator {
  public:
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      int moves[8][2] = {{1, 2}, {2, 1}, {-1, 2}, {2, -1}, {-2, 1}, {1, -2}, {-2, -1}, {-1, -2}};
      int len = end(moves) - begin(moves); 
      for (int m = 0; m < len; m++) {
	 int r = moves[m][0] + from.getRow();
	 int c = moves[m][1] + from.getColumn();
	 if (r >= 0 && r < 8 && c >= 0 && c < 8 && r == to.getRow() && c == to.getColumn()) {
	   BPiece * bp = board->get(to);
	   if (bp == nullptr || bp->getColor() != board->getCurrent()) {
	     return MoveResult(true, false, false, false, false, false);
	   }
	   return MoveResult(false, false, false, false, false, false);
	 }

      }
       
      return MoveResult(false, false, false, false, false, false);
    }
};

class KingMoveCalculator : public MoveCalculator {
  public:
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      int rowDiff = abs(from.getRow() - to.getRow());
      int colDiff = abs(from.getColumn() - to.getColumn());
      BPiece * bp = board->get(to);
      if (rowDiff <= 1 && colDiff <= 1 && (bp == nullptr || bp->getColor() != board->getCurrent())) {
        return MoveResult(true, false, false, false, false, false);
      } else if (rowDiff <= 1 && colDiff <= 1) {
	return MoveResult(false, false, false, false, false, false);
      }

      if (board->getCurrent() == Color::W && to.getRow() == 7 && to.getColumn() == 6 && board->canExecuteSmallCastle()) {
	      return MoveResult(true, false, false, false, true, false);
      }

      if (board->getCurrent() == Color::W && to.getRow() == 7 && to.getColumn() == 2 && board->canExecuteBigCastle()) {
        return MoveResult(true, false, false, false, false, true);
      }

      if (board->getCurrent() == Color::B && to.getRow() == 0 && to.getColumn() == 6 && board->canExecuteSmallCastle()) {
        return MoveResult(true, false, false, false, true, false);
      }

      if (board->getCurrent() == Color::B && to.getRow() == 0 && to.getColumn() == 2 && board->canExecuteBigCastle()) {
        return MoveResult(true, false, false, false, false, true);
      }
      return MoveResult(false, false, false, false, false, false);
    }
};


class RookMoveCalculator : public MoveCalculator {
  public:
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      if (to.getRow() != from.getRow() && to.getColumn() != from.getColumn()) {
        return MoveResult(false, false, false, false, false, false);
      }
      int addRow = 0;
      if (to.getRow() > from.getRow()) {
        addRow = 1;
      } else if (to.getRow() < from.getRow()) {
	addRow = -1;
      }
      int addCol = 0;
      if (to.getColumn() > from.getColumn()) {
        addCol = 1;
      } else if (to.getColumn() < from.getColumn()) {
        addCol = -1;
      }
      for (int r = from.getRow() + addRow, c = from.getColumn() + addCol; r >= 0 && r < 8 && c >= 0 && c < 8; r += addRow, c += addCol) {
        Position p = Position(r, c);
	BPiece * bp = board->get(p);
	if (r == to.getRow() && c == to.getColumn()) {
	   bool canMove = bp == nullptr || bp->getColor() != board->getCurrent();
	   return MoveResult(canMove, false, false, false, false, false);  
	}
	if (bp != nullptr) {
	  return MoveResult(false, false, false, false, false, false);
	}
      }
      return MoveResult(false, false, false, false, false, false);
    }
};

class QueenMoveCalculator : public MoveCalculator {
  public:
    MoveResult canMove(Board* board, Position& from, Position& to) override {
      BishopMoveCalculator b;
      MoveResult mr = b.canMove(board, from, to);
      if (mr.canMove) {
        return mr;
      }
      RookMoveCalculator r;
      return r.canMove(board, from, to);
    }
};

void Board::init() {
      calculators[Piece::P] = new PawnMoveCalculator;
      calculators[Piece::B] = new BishopMoveCalculator;
      calculators[Piece::H] = new HorseMoveCalculator;
      calculators[Piece::R] = new RookMoveCalculator;
      calculators[Piece::Q] = new QueenMoveCalculator;
      calculators[Piece::K] = new KingMoveCalculator;
}

MoveResult BPiece::canMove(Board* b, Position& from, Position& to) {
  return b->canMovePiece(this->piece, from, to);
}

int main() {

    Board b;
    b.print();
    string from;
    string to;
    string promotion;
    Piece piece;
    Result r = Result(false, false, false, false);
    while (true) {
      cout << "From?" << '\n';
      cin >> from;
      cout << "To?" << '\n';
      cin >> to;
      try {
        r = b.move(from, to);
	if (!r.canMove) {
	  cout << "Current cannot move to specified position! Try other position!" << '\n';
	}
	else if (r.promotion) {
	  cout << "Promote pawn to (Q, R, H, B)?" << '\n';
	  cin >> promotion;
          if (promotion == "Q") {
	    piece = Piece::Q;
	  } else if (promotion == "R") {
            piece = Piece::R;
	  } else if (promotion == "H") {
	    piece = Piece::H;
	  } else if (promotion == "B") {
	    piece = Piece::B;
	  } else {
	    piece = Piece::Q;
	  }
	  r = b.promote(piece);
	} 
	if (r.check) {
	  cout << "Current is in check!" << '\n';
	  cout << "Is checkmate: " << r.checkmate << '\n';
	}
      } catch (exception& e) {
	cout << e.what() << '\n';
      }      
      b.print();
      if (r.checkmate) {
        break;
      }
    }
    return 0;
}

