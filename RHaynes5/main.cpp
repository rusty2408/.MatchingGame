#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace std::chrono;

#ifdef __WIN32__
#include <windows.h>
#define sleep(n) Sleep(1000 * n)  //Windows Sleep in ms

#define clear() system("cls")
#endif

#ifdef __APPLE__
#include <unistd.h>

#define clear() system("clear")
#endif
//should eliminate "term environment variable not set warning but fails to on ios//

/* MATCHING GAME ~~ Rusty Haynes
 * Game uses vector of vectors with chars and integers to formulate matches
 * A hidden board of chars are posted with numbers
 * user is timed through game in relation to board size */

//setting up gameboard
void setupgame(vector<vector<string> >& gamelayout, vector<vector<string> >& view, int rows, int coloumn);

//repost of board between guesses and hidden board
void postBoard(const vector<vector<string> >&);

int main()
{
    srand(time(0));
    int rows, coloumn;
    int multi=-1; //multiplying

    //manages keeping time
    int timing;

    // Game instruct to set up board by row and coloumn
    cout<<"In this game, pairs of letters will be hidden on a rectangular grid."<<endl;
    cout<<"If the letters at these slots match, they will stay in view."<<endl;
    cout<<"If not, after 3 seconds the letters will disappear and be replaced by the numbers."<<endl;
    cout<<"Your job is find all the pairs."<<endl;
    cout<<endl<<endl;

    while(multi<16 || multi>64 || multi % 2 !=0) //the board must be an odd #, no less than 16 or more than 64
    {
        cout << "Size requirement: product of row x col must be between 16 and 64 and be even" << endl;

        cout << "Enter rows: ";
        cin >> rows;
        rows = abs(rows);

        cout << "Enter columns: ";
        cin >> coloumn;
        coloumn = abs(coloumn);

        multi = coloumn * rows;
    }

    //Applying different pairs to game board/layout
    vector<vector<string> > gamelayout(rows), view(rows);
    setupgame(gamelayout, view, rows, coloumn);


    //User is given an exact amount of minutes that coincide with the chosen board size
    timing = (multi / 2) * 30;
    cout<<endl<<endl;
    cout << "Allowing 30 seconds per pair "<<endl;
    cout << "You will have " << timing / 60 << " minutes and " << timing % 60
         << " seconds to find the " << multi / 2 << " pairs"<<endl;
    cout << "Let's play !!" <<endl<<endl;

    /*int timeDuration;
    //steady_clock::time_point begin, end;
    begin = steady_clock::now();*/

    auto begin = steady_clock::now();
    int timeDuration =  duration_cast<std::chrono::seconds>(steady_clock::now() - begin ).count();

    while(true)
    {
        int pos1 = -1, pos2 = -1; //represents chosen positions to flip;

        postBoard(view);
        cout<<endl<<endl;

        //making sure slot guess is on gameboard and not repeated
        while(pos1 < 1 || pos1 > multi)
        {
            cout << "Enter the first slot to view: ";
            cin >> pos1;
            pos1 = abs(pos1);
        }

        while(pos2 < 1 || pos2 > multi || pos2 == pos1)
        {
            cout << "Enter the second slot to view: ";
            cin >> pos2;
            pos2 = abs(pos2);
        }

        // og Row & Col are the first board and new Row & Col represent the symboled
        int ogRow = (pos1 - 1) / coloumn, newRow = (pos2 - 1) / coloumn;
        int ogCol = (pos1 - 1) % coloumn, newCol = (pos2 - 1) % coloumn;

        //if the positions are already flipped over
        if(view[ogRow][ogCol] == view[newRow][newCol])
        {
            postBoard(view);
            cout << "Previously matched" << endl;
        }
        // if they aren't and the user enters to that match
        else if (gamelayout[ogRow][ogCol] == gamelayout[newRow][newCol])
        {
            view[ogRow][ogCol] = gamelayout[ogRow][ogCol];
            view[newRow][newCol] =  gamelayout[newRow][newCol];
            postBoard(view);
            cout << "Match "<<endl;
        }
        // if the pair isn't flipped/correct/previously matched
        else
        {
            vector<vector<string> > wrong = view;
            view[ogRow][ogCol] = gamelayout[ogRow][ogCol];
            view[newRow][newCol] = gamelayout[newRow][newCol];
            postBoard(view);
            cout<<endl<<"No Match "<<endl;
            view = wrong;
        }

        //takes lapsed time from original initialization with AUTO through rest of while loop
        timeDuration = duration_cast<seconds>(steady_clock::now() - begin).count();

       //timeDuration = duration_cast<std::chrono::milliseconds>(end-begin).count();
       // end = steady_clock::now();

        if(timeDuration > timing)
        {
            cout<<endl<<"Time has expired "<<endl;
            cout<<endl<<"Revealed: "<<endl;
            postBoard(view);

            cout<<endl<<"All pairs were: "<<endl;
            postBoard(gamelayout);
            break;
        }
        else if(view == gamelayout)
        {
            cout<<"All pairs matched within " <<timeDuration/60<< " minutes and "
                                              << timeDuration % 60<<" seconds";
        }
        else
        {
            sleep(3);
            clear();
        }


    }



return 0;
}

//vector to store original grid of numbers
// nothing to return <-- use of void funct
void setupgame(vector<vector<string> >& gamelayout, vector<vector<string> >& view, int rows, int coloumn)
{
    int letters = 65; //ascii code of letters begin at 65
    int more = 1;
    vector<string> pairs;

    for(int i=0; i<(rows * coloumn) /2; i++)
    {
        char letpair = char (letters + i);
        string group;
        group += letpair;
        pairs.push_back(group);
        pairs.push_back(group);
    }
    random_shuffle(pairs.begin(), pairs.end());

    for(int i=0; i< rows; i++)
    {
        for(int j=0; j<coloumn; j++)
        {
            gamelayout[i].assign(pairs.begin() + (i * coloumn), pairs.begin() + (i+1) * coloumn);

            view[i].push_back(to_string(more));
            more++;
        }
    }
}

void postBoard(const vector<vector<string> >& gamelayout)
{
    string plot;
    for(int i=0; i<gamelayout.size(); i++)
    {
        for(int j=0; j<gamelayout[i].size(); j++)
        {
            plot = gamelayout[i][j].size() == 1 ? " " : " ";
            cout << plot << gamelayout[i][j];
        }
    cout<<endl;
    }
}

