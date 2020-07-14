#ifndef CHESS_HPP
#define CHESS_HPP
/* Created by: Angelic Phan
 * CS 410P, Winter 2019: Advanced C++
 * Steven Libby
 * Final Project
 * Due: March 18, 2019
 *
 * Description: This is the header file for the structs piece and test piece, as well as classes
 * game and chess. It also has some small implementations of some of the shorter functions. This
 * file also defines some global variables and includes some necessary libraries.
 */
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;

//***************************************************************************************************************
//          GLOBAL VARIABLES
//***************************************************************************************************************

const int ROWS = 8;               //Number of rows for the board
const int COLS = 8;               //Number of cols for the board
const int PIECES = 32;            //Number of chess pieces: 16 for white, 16 for black
const string BLACK = "BLACK";     //Indicates BLACK's color/turn...etc
const string WHITE = "WHITE";     //Indicates WHITE's color/turn...etc
const int SPACE = 0;              //Indicates a blank space
const int MY_COLOR = 1;           //Indicates if it is my color
const int NOT_MY_COLOR = 2;       //Indicates if it's not my color
const int GOOD = 0;               //Indicates if it's a good move and won't be in check/checkmate
const int CHECKED = 1;            //Indicates our king is in check
const int CHECKMATE = 2;          //Indicates our king is in checkmate
const char CASTLING = 'c';        //Indicates this is a castling move
const char ENPASSANT = 'e';       //Indicates this is a castling move
const char REGULAR = 'r';         //Indicates this is a regular move
const char BLANK_NAME = ' ';      //Indicates a piece with a blank name
const int DONT_CHECK_KING = 0;    //Indicates we don't want to check if the move will put king in danger
const int CHECK_KING = 1;         //Indicates we do want to check if the move will put king in dangeer
const int TRY_MOVE = 1;           //Indicates we are trying out a move
const int NOT_TRYING_MOVE = 0;    //Indicates we are trying out a move
using point = pair<int, int>;

//***************************************************************************************************************
//          TEST PIECE STRUCT
//***************************************************************************************************************

//This structure aids in the processing of whether or not the king is/will be in check
//if we do a certain move. It keeps track of the name and location of a piece, so that
//we can keep track of where pieces are on the board.
struct test_piece
{
  char name; //Name of the piece
  point loc; //Location of the piece
};

//***************************************************************************************************************
//          PIECE STRUCT
//***************************************************************************************************************

//This structure has the characteristics of a game piece
//A game piece has a name to indicate what type of piece it is
//Lowercase = white, uppercase = black
//It also indicates whether or not a piece has already moved, useful for
//when we have special moves like castling...etc. It also has an indication
//for if this piece can do the en passant move. A game piece also has a path
//of possible locations it can move to
struct piece
{
  piece(void);                          //Constructor
  piece& operator=(const piece & p);    //Function to copy game pieces
  //Function to compare game pieces to see if the names match
  bool operator !=(const piece & p) { return name != p.name;}

  char name;                            //A game piece's name
  bool has_moved;                       //Indicates if the game piece has ever moved before
  bool can_ep;                          //Indicates if the game piece can en passant
  vector<point> path;                   //Paths of possible locations it can move to
};

const piece BLANK;      //Is a blank piece to represent a piece with ' ' name

//***************************************************************************************************************
//          CHESS CLASS
//***************************************************************************************************************

//This class is the parent class of game. It deals with the setup of the game board
//and of the game pieces. It manages the location of pieces and where they can go, as
//well as other information such as its name.
class chess
{
  private:
    piece _pieces[PIECES];            //Chess pieces
    vector<vector<piece> > _board;     //Board

  public:
    //Constructor
    chess(void);

    //Destructor
    ~chess(void) {}

    //Function to display chess board
    void display_board(void);

    //Returns the name of the game piece at location "at"
    char get_name(point at) { return _board[at.first][at.second].name;}

    //Returns a copy of the entire game piece at location "at"
    piece get_piece(point at) { return _board[at.first][at.second];}

    //Returns the location of the king for the "color" team and keeps track of each teams
    //player's locations in the vectors
    point get_king(string color, vector<test_piece> & white_team, vector<test_piece> & black_team);

    //Function to update the location of the piece at location "old_loc"
    void set_piece(piece player, point loc) { _board[loc.first][loc.second] = player;}

    //Func to update the name of the piece at location "at"
    void set_name(point at, char name) { _board[at.first][at.second].name = name;}

    //Function to add a possible location to the path of chess piece at point "from"
    void build_path(point from, point add) { _board[from.first][from.second].path.push_back(add);}

    //Function to clear all possible locations so that we can get new possibilities for its new location
    void empty_path(point at)
    {
      while(!_board[at.first][at.second].path.empty()) //While Path isn't empty
        _board[at.first][at.second].path.pop_back(); //Continue to empty it out
    }

    //Clear all paths of every piece
    void empty_all_paths(void);

