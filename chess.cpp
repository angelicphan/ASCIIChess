#ifndef CHESS_CPP
#define CHESS_CPP
/* Created by: Angelic Phan
 * CS 410P, Winter 2019: Advanced C++
 * Steven Libby
 * Final Project
 * Due: March 18, 2019
 *
 * Description: This is the implementation file for the structs piece and test_piece, as well as for
 * the classes game and chess. It also contains the two helper functions for converting from char to
 * int and int to char for the correlation between user input of piece location and indices in the
 * game board. This file contains implementations for displaying the board, setting up the game pieces,
 * prompting user for input, displaying messages to the user for things such as captured pieces, and
 * whether or not their king is in check. This file also contains implementation for what moves which
 * game pieces can make for the different types of game pieces, and checks for whether or not the king
 * will be/is in check/checkmate, and if he can get out of being in check. Special moves such as castling
 * are also included.
 */

#include "chess.hpp"

//***************************************************************************************************************
//          RANDOM HELPER FUNCTIONS
//***************************************************************************************************************

//This function converts the columns, which are represented as
//letters into numbers
int convert_cols_to_ints(char c)
{
  if(c == 'A' || c == 'a')
    return 0;
  if(c == 'B' || c == 'b')
    return 1;
  if(c == 'C' || c == 'c')
    return 2;
  if(c == 'D' || c == 'd')
    return 3;
  if(c == 'E' || c == 'e')
    return 4;
  if(c == 'F' || c == 'f')
    return 5;
  if(c == 'G' || c == 'g')
    return 6;
  if(c == 'H' || c == 'h')
    return 7;
}

//This function converts the columns that have already been converted
//into numbers, back into its letter form
char convert_ints_to_cols(int i)
{
  if(i == 0)
    return 'A';
  if(i == 1)
    return 'B';
  if(i == 2)
    return 'C';
  if(i == 3)
    return 'D';
  if(i == 4)
    return 'E';
  if(i == 5)
    return 'F';
  if(i == 6)
    return 'G';
  if(i == 7)
    return 'H';
}

//***************************************************************************************************************
//          IMPLEMENTATION FOR STRUCT PIECE
//***************************************************************************************************************

//This structure has the characteristics of a game piece
//A game piece has a name to indicate what type of piece it is
//Lowercase = white, uppercase = black
//It also indicates whether or not a piece has already moved, useful for
//when we have special moves like castling...etc. A game piece also has a path
//of possible locations it can move to

//Constructor
piece::piece(void)
{
  name = ' ';
  has_moved = false;
  can_ep = false;
}

//Copies a game piece's info into a different game piece
piece& piece::operator=(const piece & p)
{
  if(this == &p) //Is it pointing to myself?
    return *this;
  //No so copy everythig
  name = p.name;
  has_moved = p.has_moved;
  can_ep = p.can_ep;
  path = p.path;
  return *this;
}

//***************************************************************************************************************
//          IMPLEMENTATION FOR CHESS CLASS
//***************************************************************************************************************

//This class is the parent class of game. It deals with the setup of the game board
//and of the game pieces. It manages the location of pieces and where they can go, as
//well as other information such as its name.

//Chess class Constructor
chess::chess(void): _board(ROWS, vector<piece>(COLS, BLANK))
{
  //Set up chess pieces
  //White pieces
  _pieces[0].name = 'r';
  _pieces[1].name = 'n';
  _pieces[2].name = 'b';
  _pieces[3].name = 'q';
  _pieces[4].name = 'k';
  _pieces[5].name = 'b';
  _pieces[6].name = 'n';
  _pieces[7].name = 'r';
  for(int w = 8; w < 16; ++w)
    _pieces[w].name = 'p';
  //Black pieces
  _pieces[16].name = 'R';
  _pieces[17].name = 'N';
  _pieces[18].name = 'B';
  _pieces[19].name = 'Q';
  _pieces[20].name = 'K';
  _pieces[21].name = 'B';
  _pieces[22].name = 'N';
  _pieces[23].name = 'R';
  for(int b = 24; b < 32; ++b)
    _pieces[b].name = 'P';
  //Set up board: value = index for piece
  for(int i = 0; i < 8; ++i)
  {
    _board[0][i] = _pieces[i];        //White royal
    _board[1][i] = _pieces[i + 8];    //White pawns
    _board[2][i] = BLANK;             //Blank spaces
    _board[3][i] = BLANK;
    _board[4][i] = BLANK;
    _board[5][i] = BLANK;
    _board[6][i] = _pieces[i + 24];   //Black pawns
    _board[7][i] = _pieces[i + 16];   //Black royals
  }
}

//Displays the game board and the setup of the game pieces
void chess::display_board(void)
{
  cout << endl;
  cout << "        a     b     c     d     e     f     g     h       " << endl;
  cout << "     _________________________________________________    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  8  |  " << _board[7][0].name << "  |  " << _board[7][1].name;
  cout << "  |  " << _board[7][2].name << "  |  " << _board[7][3].name;
  cout << "  |  " << _board[7][4].name << "  |  " << _board[7][5].name;
  cout << "  |  " << _board[7][6].name << "  |  " << _board[7][7].name << "  |  8 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  7  |  " << _board[6][0].name << "  |  " << _board[6][1].name;
  cout << "  |  " << _board[6][2].name << "  |  " << _board[6][3].name;
  cout << "  |  " << _board[6][4].name << "  |  " << _board[6][5].name;
  cout << "  |  " << _board[6][6].name << "  |  " << _board[6][7].name << "  |  7 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  6  |  " << _board[5][0].name << "  |  " << _board[5][1].name;
  cout << "  |  " << _board[5][2].name << "  |  " << _board[5][3].name;
  cout << "  |  " << _board[5][4].name << "  |  " << _board[5][5].name;
  cout << "  |  " << _board[5][6].name << "  |  " << _board[5][7].name << "  |  6 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  5  |  " << _board[4][0].name << "  |  " << _board[4][1].name;
  cout << "  |  " << _board[4][2].name << "  |  " << _board[4][3].name;
  cout << "  |  " << _board[4][4].name << "  |  " << _board[4][5].name;
  cout << "  |  " << _board[4][6].name << "  |  " << _board[4][7].name << "  |  5 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  4  |  " << _board[3][0].name << "  |  " << _board[3][1].name;
  cout << "  |  " << _board[3][2].name << "  |  " << _board[3][3].name;
  cout << "  |  " << _board[3][4].name << "  |  " << _board[3][5].name;
  cout << "  |  " << _board[3][6].name << "  |  " << _board[3][7].name << "  |  4 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  3  |  " << _board[2][0].name << "  |  " << _board[2][1].name;
  cout << "  |  " << _board[2][2].name << "  |  " << _board[2][3].name;
  cout << "  |  " << _board[2][4].name << "  |  " << _board[2][5].name;
  cout << "  |  " << _board[2][6].name << "  |  " << _board[2][7].name << "  |  3 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  2  |  " << _board[1][0].name << "  |  " << _board[1][1].name;
  cout << "  |  " << _board[1][2].name << "  |  " << _board[1][3].name;
  cout << "  |  " << _board[1][4].name << "  |  " << _board[1][5].name;
  cout << "  |  " << _board[1][6].name << "  |  " << _board[1][7].name << "  |  2 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << "     |     |     |     |     |     |     |     |     |    " << endl;
  cout << "  1  |  " << _board[0][0].name << "  |  " << _board[0][1].name;
  cout << "  |  " << _board[0][2].name << "  |  " << _board[0][3].name;
  cout << "  |  " << _board[0][4].name << "  |  " << _board[0][5].name;
  cout << "  |  " << _board[0][6].name << "  |  " << _board[0][7].name << "  |  1 " << endl;
  cout << "     |_____|_____|_____|_____|_____|_____|_____|_____|    " << endl;
  cout << endl;
  cout << "        a     b     c     d     e     f     g     h       " << endl;
  cout << endl;
}

