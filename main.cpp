#ifndef MAIN_CPP
#define MAIN_CPP
/* Created by: Angelic Phan
 * CS 410P, Winter 2019: Advanced C++
 * Steven Libby
 * Final Project
 * Due: March 18, 2019
 *
 * GAME: This is the chess game. The BLACK team are uppercased and the WHITE team are lowercased.
 * The goal of the game is to play until you get the other team's king into checkmate or if they
 * quit in order for you to win. You are to enter the position of a piece you would like to move
 * and a list of possible moves will be generated for you depending on the piece that you have 
 * chosen. However, moves will be omitted from the list if it will leave your king in check/checkmate.
 * If your king is in check, you cannot move any pieces that won't help you to get out of check.
 * You must only do moves that will get your king out of check. Special moves are supported in the
 * game: castling and en passant. Good luck to you.
 *
 * Description: This file includes main. It will display the game board and any messages to the user.
 * It also prompts the user for input as to move pieces or quit the game.
 */
#include "chess.hpp"

int main(void)
{
  try
  {
    string turn = WHITE;      //Indicates who's turn it is
    string winner = WHITE;    //Indicates who the winner is
    bool win = false;         //Indicates if there is a winner found yet
    char choice = 'M';        //Indicates what command the user choses
    bool invalid_entry = 0;   //Indicates whether they typed a valid command
    game mygame;              //A child object
    chess * play = &mygame;   //A parent pointer to the child class
    
    do
    {
      cout << endl;
      cout << "********************************************************************" << endl;
      cout << "     CHESS     CHESS     CHESS     CHESS     CHESS     CHESS     " << endl;
      cout << "********************************************************************" << endl << endl;

      //Display board with chess pieces set up
      play->display_board();

      //Display captured lists
      mygame.print_captured();

      //Check if king is in danger
      win = mygame.prompt_if_checked(turn, winner); //Prompt if king is checked/in checkmate
      if(win)
        break;

      //Display current player's turn
      cout << "Current turn: " << turn << endl;

      //Menu
      cout << endl << "Commands:    (M)ove    (Q)uit" << endl;

      do
      {
        invalid_entry = 0; //Reset to valid entry
        //Get choice
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(100, '\n');
        cout << endl;

        //Check command
        if(choice == 'M' || choice == 'm')      //User wants to move
        {
          cout << "BLACK are uppercase pieces, WHITE are lowercase pieces" << endl;
          mygame.prompt_to_move(turn); //Move user
        }
        else if(choice == 'Q' || choice == 'q') //User wants to quit the game
        {
          if(turn == WHITE) //White forfeited the game, black wins
            winner = BLACK;
          else
            winner == WHITE; //Black forfeited the game, white wins
          win = true;
        }
        else                                    //Invalid entry
        {
          cout << "Invalid entry" << endl;
          invalid_entry = 1;
        }
      } while(invalid_entry);
      
      //Switch turns
      if(turn == WHITE)
        turn = BLACK;
      else
        turn = WHITE;
    } while((choice != 'Q' && choice != 'q') && !win); //While hasn't quit/won
    
    //Someone has quit/won
    cout << endl << "              CONGRATULATIONS! " << winner << " WON!" << endl << endl;
    cout << "********************************************************************" << endl;
    cout << "        GAME OVER     GAME OVER     GAME OVER     GAME OVER        " << endl;
    cout << "********************************************************************" << endl << endl;

  }
  catch(const char* e)
  {
    cout << e << endl;
  }
  catch(...)
  {
    cout << "ERROR: an uncaught exception occured." << endl;
  }
  
  return 0;
    
}

#endif