    //Function to copy a path into a certain piece at location "loc"
    void copy_path(vector<point> to_copy, point loc)
    {
      empty_path(loc); //Clear old path
      _board[loc.first][loc.second].path = to_copy; //Set to new path
    }

    //Will change the status of the game piece when it has finally moved so it can't
    //do any more special moves that require it to not have moved before
    void mark_as_moved(point at) { _board[at.first][at.second].has_moved = true;}

    //Will change the status of the game piece so that it can en passant in the
    //next turn
    void mark_as_can_ep(point at) { _board[at.first][at.second].can_ep = true;}

    //Reset all pieces as not being able to en passant
    void mark_as_cant_ep(void);

    //Returns status of game piece to check if it has moved yet or not
    bool has_moved(point at)
    {
      if(_board[at.first][at.second].has_moved) //If it has moved
        return true; //Return true
      return false; //Else return false
    }

    //Print out all the possible locations of a game piece
    int print_path(point at);

    //Returns the location to go for the game piece at index "to" in its path
    point get_location(point from, int to) { return _board[from.first][from.second].path.at(to);}

    //Returns number of possible locations in path
    int num_in_path(point at) { return _board[at.first][at.second].path.size();}

    //Returns the whole path of game piece at location "at"
    vector<point> get_path(point at) { return _board[at.first][at.second].path;}
};

//***************************************************************************************************************
//          GAME CLASS
//***************************************************************************************************************

//This class is a child class of the chess class. This class interacts with the players
//to ask where they would like to move their game pieces, and to tell them if their king
//is in danger (check/checkmate). It also determines all the different moves of the different
//types of game pieces, including the normal moves and the special moves. It also keeps track
//of all the captured game pieces of each team. Furthermore, this class will move the game
//pieces according to the player's choice, as long as it is a valid move.
class game : public chess
{
  private:
    vector<char> white_captured; //Keeps track of the captured white game pieces
    vector<char> black_captured; //Keeps track of the captured black game pieces

  public:
    //Constructor
    game(void) {}

    //Destructor
    ~game(void) {}

    //Prompts the user for what piece they would like to move and where, or let them exit
    int prompt_to_move(string color);

    //Based on the game piece the user chosed, it will load up the possible moves
    //of that game piece
    void load_path(point from, string color, int flag);

    //Function to move the game piece, and to do the special moves if it was chosen
    void move(point from, point to, string color);

    //This function removes a potential location from the list of potential locations
    void remove_from_path(vector<point> & path, point match);

    //Function to remove game piece from board and put it into the correct captured list
    void capture(point to, string color);

    //Builds list of possible moves for a white pawn
    void move_white_pawn(point from, string color, int flag);

    //Builds list of possible moves for a black pawn
    void move_black_pawn(point from, string color, int flag);

    //Determines if the user wants to move a white or black pawn
    void move_pawn(point from, string color, int flag);

    //Builds list of possible moves for a rookie
    void move_rookie(point from, string color, int flag);

    //Builds list of possible moves for a knight
    void move_knight(point from, string color, int flag);

    //Builds list of possible moves for a bishop
    void move_bishop(point from, string color, int flag);

    //Builds list of possible moves for a king
    void move_king(point from, string color, int flag);

    //Builds list of possible moves for a queen
    void move_queen(point from, string color, int flag);

    //Checks if the space is occupied, and if so, by which team's piece
    int occupied(point location, string color);

    //Check if the game piece is my team's piece
    bool is_my_color(point location, string color);

    //Promote a pawn when it gets to the other side
    void promote(point at);

    //Checks if the king can do the castling move
    void can_castling(point king, string color, int flag);

    //Initiates the castling move
    void castle(point king_from, point king_to, string color);

    //Tells the user if their king is in danger
    bool prompt_if_checked(string color, string & winner);

    //Tries to see if we can get out of check/checkmate by eliminating the attacker
    bool eliminate_attacker(point attacker, string color);

    //Tries to see if we can move the king to avoid being in check
    bool can_move_king(point king, string color, vector<test_piece> white_team, vector<test_piece> black_team);

    //Tries to see if we can move in such a way to get out of being in check
    bool eliminate_or_block_attacker(string color, point king, vector<test_piece> white_team, vector<test_piece> black_team);
    
    //Aids in checking to see if the king can get out of being in check by getting
    //the path of the king without checking for castling or removing the possible
    //move if it gets us in check just yet. We hold off on that for now
    void get_path_of_king(point from, string color);

    //Check to see if the king is currently in danger
    int checked(string color, int flag);

    //Check to see if the possible move will put the king in danger
    int will_be_checked(point from, point at, string color, char move);

    //Check to see if the enemy can capture in passing
    void enemy_can_en_passant(point from, point to, string color);

    //Initiates the en passant move
    void capture_en_passant(point from, point to, string color);

    /*//Ask if the enemy team would like to capture in passing
    bool ask_to_en_passant(point me, string color);

    //Initiate the en passant move
    bool en_passant(point me, point enemy, string enemy_color);*/

    //Print out the list of captured game pieces
    void print_captured(void);

};

#endif