//Returns the location of the king for the suit of the color "color"
//Also loads the two opposing teams
point chess::get_king(string color, vector<test_piece> & white_team, vector<test_piece> & black_team)
{
  point king; //Holds onto king's location
  test_piece add; //Holds onto a piece's location/name

  //Search through entire board
  for(int r = 0; r < ROWS; ++r)
  {
    for(int c = 0; c < COLS; ++c)
    {
      if(_board[r][c].name != BLANK_NAME && islower(_board[r][c].name)) //White
      {
        //Add this piece to the white team
        add.name = _board[r][c].name;
        add.loc = make_pair(r, c);
        white_team.push_back(add);
        //If we are WHITE, then this is our king
        if(color == WHITE && _board[r][c].name == 'k')
          king = make_pair(r, c);
      }
      else if(_board[r][c].name != BLANK_NAME && isupper(_board[r][c].name)) //Black
      {
        //Add this piece to the black team
        add.name = _board[r][c].name;
        add.loc = make_pair(r, c);
        black_team.push_back(add);
        //If we are BLACK, then this is our king
        if(color == BLACK && _board[r][c].name == 'K')
          king = make_pair(r, c);
      }
    }
  }
  return king; //Return the king we found
}

//Print out all the possible locations of a game piece and return how many
//choices there are
int chess::print_path(point at)
{
  int choice = 1; //Labels a move with a number so it's easier for user to pick one

  if(_board[at.first][at.second].path.empty()) //Are there any possible moves?
    return 0; //No, so return
  //Loop through possible locations
  for(auto p = _board[at.first][at.second].path.begin(); p != _board[at.first][at.second].path.end(); ++p)
  { 
    cout << choice << ". " << convert_ints_to_cols(p->second) << p->first + 1 << endl; //Print it out
    choice = choice + 1; //Increment numbering for the next one
  }
  return choice - 1; //Returns how many choices there are
}

//Reset all pieces as not being able to en passant
void chess::mark_as_cant_ep(void)
{
  //Loop through entire board
  for(int r = 0; r < ROWS; ++r)
  {
    for(int c = 0; c < COLS; ++c)
      _board[r][c].can_ep = false;
  }
}

//Clear all paths of every piece
void chess::empty_all_paths(void)
{
  //Loop through entire board
  for(int r = 0; r < ROWS; ++r)
  {
    for(int c = 0; c < COLS; ++c)
      empty_path(make_pair(r,c)); //Empty the path at this point
  }
}

//***************************************************************************************************************
//          IMPLEMENTATION FOR GAME CLASS
//***************************************************************************************************************

//This class is a child class of the chess class. This class interacts with the players
//to ask where they would like to move their game pieces, and to tell them if their king
//is in danger (check/checkmate). It also determines all the different moves of the different
//types of game pieces, including the normal moves and the special moves. It also keeps track
//of all the captured game pieces of each team. Furthermore, this class will move the game
//pieces according to the player's choice, as long as it is a valid move. This class also
//has checks in place to make sure that a move won't put the team's king in danger (check/
//checkmate), and will also be able to see if the king is currently in danger, and if he
//can get out of danger or not.

//Function to move the game piece, and to do the special moves if it was chosen
void game::move(point from, point to, string color)
{
  //Check if catsling, then castle
  castle(from, to, color);
  //Check if en passant, then capture by en passant
  capture_en_passant(from, to, color);
  //Check if player chose a move that allows for capture by en passant
  enemy_can_en_passant(from, to, color);
  capture(to, color); //Capture the piece at the place we moved to
  mark_as_moved(from); //Mark game piece as having moved
  set_piece(get_piece(from), to); //Replace new location with my piece
  set_piece(BLANK, from); //Replace old location with a blank space
  promote(to); //Check if it's a pawn that now needs promotion
}

//Function to remove game piece from board and put it into the correct captured list
void game::capture(point to, string color)
{
  char name = get_name(to); //Check if it is a blank spot or occupied spot

  if(name != ' ' && color == WHITE) //I am white, and it is occupied
    black_captured.push_back(get_name(to)); //So add black piece to its captured list
  else if(name != ' ') //Else I am black and it is occupied
    white_captured.push_back(get_name(to)); //So add white piece to its captured list
}

//Determines if the user wants to move a white or black pawn
void game::move_pawn(point from, string color, int flag)
{
  if(color == WHITE) // Move upwards
    move_white_pawn(from, color, flag);
  if(color == BLACK) // Move downwards
    move_black_pawn(from, color, flag);
}

//Print out the list of captured game pieces
void game::print_captured(void)
{
  cout << "Captured white pieces: ";
  for (char w : white_captured) //print list of captured whites
    cout << w << ' ';
  cout << endl << endl << "Captured black pieces: ";
  for (char b : black_captured) //print list of captured blacks
    cout << b << ' ';
  cout << endl << endl;
}

