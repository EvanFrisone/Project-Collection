#include <stdio.h>
#include <stdlib.h>

#define BROKEN -1
#define FIXED 1

typedef struct part part;
typedef struct partList partList;
typedef struct component component;
typedef struct componentList componentList;
typedef struct repairList repairList;

struct part {
    int cost, ext_part, total_num_parts;
};

struct partList {
    struct part* part;
    int size, capacity;
};

struct component {
    int status, part_req, num_comp_rev;
    int* comp_rev;
};

struct componentList {
    struct component* component;
    int size, capacity;
};

struct repairList {
    int* list;
    int size, capacity;
};

partList* createPartList(int capacity);
void appendPartList(partList* partList, part value);
componentList* createCompList(int capacity);
void appendCompList(componentList* componentList, component value);
int repair(componentList* componentList, partList* partList);
void enqueueRepairList(repairList* repairList, int temp);
repairList* createRepairList(int capacity);

int main() {
    //checks how many parts there are based on user input
    int parts;
    scanf("%d", &parts);
    int capacity = parts - 1;
    //creates the partList struct
    partList* partList = createPartList(capacity);

    //reads the input from the user and stores into the partList struct
    for (int i = 0; i < parts; i++) {
        part newPart;

        scanf("%d %d", &newPart.cost, &newPart.ext_part);
        newPart.total_num_parts = 0;
        appendPartList(partList, newPart);
    }

    //creates the component list
    int components;
    scanf("%d", &components);
    capacity = components - 1;
    componentList* componentList = createCompList(capacity);

    //reads user input and stores the information from the user into the component list
    for (int x = 0; x < components; x++) {
        component newComponent;

        scanf("%d %d", &newComponent.part_req, &newComponent.num_comp_rev);
        newComponent.status = BROKEN;
        newComponent.comp_rev = (int*)malloc(sizeof(int) * newComponent.num_comp_rev);
        for (int y = 0; y < newComponent.num_comp_rev; y++) {
            scanf("%d", &newComponent.comp_rev[y]);
        }
        appendCompList(componentList, newComponent);
    }
    //calls function to repair the ship and returns the total cost
    int cost = repair(componentList, partList);

    //prints the cost to fix the ship and frees the memory that was allocated during the program.
    printf("%d", cost);
    free(componentList->component);
    free(componentList);
    free(partList->part);
    free(partList);

    return 0;
}

//function which allocates space and returns the space allocated
partList* createPartList(int capacity) {
    partList* list_ref = (partList*)malloc(sizeof(partList));
    list_ref->size = 0;
    list_ref->capacity = capacity;
    list_ref->part = (part*)malloc(sizeof(part) * capacity);
    return list_ref;
}

//adds a part to the partList
void appendPartList(partList* partList, part value) {
    if (partList->size == partList->capacity) {
        partList->capacity *= 2;
        partList->part = (part*)realloc(partList->part, partList->capacity * sizeof(part));
    }
    partList->size++;
    partList->part[partList->size - 1] = value;
}

//function which allocates space for the compList and returns the space allocated
componentList* createCompList(int capacity) {
    componentList* list_ref = (componentList*)malloc(sizeof(componentList));
    list_ref->size = 0;
    list_ref->capacity = capacity;
    list_ref->component = (component*)malloc(sizeof(component) * capacity);
    return list_ref;
}

//function that creates the queue
repairList* createRepairList(int capacity) {
    repairList* list_ref = (repairList*) malloc(sizeof(repairList));
    list_ref->size = 0;
    list_ref->capacity = capacity;
    list_ref->list = (int*) malloc(sizeof(int) * capacity);
    return list_ref;
}

//function that adds components to the compList and allocates memory if it is needed
void appendCompList(componentList* componentList, component value) {
    if(componentList->size == componentList->capacity) {
        componentList->capacity *= 2;
        componentList->component = (component*) realloc(componentList->component, componentList->capacity * sizeof(component));
    }
    componentList->component[componentList->size] = value;
    componentList->size++;
}

//adds a component that needs to be repaired to the tail of the queue
void enqueueRepairList(repairList* repairList, int temp) {
    if(repairList->size == repairList->capacity) {
        repairList->capacity *= 2;
        repairList->list = (int*) realloc(repairList->list, repairList->capacity * sizeof(int));
    }
    repairList->list[repairList->size] = temp;
    repairList->size++;
}

//function that finds the total cost of ordering and repairing the ship
int repair(componentList* componentList, partList* partList) {
    int capacity = componentList->size;
    repairList* repairList = createRepairList(capacity);
    int cost = 0;
    //loops through the component list and adds components that are broken to the end of the queue
    for(int i = 0; i < componentList->size; i++) {
        if(componentList->component[i].status == BROKEN) {
            int temp = i;
            enqueueRepairList(repairList, temp);
            for(int j = 0; j < componentList->component[temp].num_comp_rev; j++) {
                temp = componentList->component[temp].comp_rev[j];
                enqueueRepairList(repairList, temp);
            }
        }
    }
    //loops through the component list and if the part is broken and is on the repair list the status is set to fixed
    for(int z = 0; z < repairList->size; z++) {
        if(componentList->component[repairList->list[z]].status == BROKEN) {
            //checks to see if there are any extra parts that can used to repair the component
            if(partList->part[componentList->component[repairList->list[z]].part_req - 1].total_num_parts > 0) {
                componentList->component[repairList->list[z]].status = FIXED;
                partList->part[componentList->component[repairList->list[z]].part_req - 1].total_num_parts--;
                //loop that dequeues the front of the repair list queue
                for(int k = z; k < repairList->size - 1; k++) {
                    repairList->list[k] = repairList->list[k + 1];
                }
                repairList->size--;
                z--;
                //if there is no extra parts of the needed part one is then ordered and the cost is then increased
            } else {
                cost += partList->part[componentList->component[repairList->list[z]].part_req - 1].cost;
                partList->part[partList->part[componentList->component[repairList->list[z]].part_req - 1].ext_part - 1].total_num_parts++;
                componentList->component[repairList->list[z]].status = FIXED;
                //loop that dequeues the front of the repair list queue
                for(int k = z; k < repairList->size; k++) {
                    repairList->list[k] = repairList->list[k + 1];
                }
                repairList->size--;
                z--;
            }
        } else {
            //loop that dequeues the front of the queue
            for(int k = z; k < repairList->size - 1; k++) {
                repairList->list[k] = repairList->list[k + 1];
            }
            repairList->size--;
            z--;
        }
    }
    //frees the memory that was allocated for the repairList
    free(repairList->list);
    free(repairList);
    return cost;
}