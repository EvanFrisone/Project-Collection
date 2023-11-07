#include <stdio.h>
#include <stdlib.h>

typedef struct Stack Stack;
typedef struct room room;
typedef struct roomList roomList;

//struct which is used to store info about the rooms
struct room {
    int depth;
    int isInStack;
    int isLeaf;
};

//array of rooms
struct roomList {
    room * room;
    int size, capcity;
};

//the stack
struct Stack {
    int * rooms;
    int size, capcity;
};

roomList * createRoomList();
Stack * createStack();
void push(Stack * stack, int roomNum);
void pop(Stack * stack);
int isEmpty(Stack * stack);
int top(Stack * stack);
void appendToList(Stack * stack, int roomNum);

int main(){
    //allocates and creates the stack and the array of rooms
    roomList * roomList = createRoomList();
    Stack * stack = createStack();

    //while loop to stop looping when the terminator value is inputed
    int roomNum = 0;
    while(roomNum != -1){
        scanf("%d", &roomNum);
        //increases the size of the list of rooms if the room number is greater then the size
        if(roomNum > roomList->size){
            roomList->size = roomNum;
        }
        //checks if the room number is already in the stack if not then it gets pushed onto the stack
        if(roomList->room[roomNum].isInStack != 1){
            roomList->room[roomNum].depth = stack->size;
            roomList->room[roomNum].isLeaf = 1;
            roomList->room[roomNum].isInStack = 1;
            //checks if the stack is empty and if it is the room number is no longer a leaf
            if(isEmpty(stack) == 1){
                roomList->room[top(stack)].isLeaf = 0;
            }
            push(stack, roomNum);
            stack->size++;
        //if the room number is already in the stack the room number is no longer a leaf
        //and the value on the stack is popped
        } else {
            roomList->room[roomNum].isLeaf = 0;
            pop(stack);
        }
    }
    int countLeaf = 0;
    int countDepth = 0;
    //counts the amount of corridors and hallways
    for(int i = 0; i < roomList->size; i++){
        if(roomList->room[i].isLeaf == 1){
            countLeaf++;
            countDepth = roomList->room[i].depth + countDepth;
        }
    }
    //prints out the number of hallways and the amount of corridors on the ship
    printf("%d %d", countLeaf, countDepth);

    //frees the dynamic memory
    free(roomList);
    free(stack);
    return 0;
}

//function that allocates memory and innitializes the list of rooms
roomList * createRoomList(){
    roomList * temp = (roomList *) malloc(sizeof(roomList));
    temp->size = 0;
    temp->capcity = 100;
    temp->room = (room *) malloc(sizeof(room) * temp->capcity);
    return temp;
}

//function that allocates memory and innitalizes the size and capcity of the stack
Stack * createStack(){
    Stack * temp = (Stack *) malloc(sizeof(Stack));
    temp->size = 0;
    temp->capcity = 100;
    temp->rooms = (int *) malloc(sizeof(int) * temp->capcity);
    return temp;
}

//function that adds a value to the top of the stack
void push(Stack * stack, int roomNum){
    appendToList(stack, roomNum);
}

//function that adds a room to the room list
void appendToList(Stack * stack, int roomNum){
    if(stack->size >= stack->capcity){
        stack->capcity *= 2;
        stack->rooms = (int *) realloc(stack->rooms, sizeof(int) * stack->capcity);
    }
    stack->rooms[stack->size] = roomNum;
}

//the stack size is decreased and the top value of the stack is removed
void pop(Stack * stack){
    if(stack->size != 0){
        stack->size--;
    }
}

//function to check if the stack is empty
int isEmpty(Stack * stack){
    if(stack->size > 0){
        return 1;
    }
}

//function that returns the top value from the stack
int top(Stack * stack){
    if(stack->size > 0){
        return stack->rooms[stack->size - 1];
    }
}