//This function will prompt the user for what game piece they would like to move
//and to where. All checks will be in place to test if they chose the correct piece
//that belongs to their color, and if they are valid entries in accordance to the
//boundaries of the board. Furthermore, it will print out the list of possible
//locations that piece could move to, and let the user select one out of them all.
//The move will be initiated, and positions of game pieces will be updated.
void game::prompt_to_move(string color)
{
  bool valid = false;     //indicates if entries are valid
  char from[4];           //response from user of where to move from
  int to;                 //response from user of where to move to
  point source;           //responses converted to point values
  point dest;             //responses converted to point values
  int fc = 0;             //from column converted to int (cols = letters)
  int fr = 0;             //from char rep of row to int rep of row
  int options = 0;        //Number of possible moves the piece can take

  do
  {
    do
    {
      do
      {
        // Get starting location of piece
        cout << "Location of piece to move (column then row, i.e. A2): ";
        cin.get(from, 4);
        cin.ignore(100, '\n');

        // Check if it's a valid entry
        if(((from[0] >= 'A') && (from[0] <= 'H')) || ((from[0] >= 'a') && (from[0] <= 'h'))) // Column correct?
        {
          fr = from[1] - '0'; // Convert number of char type to int
          if(fr >= 1 && fr <= 8) // Row correct?
            valid = true;
        }
        if(!valid)
          cout << "Invalid entry, please enter a valid column, and then a valid row!" << endl;
      }while(!valid);


      //Convert to point
      fc = convert_cols_to_ints(from[0]);
      source = make_pair(fr - 1, fc);

      // Check if it's their color piece
      valid = is_my_color(source, color);
      if(!valid)
        cout << "Invalid entry, that is not your color!" << endl;
    }while(!valid);

    //Load paths for the piece
    load_path(source, color, CHECK_KING);

    //Print valid paths for that piece
    options = print_path(source);
    if(!options)
    {
      valid = false;
      cout << endl << "There are no possible moves for piece. Please pick another piece." << endl;
    }
  }while(!valid);

  do
  {
    //Reset to invalid
    valid = false;
    //Get valid paths

    // Get ending location of piece
    cout << endl << "Destination of piece (column the row, i.e. '1' for option 1): ";
    cin >> to;
    while(cin.fail()) //Make sure they entered an integer
    {
      cin.clear(); //Clear input to prepare for new entry
      cin.ignore(100, '\n');
      cout << "Invalid entry type. Please enter only integers." << endl; //Output that there was an error
      //Prompt user for another input
      cout << endl << "Destination of piece (column the row, i.e. '1' for option 1): ";
      cin >> to;
    }
    cin.ignore(100, '\n');
    cout << endl;

    // Check if it's a valid entry
    options = num_in_path(source);
    for(int o = 1; o <= options; ++o)
    {
      if(to == o)
        valid = true;
    }

    if(!valid)
      cout << "Sorry, invalid move" << endl;
  }while(!valid);

  //Make the dest point
  dest = get_location(source, to - 1);

  //Move
  move(source, dest, color);

  //Clear old possible paths to refill with new positions later on
  empty_path(source);
  empty_path(dest);

}

//This function will discern what type of game piece the user has chosen,
//and call the corresponding function to build the path of possible locations
//the game piece can go to
void game::load_path(point from, string color, int flag)
{
  char name = get_name(from); //Get the name of the game piece

  if(name == 'p') move_white_pawn(from, color, flag); //Is it a white pawn?
  else if(name == 'P') move_black_pawn(from, color, flag); //Is it a black pawn?
  else if(name == 'r' || name == 'R') move_rookie(from, color, flag); //Is it a rookie?
  else if(name == 'n' || name == 'N') move_knight(from, color, flag); //Is it a knight?
  else if(name == 'b' || name == 'B') move_bishop(from, color, flag); //Is it a bishop?
  else if(name == 'k' || name == 'K') move_king(from, color, flag); //Is it a king?
  else if(name == 'q' || name == 'Q') move_queen(from, color, flag); //Is it a queen?
}

//This function will build the possible paths for a white pawn, checking to see if it's the 
//correct color and if the move will put the king in danger. A pawn can move in two different
//ways. It can move up one or up two if it hasn't moved before.
void game::move_white_pawn(point from, string color, int flag)
{
  //Keeps track of whether or not we need to check to see if the king is in danger or not
  bool in_danger = false;

  point one = make_pair(from.first + 1, from.second); //Move up one
  if(one.first < ROWS && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, one, color, REGULAR); //Returns whether or not he's in danger
  if(one.first < ROWS && occupied(one, color) == SPACE && !in_danger) //Is the move valid?
  {
    build_path(from, one); //Add to path
    if(!has_moved(from)) //First time moving = can move up two
    {
      point two = make_pair(from.first + 2, from.second); //Move up two
      in_danger = false; //Reset flag
      if(two.first < ROWS && flag == CHECK_KING) //We need to check the king
        in_danger = will_be_checked(from, two, color, REGULAR); //Returns whether or not he's in danger
      if(two.first < ROWS && occupied(two, color) == SPACE && !in_danger) //Is the move valid?
        build_path(from, two); //Add to path
    }
  }
  //Can it capture enemies?
  point upper_right = make_pair(from.first + 1, from.second + 1); //Enemy on the right?
  in_danger = false; //Reset flag
  if(upper_right.first < ROWS && upper_right.second < COLS && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, upper_right, color, REGULAR); //Returns whether or not he's in danger
  if(upper_right.first < ROWS && upper_right.second < COLS && occupied(upper_right, color) == NOT_MY_COLOR && !in_danger) //Is the move valid?
    build_path(from, upper_right); //Add to path
  point upper_left = make_pair(from.first + 1, from.second -1); //Enemy on the left?
  in_danger = false; //Reset flag
  if(upper_left.first < ROWS && upper_left.second >= 0 && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, upper_left, color, REGULAR); //Returns whether or not he's in danger
  if(upper_left.first < ROWS && upper_left.second >= 0 && occupied(upper_left, color) == NOT_MY_COLOR && !in_danger) //Is the move valid?
    build_path(from, upper_left); //Add to path
}

//Builds possible paths for black pawn and checks to see if it's the correct color and if
//the move will put the king in danger. A pawn can move in two different ways. It can move
//up one or up two if it hasn't moved before.
void game::move_black_pawn(point from, string color, int flag)
{
  //Keeps track of whether or not we need to check to see if the king is in danger or not
  int in_danger = false; 

  point one = make_pair(from.first - 1, from.second); //Move up one
  if(one.first >= 0 && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, one, color, REGULAR); //Returns whether or not he's in danger
  if(one.first >= 0 && occupied(one, color) == SPACE && !in_danger) //Is the move valid?
  {
      build_path(from, one); //Add to path
    if(!has_moved(from)) //First time moving = can down up two
    {
      point two = make_pair(from.first - 2, from.second); //Move up two
      in_danger = false; //Reset flag
      if(two.first >= 0 && flag == CHECK_KING) //We need to check the king
        in_danger = will_be_checked(from, two, color, REGULAR); //Returns whether or not he's in danger
      if(two.first >= 0 && occupied(two, color) == SPACE && !in_danger) //Is the move valid?
        build_path(from, two); //Add to path
    }
  }
  //Can it capture enemies?
  point lower_right = make_pair(from.first - 1, from.second + 1); //Enemy on the right?
  in_danger = false; //Reset flag
  if(lower_right.first >= 0 && lower_right.second < COLS && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, lower_right, color, REGULAR); //Returns whether or not he's in danger
  if(lower_right.first >= 0 && lower_right.second < COLS && occupied(lower_right, color) == NOT_MY_COLOR && !in_danger) //Is the move valid?
      build_path(from, lower_right); //Add to path
  point lower_left = make_pair(from.first - 1, from.second - 1); //Enemy on the left?
  in_danger = false; //Reset flag
  if(lower_left.first >= 0 && lower_left.second >= 0 && flag == CHECK_KING) //We need to check the king
    in_danger = will_be_checked(from, lower_left, color, REGULAR); //Returns whether or not he's in danger
  if(lower_left.first >= 0 && lower_left.second >= 0 && occupied(lower_left, color) == NOT_MY_COLOR && !in_danger) //Is the move valid?
    build_path(from, lower_left); //Add to path
}

