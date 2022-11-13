#include <stdio.h>

#define WIDTH 7
#define DEPTH 6
#define SIZE WIDTH*DEPTH
const char UNKNOWN_PLAYER = '-';
const char PLAYER_ONE = 'r';
const char PLAYER_TWO = 'y';

char board[SIZE];
int height[WIDTH];

int right_diagonal(int x){return x;}
int left_diagonal(int x){return -x;}
int line(int x){return 0;}

int full(int move)
{
    return height[move] >= DEPTH; 
}
int read(char player)
{
    int accepted = 0;
    int move;
    printf("%s turn: ", player == PLAYER_TWO ? "Player Two" : "Player One");
    
    while(!accepted)
    {
        scanf("%i", &move);
        if(move > WIDTH || move < 1)
        {
            printf("Illegal move!\nEnter a slot number between 1 and %i: ", WIDTH);
            continue;
        }
        if(full(move - 1))
        {
            
            printf("Illegal move, slot #%i is full!\nEnter a number of an empty slot: ", move);
            continue;
        }
        accepted = 1;
    }
    
    return move - 1;
}
char at(int x, int y)
{
    if(x >= WIDTH || x < 0) return UNKNOWN_PLAYER;
    if(y >= DEPTH || y < 0) return UNKNOWN_PLAYER;

    return *(board + WIDTH*y + x);
}
void display()
{
    
    for(int y = DEPTH - 1; y >= 0; y--)
    {
            for(int x = 0; x < WIDTH; x++)    
            printf("%c ", at(x,y));
        putchar('\n');
    }
}
int test(int (*f) (int), int inverse, int move_x, int move_y, char player)
{
    int proceed;
    int length = 1;
    int offset_x, offset_y;

    for(offset_x = 1, proceed = 1; proceed; offset_x++)
    {
        offset_y = f(offset_x);
        if(inverse)
        {
            if(at(move_x + offset_y, move_y + offset_x) == player)
            {
                if(++length >= 4) return 1;
                continue;
            }
        }
        else
        {
            if(at(move_x + offset_x, move_y + offset_y) == player)
            {
                if(++length >= 4) return 1;
                continue;
            }
        }
        
        proceed = 0;
    }
    for(offset_x = -1, proceed = 1; proceed; offset_x--)
    {
        offset_y = f(offset_x);
        if(inverse)
        {
            if(at(move_x + offset_y, move_y + offset_x) == player)
            {
                if(++length >= 4) return 1;
                continue;
            }
        }
        else
        {
            if(at(move_x + offset_x, move_y + offset_y) == player)
            {
                if(++length >= 4) return 1;
                continue;
            }
        }
        proceed = 0;
    }

    return 0;
}
int winning(int move_x, int move_y, char player)
{
    int(*functions[3])(int) = {line, left_diagonal, right_diagonal};
    for(int i = 0; i < 3; i++)
        if(test(functions[i], 0, move_x, move_y, player))  return 1;
    if(test(functions[0], 1, move_x, move_y, player)) return 1;
    return 0;    
}
char session()
{
    int round;
    char player;
    int move_x, move_y;

    printf("Welcome to Connect Four:\n");
    display();

    for(round = 0; round < SIZE; round++)
    {
        player = round % 2 ? PLAYER_TWO : PLAYER_ONE;

        move_x = read(player);
        move_y = height[move_x]++;
        board[move_y*WIDTH + move_x] = player;

        display();
        
        if(winning(move_x,move_y, player)) return player; 
    }
    
    return UNKNOWN_PLAYER;
}
int init()
{
    for(int i = 0; i < SIZE; i++)
        board[i] = UNKNOWN_PLAYER;
    for(int i = 0; i < WIDTH; i++)
        height[i] = 0;
    return 0;
}
int main()
{
    if(init())
    {
        printf("Could not initiate the game!");
        return 1;
    }

    char winner = session();
    if(winner == 0)
    {
        printf("Something unexpected occured...\n");
        return 1;
    }
    if(winner == '-')
    {
        printf("Its a tie!\n");
        return 0;
    }
    printf("++++++++++++++++++++++++++++++++++\n");
    printf("The winner is: %s!\n", winner == PLAYER_ONE ? "Player One" : "Player Two");
    printf("++++++++++++++++++++++++++++++++++\n");

    return 0;
}