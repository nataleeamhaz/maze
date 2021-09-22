#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "generator.h"

/**
  Given a direction, get its opposite
 
  Parameters:
   - dir: direction
 
  Returns:
   - the opposite direction to dir
 **/
Direction get_opposite_dir(Direction dir) {
    if (dir == NORTH){
        return SOUTH;
    }
    else if (dir == EAST){
        return WEST;
    }
    else if (dir == SOUTH){
        return NORTH;
    }
    else{ //else it's WEST
        return EAST;
    }
    // TODO: implement this function
}

/**
  Given an array of the four directions, randomizes the order of the directions
 
  Parameters:
   - directions: an array of Direction enums of size 4
 
  Returns:
   - nothing - the array should be shuffled in place
 **/
void shuffle_array(Direction directions[]) {
    int r;
    int temp;
    for (int i = 0; i < 4; i++){
        r = rand() % 4;
        temp = directions[i]; //store the old value
        directions[i] = directions[r];
        directions[r] = temp;

    }
    // TODO: implement this function
}

/**
  Recursive function for stepping through a maze and setting its
 connect/walls using the drunken_walk algorithm
 
  Parameters:
   - row: row of the current room
   - col: column of the current room
   - num_rows: number of rows in the maze
   - num_cols: number of columns in the maze
   - maze: a 2D array of maze_room structs representing your maze
  Returns:
   - nothing - the state of your maze should be saved in the maze array
 being passed in (make sure to use pointers correctly!) -> 
 **/
void drunken_walk(int row, int col, int num_rows, int num_cols,
                  struct maze_room maze[num_rows][num_cols]) {
                    struct maze_room *room;
                    room = &maze[row][col];
                    room -> visited = true;
                    Direction dir[] = {NORTH, SOUTH, EAST, WEST};
                    shuffle_array(dir); //not sure
                    for(int i = 0; i < 4; i++){
                        if(get_neighbor(num_rows, num_cols, maze, room, dir[i]) == NULL){
                            room -> connect[dir[i]] = 1;
                        }
                        
                        else{
                         struct maze_room *nextRoom = get_neighbor(num_rows, num_cols, maze, room, dir[i]);
                         if(nextRoom -> visited == false){
                            room -> connect[dir[i]] = 0;
                            drunken_walk(nextRoom -> row, nextRoom -> column, num_rows, num_cols, maze);
                         }
                        
                         else{
                            Direction oppDir = get_opposite_dir(dir[i]);
                            if(nextRoom -> connect[oppDir] != -1){
                            room -> connect[dir[i]] = nextRoom -> connect[oppDir];
                            }
                            else{
                            room -> connect[dir[i]] = 1; 
                            
                        } 
                         }
                            }
                         }
                        }

             
                        
                    
                    



    // TODO: implement this function


/**
  Represents a maze_room struct as an integer based on its connect
 
  Parameters:
   - room: a struct maze_room to be converted to an integer
 
  Returns:
   - the integer representation of a room
 **/
int encode_room(struct maze_room room) {
    int sum = 0;
    struct maze_room *roomPnt = &room;
    if(roomPnt -> connect[NORTH] != -1){
        sum = sum + roomPnt -> connect[NORTH];
    }
     if(roomPnt -> connect[SOUTH] != -1){
        sum = sum + 2*(roomPnt -> connect[SOUTH]);
     }
     if(roomPnt -> connect[WEST] != -1){
        sum = sum + 4*(roomPnt -> connect[WEST]);
     }
        if(roomPnt -> connect[EAST] != -1){
        sum = sum + 8*(roomPnt -> connect[EAST]);
        }
        return sum;
    // TODO: implement this function
}

/**
  Represents a maze_room array as a hexadecimal array based on its connect
 
  Parameters:
   - num_rows: number of rows in the maze
   - num_cols: number of columns in the maze
   - maze: a 2D array of maze_room structs representing the maze
   - result: a 2D array of hexadecimal numbers representing the maze
 
  Returns:
   - nothing - each maze_room in the maze should be represented
     as a hexadecimal number  and put in nums at the corresponding index
 **/
void encode_maze(int num_rows, int num_cols,
                 struct maze_room maze[num_rows][num_cols],
                 int result[num_rows][num_cols]) {

                     for( int i = 0; i < num_rows; i++){
                         for(int j = 0; j < num_cols; j++){
                             result[i][j] = encode_room(maze[i][j]);
                         }
                     }
    // TODO: implement this function
}

/**
  Writes encoded maze to an output file
 
  Parameters:
   - num_rows: number of rows in the maze
   - num_cols: number of columns in the maze
   - encoded_maze: a maze_room array as a hexadecimal array based on its
     connect
   - file_name: the name of the output file for the encoded maze
 
  Returns:
   - 1 if an error occurs, 0 otherwise
 **/
int write_encoded_maze_to_file(int num_rows, int num_cols,
                               int encoded_maze[num_rows][num_cols],
                               char *file_name) {
    int err = 0;

    // open file (create it if necessary)
    FILE *f = fopen(file_name, "w+");
    if (f == NULL) {
        fprintf(stderr, "Error opening file -> \n");
        return 1;
    }
    // write each room's hex value into file
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            int encoded_room = encoded_maze[i][j];
            err = fprintf(f, "%x", encoded_room);
            if (err < 0) {
                fprintf(stderr, "Writing to file failed: %i\n", err);
                return 1;
            }
        }
        // add a newline between rows
        err = fprintf(f, "\n");
        if (err < 0) {
            fprintf(stderr, "Writing to file failed: %i\n", err);
            return 1;
        }
    }
    // close file
    int close = fclose(f);
    if (close == EOF) {
        fprintf(stderr, "Could not close file -> \n");
        return 1;
    }
    return 0;
}

/**
  Main function
 
  Parameters:
   - argc: the number of command line arguments - for this function 4
   - argv: a pointer to the first element in the command line
             arguments array - for this function:
             ["generator", <output file>, <number of rows>, <number of
 columns>]
 
  Returns:
   - 0 if program exits correctly, 1 if there is an error
 **/

int main(int argc, char **argv) {
    srand(time(NULL)); //in order to randomize 
    char *file_name;
    int num_rows;
    int num_cols;

    if (argc != 4) {
        printf("Incorrect number of arguments -> \n");
        printf(
            " -> /generator <output file> <number of rows> <number of columns>\n");
        return 1;
    } else {
        file_name = argv[1];
        num_rows = atoi(argv[2]);
        num_cols = atoi(argv[3]);
    }
    // TODO: implement this function
    struct maze_room newM[num_rows][num_cols];
    initialize_maze(num_rows,num_cols,newM);
    drunken_walk(0, 0, num_rows, num_cols, newM);
    int encodeM[num_rows][num_cols];
    encode_maze(num_rows, num_cols, newM, encodeM);
    write_encoded_maze_to_file(num_rows, num_cols, encodeM, file_name);

    
}