//Build possible paths for the rookie piece while checking if the move will put the king in danger.
//A rook can move in four directions: up, down, left, and right.
void game::move_rookie(point from, string color, int flag)
{
  int occupado = 0; //Indicates if the position is occupied, and by what
  //Keeps track of how many pieces not my color I've already hit in this direction, as we can't go further than 1 piece not my color
  int hit = 0;
  //Rookies can only move up/down/right/left
  point right = make_pair(from.first, from.second + 1);
  point left = make_pair(from.first, from.second - 1);
  point top = make_pair(from.first + 1, from.second);
  point bottom = make_pair(from.first - 1, from.second);

  occupado = occupied(right, color); //Check if this location is occupied
  while(right.second < COLS && occupado != MY_COLOR && !hit) //Can go right
  {
    if(flag == CHECK_KING && !will_be_checked(from, right, color, REGULAR)) //Will it put the king in danger?
      build_path(from, right); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, right); //Add to path
    ++right.second; //Continue to check in this direction if we can
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(right, color); //Check if next location is occupied
  }
  hit = 0; //Reset for next point
  occupado = occupied(left, color); //Check if location is occupied
  while(left.second >= 0 && occupado != MY_COLOR && !hit) // Can go left
  {
    if(flag == CHECK_KING && !will_be_checked(from, left, color, REGULAR)) //Will it put the king in danger?
      build_path(from, left); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, left); //Add to path
    --left.second; //Continue to check in this direction if we can
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(left, color); //Check if the next location is occupied
  }
  hit = 0; //Reset for next point
  occupado = occupied(top, color); //check if location is occupied
  while(top.first < ROWS && occupado != MY_COLOR && !hit) //Can go up
  {
    if(flag == CHECK_KING && !will_be_checked(from, top, color, REGULAR)) //Will it put the king in danger?
      build_path(from, top); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, top); //Add to path
    ++top.first; //Continue to check in this direction if we can
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(top, color); //Check if next location is occupied
  }
  hit = 0; //Reset for next point
  occupado = occupied(bottom, color); //Check if location is occupied
  while(bottom.first >= 0 && occupado != MY_COLOR && !hit) //Can go down
  {
    if(flag == CHECK_KING && !will_be_checked(from, bottom, color, REGULAR)) //Check if king will be put in danger
      build_path(from, bottom); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, bottom); //Add to path
    --bottom.first; //Continue to check in this direction if we can
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(bottom, color); //Check if next location is occupied
  }
}

//Build list of possible moves for the knight and checking if the king will be put in danger
//based on certain moves. A knight can move to 8 possible squares.
void game::move_knight(point from, string color, int flag)
{
  //Knight has 8 possible moves
  point move;

  move = make_pair(from.first + 1, from.second - 2); // up one, left 2
  if(move.first < ROWS && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 1, from.second + 2); // up one, right 2
  if(move.first < ROWS && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 2, from.second - 1); // up two, left 1
  if(move.first < ROWS && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 2, from.second + 1); // up two, right 1
  if(move.first < ROWS && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 1, from.second + 2); // down 1, right 2
  if(move.first >= 0 && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 1, from.second - 2); // down 1, left 2
  if(move.first >= 0 && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 2, from.second - 1); // down 2, left 1
  if(move.first >= 0 && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 2, from.second + 1); // down 2, right 1
  if(move.first >= 0 && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
}

//Build list of possible moves for the bishop as long as certain moves are valid.
//A bishop can move in four diagonal directions: upper right, upper left, lower left, and lower right.
void game::move_bishop(point from, string color, int flag)
{
  int occupado = 0; //Indicates if the position is occupied, and by what
  //Keeps track of how many pieces not my color I've already hit in this direction, as we can't go further than 1 piece not my color
  int hit = 0;
  //Bishop moves diagonal in the following directions
  point upper_right = make_pair(from.first + 1, from.second + 1);
  point upper_left = make_pair(from.first + 1, from.second - 1);
  point lower_left = make_pair(from.first - 1, from.second - 1);
  point lower_right = make_pair(from.first - 1, from.second + 1);

  occupado = occupied(upper_right, color); //Is the location occupied?
  while(upper_right.first < ROWS && upper_right.second < COLS && occupado != MY_COLOR && !hit) //Can go to the upper right
  {
    if(flag == CHECK_KING && !will_be_checked(from, upper_right, color, REGULAR)) //will the king be put in danger?
      build_path(from, upper_right); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, upper_right); //Add to path
    ++upper_right.first; //Continue to check in this direction if we can
    ++upper_right.second;
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(upper_right, color); //Is the next location occupied?
  }
  hit = 0; //Reset for next point
  occupado = occupied(upper_left, color); //Is the location occupied
  while(upper_left.first < ROWS && upper_left.second >= 0 && occupado != MY_COLOR && !hit) //Can go to the lower left
  {
    if(flag == CHECK_KING && !will_be_checked(from, upper_left, color, REGULAR)) //will king be in danger?
      build_path(from, upper_left); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, upper_left); //Add to path
    ++upper_left.first; //Continue to check in this direction if we can
    --upper_left.second;
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(upper_left, color); //IS the next location occupied
  }
  hit = 0; //Reset for next point
  occupado = occupied(lower_left, color); //Is this location occupied
  while(lower_left.first >= 0 && lower_left.second >= 0 && occupado != MY_COLOR && !hit) //Can go to the lower left
  {
    if(flag == CHECK_KING && !will_be_checked(from, lower_left, color, REGULAR)) //Will king be in danger
      build_path(from, lower_left); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, lower_left); //Add to path
    --lower_left.first; //Continue to check in this direction if we can
    --lower_left.second;
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(lower_left, color); //Check if next location is occupied
  }
  hit = 0; //Reset for next point
  occupado = occupied(lower_right, color); //Is location occupied
  while(lower_right.first >= 0 && lower_right.second < COLS && occupado != MY_COLOR && !hit) //Can go to the lower right
  {
    if(flag == CHECK_KING && !will_be_checked(from, lower_right, color, REGULAR)) //Will king be in trouble?
      build_path(from, lower_right); //Add to path
    else if(flag == DONT_CHECK_KING)
      build_path(from, lower_right); //Add to path
    --lower_right.first; //Continue to check in this direction if we can
    ++lower_right.second;
    if(occupado == NOT_MY_COLOR) //Already met one opposing piece, can't continue further past
      ++hit;
    occupado = occupied(lower_right, color); //Is next location occupied
  }
}

//Build list of possible moves for king as long as it doesn't put him in danger.
  //King can move in 8 possible directions (not including castling), and only if it won't be in check.
void game::move_king(point from, string color, int flag)
{
  point move; //Keeps track of the move a king can make

  move = make_pair(from.first, from.second - 1); //Go to the left
  if(move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first, from.second + 1); //Go to the right
  if(move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 1, from.second); //Go down
  if(move.first >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 1, from.second); //Go up
  if(move.first < ROWS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 1, from.second + 1); //Go to the upper right
  if(move.first < ROWS && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 1, from.second - 1); //Go to the lower left
  if(move.first >= 0 && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first + 1, from.second - 1); //Go to the upper left
  if(move.first < ROWS && move.second >= 0 && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  move = make_pair(from.first - 1, from.second + 1); //Go to the lower right
  if(move.first >= 0 && move.second < COLS && occupied(move, color) != MY_COLOR && (flag == DONT_CHECK_KING || (flag == CHECK_KING && !will_be_checked(from, move, color, REGULAR)))) //Is the move valid?
    build_path(from, move); //Add to path
  can_castling(from, color, flag); //Check if castling is ok, and if so, add it to path
}

//Build list of possible moves for the queen as long as her moves doesn't put the king in danger.
//Queen can move in any directions both a rookie and bishop can move.
void game::move_queen(point from, string color, int flag)
{
  move_rookie(from, color, flag); //Can move like a rook
  move_bishop(from, color, flag); //Can move like a bishop
}

//This function will check if the location is occupied, and if it is, is it a piece of my own
//team or of the enemy team? Return values that indicate which accordingly
int game::occupied(point location, string color)
{
  char name; //To hold onto name of a piece

  //Check if location is in range
  if((location.first >= ROWS) || (location.first < 0) || (location.second >= COLS) || (location.second < 0))
    return NOT_MY_COLOR; //If not, then it's not a valid location

  name = get_name(location); //Get piece at the location we want to check
  if(color == WHITE) //We are white
  {
    if(name != ' ') // Is it occupied?
    {
      if(islower(name))//Is it my own? Can't move if lowercase (white)
        return MY_COLOR; // It is also white
     return NOT_MY_COLOR; // It is black = can capture
    }
    return SPACE; // It is a blank space
  }
  // We are BLACK
  if(name != ' ') // Is it occupied?
  {
    if(isupper(name))//Is it my own? Can't move if uppercase (black)
      return MY_COLOR; // It is also black
   return NOT_MY_COLOR; // It is white = can capture
  }
  return SPACE;  // It is a blank space
}

//Is the game piece at the location I'm looking at a piece of my team or of the enemy?
//Return accordingly
bool game::is_my_color(point location, string color)
{
  char name = get_name(location); //Get the name of the piece we are looking at
  string pcolor = WHITE;

  if(isupper(name))
    pcolor = BLACK; //black = uppercase
  return pcolor == color; //True = it is the same color as me; false = it is not the same color
}

//This function will promote a pawn that has reached the other side of the board. They
//can ask for anything they want as long as it is their own color. No limits as to what
//type of piece, so the team may have more than one queen if it comes to that..
void game::promote(point at)
{
  char pname = get_name(at); //Get name of the game piece to see if it is a pawn
  char name = 'p'; //User entry for what they would like to replace the piece with
  bool valid = true; //Is their entry a valid entry?

  if(pname == 'p' && at.first == 7) // White pawn made it to the other side
  {
    cout << endl << "****************************************************************" << endl;
    cout << "CONGRATULATIONS! You can promote your pawn to anything you want!" << endl;
    cout << "****************************************************************" << endl;
    do
    {// Ask what piece they would like to promote to
      valid = true; //Reset
      cout << "What piece would you like (enter corresponding character: i.e. n for knight): ";
      cin >> name;
      cin.ignore(100, '\n');
      tolower(name);
      // Make sure it is a valid piece type
      if(name != 'n' && name != 'b' && name != 'r' && name != 'q' && name != 'k')
      {
        valid = false;
        cout << "Sorry invalid entry." << endl;
      }
    }while(!valid); //Continue to loop if invalid entry
    set_name(at, name); //Valid entry, replace pawn with the new piece
  }
  else if(pname == 'P' && at.first== 0) // Black pawn made it to the other side
  {
    cout << endl << "****************************************************************" << endl;
    cout << "CONGRATULATIONS! You can promote your pawn to anything you want!" << endl;
    cout << "****************************************************************" << endl;
    do
    {// Ask what they would like to promote to
      valid = true; //Reset
      cout << "What piece would you like (enter corresponding character: i.e. N for knight): ";
      cin >> name;
      cin.ignore(100, '\n');
      toupper(name);
      // Make sure it is a valid piece type
      if(name != 'N' && name != 'B' && name != 'R' && name != 'Q' && name != 'K')
      {
        valid = false;
        cout << "Sorry invalid entry." << endl;
      }
    }while(!valid); //Continue to loop if invalid entry
    set_name(at, name); //Valid entry, replace pawn with the new piece
  }
}

//This function will check to see if the castling move is possible for the king
//The king can only castle with a rookie if and only if both pieces have never moved and the king is not in check
//before and there are no other pieces in between them. DONT_CHECK_KING means we're testing out how to get out of
//check, so our king is currently in check
void game::can_castling(point king, string color, int flag)
{
  //Check if king has moved before or if it's in check
  if(has_moved(king) || flag == DONT_CHECK_KING || checked(color, NOT_TRYING_MOVE))
    return;
  //Check if there are empty squares in between rookie and king on the right
  point right_one = make_pair(king.first, king.second + 1);
  point right_two = make_pair(king.first, king.second + 2);
  point right_rookie = make_pair(king.first, king.second + 3);
  if(get_name(right_one) == ' ' && get_name(right_two) == ' ') //Nothing in between pieces
  {
    if(color == WHITE) //We are white, so we want a white rookie
    {
      if(get_name(right_rookie) == 'r' && !has_moved(right_rookie) && !will_be_checked(king, right_two, color, CASTLING)) //It is the correct rookie and hasn't moved yet
        build_path(king, right_two); //Add to list of possible moves
    }
    else //We are black so we want a black rookie
    {
      if(get_name(right_rookie) == 'R' && !has_moved(right_rookie) && !will_be_checked(king, right_two, color, CASTLING)) //It is the correct rookie and hasn't moved yet
        build_path(king, right_two); //Add to list of possible moves
    }
  }
  //Check if there are empty squares in between rookie and king on the left
  point left_one = make_pair(king.first, king.second - 1);
  point left_two = make_pair(king.first, king.second - 2);
  point left_three = make_pair(king.first, king.second - 3);
  point left_rookie = make_pair(king.first, king.second - 4);
  if(get_name(left_one) == ' ' && get_name(left_two) == ' ' && get_name(left_three) == ' ') //Nothing in between pieces
  {
     if(color == WHITE) //We are white, so we want a white rookie
     {
        if(get_name(left_rookie) == 'r' && !has_moved(left_rookie) && !will_be_checked(king, left_two, color, CASTLING)) //It is the correct rookie and hasn't moved yet
          build_path(king, left_two); //Add to list of possible moves
     }
     else //We are black so we want a black rookie
     {
        if(get_name(left_rookie) == 'R' && !has_moved(left_rookie) && !will_be_checked(king, left_two, color, CASTLING)) //It is the correct rookie and hasn't moved yet
          build_path(king, left_two); //Add to list of possible moves
     }
  }
}

//This function will check if the king is in check, and if so, it will
//output it as so. It will also check if the king is in checkmate, and
//if it is, then the winner will be set, and we will mark that a winner
//has been found
bool game::prompt_if_checked(string color, string & winner)
{
  string enemy;  //Indicates the enemy's color
  int status = checked(color, NOT_TRYING_MOVE); //Check status of king

  if(color == WHITE) //If I am white, then the winner will be black if I am in checkmate
    enemy = BLACK;
  else
    enemy = WHITE; //else I am black and the winner will be white if I am in checkmate

  if(status == CHECKED) //Am I in check?
  {
    cout << endl << "********************************" << endl;
    cout << color << " king is in check!";
    cout << endl << "********************************" << endl;
  }
  else if(status == CHECKMATE) //Am I in checkmate?
  {
    cout << endl << "********************************" << endl;
    cout << color << " king is in checkmate!";
    cout << endl << "********************************" << endl;
    winner = enemy; //The winner is the enemy then
    return true; //We have found a winner, the game is over
  }
  return false; //We have not found a winner yet
}

//This function will update a vector of points by removing the point
//that we don't want anymore.
void game::remove_from_path(vector<point> & path, point match)
{
  vector<point> new_path; //New path without point

  //Copy only good points without matching point
  for(point p : path)
  {
    if(p != match) //If it it's not the point we don't want
      new_path.push_back(p); //Add it to the new path
  }
  while(!path.empty()) //Clear path
    path.pop_back();
  path = new_path; //Make it have the newly updated path
}

//This function will check to see if the king can get out of being in check by either eliminating the attacker
//or by blocking the attacker. If there are multiple attackers, it is impossible to take them all out. This function
//essentially tries out all the possible moves of all of its pieces, to see if the king will get out of danger.
//For every scenario that it runs, it will see if the king will still be in check or not by seeing if any of
//the possible moves of all of the enemy's team will still hit the king. This function will return CHECK if we have
//found a scenario in which the king can be out of danger, and CHECKMATE if there is no hope.
bool game::eliminate_or_block_attacker(string color, point king, vector<test_piece> white_team, vector<test_piece> black_team)
{
  vector<test_piece> my_team; //Keeps track of my pieces
  vector<test_piece> enemy_team; //Keeps track of enemy's pieces
  string enemy_color; //Enemy color
  vector<point> my_old_path; //Old path of the enemy in case it has some stored
  vector<point> my_new_path; //New path of the enemy to work with
  vector<point> enemy_old_path; //Old path of the enemy in case it has some stored
  vector<point> enemy_new_path; //New path of the enemy to work with
  char p_name; //name at location we want to try moving to
  bool hit_king = false; //Indicates whether or not this scenario has an attacker
  
  //Determine enemy color
  if(color == WHITE) //If I am white
  {
    my_team = white_team; //We are white
    enemy_color = BLACK; //They are black
    enemy_team = black_team;
  }
  else
  {
    my_team = black_team; //We are black
    enemy_color = WHITE; //Otherwise they are white
    enemy_team = white_team;
  }

  //For all of my pieces
  for(test_piece m : my_team)
  {
    if(m.loc == king) //Exclude king because we know he can't move if we've gotten to this func
      continue;
    my_old_path = get_path(m.loc); //Hold onto its old path just in case
    empty_path(m.loc); //Empty it out for use
    load_path(m.loc, color, DONT_CHECK_KING); //Build my new path
    my_new_path = get_path(m.loc); //Retrieve new path of mine
    
    //For all of its possible paths
    for(point p : my_new_path)
    {
      //Fake move
      p_name = get_name(p); //Hold onto what was previously there
      set_name(p, m.name); //Pretend we moved the piece
      set_name(m.loc, BLANK_NAME);

      for(test_piece e : enemy_team)
      {
        if(e.loc != p) //Exclude the piece I've captured
        {
          enemy_old_path = get_path(e.loc); //Hold onto its old path just in case
          empty_path(e.loc); //Empty it out for use
          load_path(e.loc, enemy_color, DONT_CHECK_KING); //Build enemy's path
          enemy_new_path = get_path(e.loc); //Retrieve new path of enemy's
          
          //For all of its possible paths
          for(point ep : enemy_new_path)
          {
            if(ep == king) //Does it hit my king?
              hit_king = true; //yes
          }
          //Revert enemy to it's original state
          copy_path(enemy_old_path, e.loc);
        }
      }
      //Undo fake move
      set_name(p, p_name);
      set_name(m.loc, m.name);
      if(!hit_king) //This scenario had no attackers left
      {
        //Revert me to it's original state
        copy_path(my_old_path, m.loc);
        return true; //We are able to move in such a way that eliminates being in check/checkmate
      }
      hit_king = false; //Reset check
    }
    //Revert me to it's original state
    copy_path(my_old_path, m.loc);
  }
  return false; //We are unable to move in such a way that eliminates being in check/checkmate
}

//This function will see if the king can get out of being in check by moving the king to another position
//on the board. It will test out all the possible points it could go to, and see if any of the opposing
//team pieces can still attack the king. If a position will be under attack, it will be removed from
//the king's list of possible moves. By the end, if the list is empty, it means the king cannot move anywhere
//as all the positions will still leave the king in danger, so we return CHECK. But if there are still places
//in the list, then the king is able to move out of danger's way.
bool game::can_move_king(point king, string color, vector<test_piece> white_team, vector<test_piece> black_team)
{
  vector<test_piece> team_to_check; //Keeps track of team to check's locations
  string enemy_color; //Enemy color
  vector<point> enemy_old_path; //Old path of the enemy in case it has some stored
  vector<point> enemy_new_path; //New path of the enemy to work with
  vector<point> kings_old_path; //King's old path
  vector<point> kings_new_path; //King's new path
  vector<point> kings_test_path; //King's test path
  char k_name; //Name at one of king's possible places to move to
  char king_name = get_name(king); //King's name

  kings_old_path = get_path(king); //Hold onto its old path just in case
  empty_path(king); //Empty it out for use
  load_path(king, color, DONT_CHECK_KING); //Build king's path
  kings_new_path = get_path(king); //Get king's new path
  kings_test_path = get_path(king); //Get king's test path

  //Determine enemy color
  if(color == WHITE) //If I am white
  {
    enemy_color = BLACK; //They are black
    team_to_check = black_team; //We will check the black team's pieces
  }
  else
  {
    enemy_color = WHITE; //Otherwise they are white
    team_to_check = white_team; //We will check the white team's pieces
  }

  //For all of my king's possible path
  for(point k : kings_new_path)
  {
    //Fake move
    k_name = get_name(k); //Get name at location k
    k_name = get_name(k); //Hold onto what was previously there
    set_name(k, king_name); //Pretend we moved the king
    set_name(king, BLANK_NAME);

    //For every enemy piece in the opposing team
    for(test_piece e : team_to_check)
    {
      enemy_old_path = get_path(e.loc); //Hold onto its old path just in case
      empty_path(e.loc); //Empty it out for use
      load_path(e.loc, enemy_color, DONT_CHECK_KING); //Build enemy's path
      enemy_new_path = get_path(e.loc); //Retrieve new path of enemy's
    
      //And for all of its possible paths
      for(point p : enemy_new_path)
      {
        if(p == k) //Do they match?
          remove_from_path(kings_test_path, k); //Then take away this possible loc from king's test path
      }

      //Revert enemy to it's original state
      copy_path(enemy_old_path, e.loc);
    }

    //Undo fake move
    set_name(k, k_name);
    set_name(king, king_name);
  }

  //Revert king to it's original state
  copy_path(kings_old_path, king);

  if(kings_test_path.empty()) //King's test path is empty?
    return false; //Yes = can't move anywhere
  return true; //No = can move still
}

//This function will check to see if the king is in check or checkmate, or if it is safe.
//It will run through all of the enemy pieces to see if they are able to hit the king. If
//so, then check to see if the king can move out of the way. If the king cannot move, then
//check to see if another team member can eliminate the attacker or block the attacker.
//However, if there is more then one attacker, it is impossible to take them all out = CHECKMATE.
//If no one can do anything to save the king, then the king is in checkmate and the game ends
//with the other team as the winner.
int game::checked(string color, int flag)
{
  vector<test_piece> white_team; //Keeps track of white pieces' locations
  vector<test_piece> black_team; //Keeps track of black pieces' locations
  vector<test_piece> enemy_team; //Keeps track of enemy team to check their locations
  string enemy_color; //Enemy color
  vector<point> enemy_old_path; //Old path of the enemy in case it has some stored
  vector<point> enemy_new_path; //New path of the enemy to work with
  point king = get_king(color, white_team, black_team); //King's location
  int check = GOOD; //Indicates whether or not we are in check or we are good
  //Indicates whether or not we've already found an attacker
  //More than one is impossible to prevent
  int found_attacker = 0;

  //Determine enemy color
  if(color == WHITE) //If I am white
  {
    enemy_color = BLACK; //They are black
    enemy_team = black_team; //We will check the black team's pieces
  }
  else
  {
    enemy_color = WHITE; //Otherwise they are white
    enemy_team = white_team; //We will check the white team's pieces
  }

  //For every enemy piece in the opposing team
  for(test_piece e : enemy_team)
  {
    enemy_old_path = get_path(e.loc);
    empty_path(e.loc);
    load_path(e.loc, enemy_color, DONT_CHECK_KING);
    enemy_new_path = get_path(e.loc);
    
    //Will any of its possible paths hit my king?
    for(point p : enemy_new_path)
    {
      if(p == king) //Yes
      {
        check = CHECKED; //We are under attack

        //If we are just testing out a move to see if it will put us in check, we can't make any other moves
        //If we are trying to see if we can get out of check, we can test moves
        if(flag == NOT_TRYING_MOVE)
        {
          ++found_attacker; //Count how many attackers we have
          if(found_attacker > 1) //More than one attacker is impossible to deal with
          {
            //Revert enemy to it's original state
            copy_path(enemy_old_path, e.loc);
            return CHECKMATE; //Thus we are in checkmate
          }
          
          if(!can_move_king(king, color, white_team, black_team)) //Can we move the king?
          {
            //No
            if(!eliminate_or_block_attacker(color, king, white_team, black_team)) //Can we eliminate/block the attacker?
              check = CHECKMATE; //No, we are in checkmate
          }
        }
        else
        {
          //Revert enemy to it's original state
          copy_path(enemy_old_path, e.loc);
          return check; //This move will put us in check
        }
      }
    }
    //Revert enemy to it's original state
    copy_path(enemy_old_path, e.loc);
  }

  return check;
}

//Look into the future to see if your possible move will affect the king to make him
//be in check or checkmate. Special attention needs to be made to the special moves
//CASTLING and EN PASSANT. This will try out the move and see if this will leave the
//king in check/checkmate. If it does, then we must reject this move. Otherwise, it
//is okay to do such a move. For the special moves, we must also move other pieces
//to pretend those special moves happened.
int game::will_be_checked(point from, point to, string color, char move)
{
  int check = GOOD; //Indicate if king will be in check or not
  char to_name = get_name(to); //Hold onto location we're moving to
  char from_name = get_name(from); //Hold onto our name
  //vector<point> my_old_path; //Hold onto our old path
  //If we did an en passant move, need to hold onto that pawn we captured and its original location
  char pawn = 'p';
  point pawn_loc;
  //If we did a castling move, we need to move the rookie and hold onto its old/new location
  char rookie = 'r';
  point rookie_new_loc;
  point rookie_old_loc;

  if(move == CASTLING) //If this is a castling attempt
  {
    //Which rookie are we trying to switch with?
    if(to.second > from.second) //King is moving right to swith with rookie on the right
    {
      rookie_old_loc = make_pair(to.first, to.second + 1); //Rookie on the right one square over from king's new loc
      rookie_new_loc = make_pair(to.first, to.second - 1); //Rookie's new location is to the left of king's new loc
    }
    else //King is moving to the left to switch with rookie on the left
    {
      rookie_old_loc = make_pair(to.first, to.second - 2); //Rookie on the left two squares over from king's new loc
      rookie_new_loc = make_pair(to.first, to.second + 1); //Rookie's new location is to the right of king's new loc
    }
    rookie = get_name(rookie_old_loc); //Hold onto the rookie's name
    //Indicate rookie has been moved
    set_name(rookie_old_loc, BLANK_NAME);
    set_name(rookie_new_loc, rookie);
  }

  if(move == ENPASSANT) //If this is an en passant move
  {
    //"to" is the position we want to go to which is behind the piece we will capture
    //WHITE just moved, Black wants to capture white, so it's location is going up the board
    if(color == WHITE)
      pawn_loc = make_pair(to.first + 1, to.second); //Get location of pawn we will capture that hasn't moved yet
    else //BLACK just moved, white wants to capture black, so it location is going down the board
      pawn_loc = make_pair(to.first - 1, to.second); //Get location of pawn we will capture that hasn't moved yet
    pawn = get_name(pawn_loc); //Hold onto this piece's name
    set_name(pawn_loc, BLANK_NAME); //Pretend we captured it to remove it from the board
  }

  //Pretend we moved the piece
  //my_old_path = get_path(from);
  set_name(to, from_name);
  set_name(from, BLANK_NAME);
  if(checked(color, TRY_MOVE) != GOOD) //This move could result in a CHECK/CHECKMATE
    check = CHECKED; //Mark that it will cause a check
  //Undo the move
  set_name(to, to_name);
  set_name(from, from_name);
  //empty_path(from);
  //copy_path(my_old_path, from);

  if(move == CASTLING) //If this is a castling attempt
  {
    set_name(rookie_new_loc, BLANK_NAME); //Remove rookie from its new location
    set_name(rookie_old_loc, rookie); //Put rookie back where it was originally
  }
  if(move == ENPASSANT) //If this is an en passant move
    set_name(pawn_loc, pawn); //Place the piece captured en passant back on the board

  return check; //Return the status
}

//This function will carry out the castling move
//If the king is castling with the right rookie, then we update the location of
//the right rookie, else if it's the left, we update the left rookie. We don't
//need to move the king because it will be covered by the move function. Lastly,
//mark the rookie as having been moved
void game::castle(point king_from, point king_to, string color)
{
  if((get_name(king_from) == 'k' || get_name(king_from) == 'K') && ((king_from.second + 2) == king_to.second)) //Are we castling to the right?
  {
    point rookie_from = make_pair(king_from.first, king_from.second + 3); //Get right rookie
    point rookie_to = make_pair(king_from.first, king_from.second + 1); //Get its new location
    set_piece(get_piece(rookie_from), rookie_to); //Replace rookie's new location with the rookie
    set_piece(BLANK, rookie_from); //Replace old location with a blank space
    mark_as_moved(rookie_to); //Mark rookie as having moved
  }
  else if((get_name(king_from) == 'k' || get_name(king_from) == 'K') && ((king_from.second - 2) == king_to.second)) //Are we castling to the left?
  {
    point rookie_from = make_pair(king_from.first, king_from.second - 4); //Get left rookie
    point rookie_to = make_pair(king_from.first, king_from.second - 1); //Get its new location
    set_piece(get_piece(rookie_from), rookie_to); //Replace rookie's new location with the rookie
    set_piece(BLANK, rookie_from); //Replace old location with a blank space
    mark_as_moved(rookie_to); //Mark rookie as having moved
  }
}

//This function will check to see if the opposing team is able to capture en passant.
//This is where if you have moved your pawn up two squares for the first time, and there
//happens to be an opposing pawn on either your right or left. If there is, then they have
//the option to move diagonally to the square behind you and capture your pawn. This is
//capture by en passant, or "in passing". However, they can only do so in the move right
//after you move your pawn. If they don't, then they forfeit the right to capture in this way.
void game::enemy_can_en_passant(point from, point to, string color)
{
  char name = get_name(from); //Name of current location
  string enemy_color = WHITE; //Indicates the other team's color

  //Determine enemy color
  if(color == WHITE) //I am white so they are black
    enemy_color = BLACK;
  else
    enemy_color = WHITE; //Else I am black and they are white

  point right = make_pair(to.first, to.second + 1); //piece to my left
  point left = make_pair(to.first, to.second - 1); //piece to my right
  if(name == 'p' && (from.first + 2 == to.first)) //I am a White pawn trying to move up two
  {
    point take = make_pair(from.first + 1, from.second); //Point behind me
    if(right.second < COLS && get_name(right) == 'P' && !will_be_checked(right, take, color, REGULAR)) //Right is an enemy pawn
    {
      if(!will_be_checked(right, take, enemy_color, ENPASSANT))
      {
        build_path(right, take); //Add the en passant move to this pawn
        mark_as_can_ep(right); //Mark that we can capture en passant
      }
    }
    if(left.second >= 0 && get_name(left) == 'P' && !will_be_checked(left, take, color, REGULAR)) //Left is an enemy pawn
    {
      if(!will_be_checked(left, take, enemy_color, ENPASSANT))
      {
        build_path(left, take); //Add the en passant move to this pawn
        mark_as_can_ep(left); //Mark that we can capture en passant
      }
    }
  }
  else if(name == 'P' && (from.first - 2 == to.first)) //I am a black pawn and I am moving two
  {
    point take = make_pair(from.first - 1, from.second); //Point behind me
    if(right.second < COLS && get_name(right) == 'p' && !will_be_checked(right, take, color, REGULAR)) //Right is an enemy pawn
    {
      if(!will_be_checked(right, take, enemy_color, ENPASSANT))
      {
        build_path(right, take); //Add this move to this pawn
        mark_as_can_ep(right); //Mark that we can capture en passant
      }
    }
    if(left.second >= 0 && get_name(left) == 'p' && !will_be_checked(left, take, color, REGULAR)) //Left is an enemy pawn
    {
      if(!will_be_checked(left, take, enemy_color, ENPASSANT))
      {
        build_path(left, take); //Add this move to this pawn
        mark_as_can_ep(left); //Mark that we can capture en passant
      }
    }
  }
}

//This function will initiate the en passant move if that is what we are trying to do
//Whether or not we are doing that move now or not, we need to reset all pieces to
//not being able to en passant, and clear their paths
void game::capture_en_passant(point from, point to, string color)
{
  char from_name = get_name(from); //Get name of piece to see if it's a pawn
  char to_name = get_name(to); //Get name of piece to see if it's a blank space
  point take;                  //Point of enemy pawn to take

  if(from_name == 'p' && to_name == ' ' && from.second != to.second) //We are white and will en passant
  {
    take = make_pair(to.first - 1, to.second); //Enemy to capture is behind where we will move to
    capture(take, color); //Capture en passant
    set_piece(BLANK, take); //Replace old location with a blank space
  }
  if(from_name == 'P' && to_name == ' ' && from.second != to.second) //We are black and will en passant
  {
    take = make_pair(to.first + 1, to.second); //Enemy to capture is behind where we will move to
    capture(take, color); //Capture en passant
    set_piece(BLANK, take); //Replace old location with a blank space
  }
  //Reset all pieces as not being able to en passant
  mark_as_cant_ep();
  //Clear all paths of every piece
  empty_all_paths();
}

#endif
