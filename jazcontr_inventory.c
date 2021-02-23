#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Item {
  int key;
  char name[30];
  int threshold;
  int stock;
  float price;
  struct Item *next;
};

typedef struct Item invent_item;
invent_item* inventory[10];

struct Groceries {
  int key;
  int amount;
  struct Groceries *next;
};
typedef struct Groceries grocery_list;

struct Customer {
  char name[40];
  float cash;
  struct Customer *next;
  grocery_list *groceries;
};

typedef struct Customer customer;

customer *queueHead = NULL;
customer *queueTail = NULL;
grocery_list *head = NULL;
grocery_list *tail = NULL;

//creates the customers and adds them to the queue
void createCustomer(char name[], float cash, grocery_list *ptr) {
  customer *temp = (customer *)malloc(sizeof(customer));
  strcpy(temp->name,name);
  temp->cash = cash;
  temp->groceries = ptr;
  if(queueHead == NULL) {
    queueHead = queueTail = temp;
  }
  else {
    queueTail->next = temp;
    queueTail = temp;
  }
}

//creates the grocery list that is a part of the customer 
grocery_list* createGroceries(int key, int amount) {
  grocery_list *temp = (grocery_list *)malloc(sizeof(grocery_list));
  temp->key = key;
  temp->amount = amount;
  if(tail == NULL) {
    head = tail = temp;
  }
  else {
    tail->next = temp;
    tail = temp;
  }
  temp->next = NULL;
  return head;
}

//displays the groceries in the console
void displayGroceries(grocery_list *temp) {
  grocery_list *list = temp;
  while(list != NULL) {
    printf("Grocery Item Key: %d\n",list->key);
    printf("Grocery Item Amount: %d\n",list->amount);
    list = list->next;
  }
}

//displays the customers in the console
void displayCustomers() {
  customer *current = queueHead;
  while(current != NULL) {
    printf("Customer name: %s\n",current->name);
    printf("Customer Amount: %.2f\n",current->cash);
    printf("Groceries: \n");
    displayGroceries(current->groceries);
    current = current->next;
    printf("\n");
  }
}

//removes people from the queue until it is empty
customer* pop() {
  if(queueHead == NULL) { 
    printf("Underflow\n");
    return NULL;
  }
  else {
    customer *temp = queueHead;
    customer *node = (customer *)malloc(sizeof(customer));
    strcpy(node->name,temp->name);
    node->groceries = temp->groceries;
    node->cash = temp->cash;
    node->next = temp->next;
    queueHead = queueHead->next;
    free(temp);
    return node;
  }
}

//this returns the index at which the key will be inserted in the inventory structure array
int makeKey(int data) {
  int key = data % 10;
  return key;
}

//creates the inventory by filling all of the
//position in the array with NULL
void createInventory() {
  for(int i = 0; i < 10; i++) {
    inventory[i] = NULL;
  }
}

//creates the node which stores the information of the key
invent_item* createNode(int key, char name[], int threshold, int stock, float price) {
  invent_item *temp = (invent_item*)malloc(sizeof(invent_item));
  temp->key = key;
  strcpy(temp->name,name);
  temp->threshold = threshold;
  temp->stock = stock;
  temp->price = price;
  temp->next = NULL;
  return temp;
}

//prints the inventory/hash table to the console
void printInventory() {
  for(int i = 0;  i < 10; i++) {
    invent_item *temp = inventory[i];
    printf("Key[%d]: ",i);
    while(temp != NULL) {
      printf("%d -> ",temp->key);
      temp = temp->next;
    }
    printf("\n");
  }
}

//checks to see if a key already exists
//if it does then it returns 1 otherwise it returns 0
int checkKey(int key) {
  float total;
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL ) {
    invent_item *temp = inventory[findkey];
    while(temp != NULL) {
      if(key == temp->key) {
        return 1;
      }
      temp = temp->next;
    }
  }
  return 0;
}

//this function adds the keys to the hash table
void createPair(int key, invent_item* val) {
  int flag = checkKey(key);
  if(flag == 1) {
    printf("KEY WAS FOUND\n");
  }
  else {
    if(inventory[key] == NULL) {
      inventory[key] = val;
    }
    else {
      invent_item *temp = inventory[key];
      while(temp->next != NULL) {
        temp = temp->next;
      }
      temp->next = val;
    }
  }
}

//parses the file with all of the customers
void parseFile(FILE *fp) {
  char str[100];
  fgets(str,100,fp);
  char name[40];
  char fstr[100][250];
    int i, counter = 0,j = 0;
    int key, amount,cash;
    grocery_list *temp;
    //loops until character at i encounters '\0'
    for(i = 0; str[i] != '\0';i++) {
        //gets rid of unnecessary characters
        if((str[i] == '\"') || (str[i] == '{') || (str[i] == '}') || (str[i] == '[') || (str[i] == ']') || (str[i] == '\n')) {
            continue;
        }
        //comma acts as delimiter
        else if(str[i] == ',') {
            //null character is added to the end of the string
            fstr[counter][j] = '\0';
            //counter is incremented to change word
            counter++;
            i++;
            j = 0;
            continue;
        }

        else {
            //assigns necessary characters to fstr
            fstr[counter][j] = str[i];
            j++;
        }
    }
  //sets each piece of information equal to a variable which
  //is used as input for createGroceries and createCustomer function
  for(int i = 0; i <= counter; i++) {
    if(i == 0) {
      strcpy(name,fstr[0]);
    }
    else if(i == 1) {
      cash = atof(fstr[1]);
    }
    else {
      if(i%2 == 0) {
        key = atoi(fstr[i]);
      }
      else {
        amount = atoi(fstr[i]);
        temp = createGroceries(key,amount);
      }
    }
  }
  createCustomer(name,cash,temp);
  head = tail = NULL;
}

//parses the inventory file and puts everything into the hash table
void insertToHash(FILE *fpointer) {
  char str[200];
  fgets(str,200,fpointer);
  char fstr[100][250];
    int i, counter = 0,j = 0;
    int key, threshold, stock;
    float price;
    char name[40];
    //loops until character at i encounters '\0'
    for(i = 0; str[i] != '\0';i++) {
        //gets rid of unnecessary characters
        if((str[i] == '\"') || (str[i] == '{') || (str[i] == '}') || (str[i] == '\n')) {
            continue;
        }
        //comma acts as delimiter
        else if(str[i] == ',') {
            //null character is added to the end of the string
            fstr[counter][j] = '\0';
            //counter is incremented to change word
            counter++;
            i++;
            j = 0;
            continue;
        }

        else {
            //assigns necessary characters to fstr
            fstr[counter][j] = str[i];
            j++;
        }
    }
    //stores everything from the 2d array into variables which are used 
    //as input for makeKey, createNode and createPair which inserts everything into the hash table
    for(int i = 0; i <= counter; i++) {
      if(i == 0) {
        key = atoi(fstr[0]);
      }
      else if(i == 1) {
        strcpy(name,fstr[1]);
      }
      else if(i == 2) {
        threshold = atoi(fstr[2]);
      }
      else if(i == 3) {
        stock = atoi(fstr[3]);
      }
      else if(i == 4) {
        price = atof(fstr[4]);
      }
    }
  int data = makeKey(key);
  invent_item *temp = createNode(key,name,threshold,stock,price);
  createPair(data,temp);
}

//checks to see if anything in the hash table has a low stock.
//if it does it prints out a warning message at the end of the receipt with all of the things that need to be restocked
void checkLS(FILE *fp) {
  int flag = 0;
  for(int key = 0; key < 10; key++) {
    if(inventory[key] != NULL ) {
      invent_item *temp = inventory[key];
      while(temp != NULL) {
       
        if(temp->stock < temp->threshold) {
          if(flag == 0) {
            fprintf(fp, "Warning! The following Item(s) may need to be restocked:\n");
          }
          fprintf(fp,"%03d (%s): %d remain in stock, replenishment threshold is %d\n",temp->key,temp->name,temp->stock,temp->threshold);
        }
        flag = 1;
        temp = temp->next;
      }
    }
  }
}

//finds the node of the key in the hash table and returns that node
invent_item* findKey(int key) {
  float total;
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL ) {
    invent_item *temp = inventory[findkey];
    while(temp != NULL) {
      if(key == temp->key) {
        return temp;
      }
      temp = temp->next;
    }
  }
  return NULL;
}

//decreases the stock of the key if the person has enough cash
void decreaseStock(int key, int amount,FILE *fp) {
  float total;
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL ) {
    invent_item *temp = inventory[findkey];
    while(temp != NULL) {
      if(key == temp->key) {
        temp->stock -= amount;
      }
      temp  = temp->next;
    }
  }
}

//prints the groceries to the receipt file
void fprintgroceries(grocery_list *list, FILE *fp) { 
  grocery_list *temp = list;
  while(temp != NULL) {
    //printf("xyz\n");
    invent_item* node = findKey(temp->key);
    fprintf(fp,"%s x%d @ $%.2f\n",node->name,temp->amount,node->price);
    temp = temp->next;
  }
}

//checks the items out if the customer had enough money to pay for the total
void checkout(grocery_list *list, FILE *fp) { 
  grocery_list *temp = list;
  while(temp != NULL) {
    decreaseStock(temp->key,temp->amount,fp);
    temp = temp->next;
  }
}

//returns the total for the groceries of the customer
float getTotal(int key, int amount) {
  float total;
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL) {
    invent_item *temp = inventory[findkey];
    while(temp != NULL) {
      if(temp->key == key) {
          total = temp->price * amount;
      }
      temp  = temp->next;
    }
  }
  return total;
}

//checks out the groceries of the customer
float checkoutGroceries(grocery_list *temp){
  float total = 0;
  grocery_list *list = temp; 
  while(list != NULL) {
    total = total + getTotal(list->key,list->amount);
    list = list->next;
  }
  return total;
}


//checks out the customers along with their groceries and removes them from the queue
void checkoutCustomers(FILE *fp) {
  float total;
  int i = 0;
  customer *current = queueHead;
  while(1) {
    current = pop();
    if(current == NULL) {
      break;
    }
    fprintf(fp,"Customer - %s\n\n",current->name);
    total = checkoutGroceries(current->groceries);
    fprintgroceries(current->groceries, fp);
    if(current->cash >= total) {
      checkout(current->groceries,fp);
      fprintf(fp,"\nTotal: $%.2f\n",total);
    }
    else {
      fprintf(fp,"Total: $%.2f\n",total);
      fprintf(fp,"Customer did not have enough money and was REJECTED\n");
    }
    fprintf(fp,"Thank you, come back soon!\n");
    fprintf(fp,"-------------------------------------------------------\n");
  }
  checkLS(fp);
}

//enqueues the customers from the input file
void queueCustomers(FILE *fp, char filename[30]) {
  while(!feof(fp)) {
      parseFile(fp);
  }
  FILE *fnew;
  fnew = fopen(filename,"w+");
  checkoutCustomers(fnew);
  fclose(fnew);
}

//adds a key to the hash table based on user input
void addKey(int key, char name[],int threshold, int stock, float price) {
    if(checkKey(key) == 1) {
      printf("Please enter another key. This key is already taken.\n");
    }
    else {
      int data = makeKey(key);
      invent_item *temp = createNode(key,name,threshold,stock,price);
      createPair(data,temp);
    }
}

//prints the hash table to the inventory file
void fprintInventory() {
  FILE *fp;
  fp = fopen("inventory.txt","w+");
  int flag = 0;
  for(int key = 0; key < 10; key++) {
    if(inventory[key] != NULL ) {
      invent_item *temp = inventory[key];
      while(temp != NULL) {
        fprintf(fp,"{%03d, \"%s\", %d, %d, %.2f}\n",temp->key,temp->name,temp->threshold,temp->stock,temp->price);
        temp = temp->next;
      }
    }
  }
  fclose(fp);
}

//deletes the key that the user input
void deleteKey(int key) {
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL ) {
    invent_item *temp = inventory[findkey];
    invent_item *prev = NULL;
    if(temp->key == key && temp->next == NULL) {
      free(temp);
      inventory[findkey] = NULL;
    }
    else
    while(temp != NULL) {
      invent_item *next = temp->next;
      if(temp->key == key) {
        prev->next = next;
        free(temp);
        break;
      }

      else if((temp->key == key) && (prev == NULL)) {
        prev = NULL;
        next = temp->next->next;
        free(temp);
        break;
      }
      prev = temp;
      temp  = temp->next;
    }
  }
}

//restocks the key that the user input with the amount they input
void restock(int key, int amount) {
    float total;
  int findkey = makeKey(key);
  if(inventory[findkey] != NULL ) {
    invent_item *temp = inventory[findkey];
    while(temp != NULL) {
      if(key == temp->key) {
        temp->stock += amount;
      }
      temp  = temp->next;
    }
  }
}

//restocks all of the keys if they have a stock below their threshold
void restockAll() {
  for(int key = 0; key < 10; key++) {
    if(inventory[key] != NULL ) {
      invent_item *temp = inventory[key];
      while(temp != NULL) {
        if(temp->stock < temp->threshold) {
          temp->stock = temp->threshold;
        }
        temp = temp->next;
      }
    }
  }
}


//prints out the sub-menu which allows the user to add,delete,restock or restock all
void submenu() {
  int key, threshold,stock,n = 0;
  char keystr[40];
  char name[40],str[40];
  float price;
  int operation =  0; 
  while(n != 1) {
    printf("\nINVENTORY SUB-MENU: \n");
    printf("1) add\n");
    printf("2) delete\n");
    printf("3) restock\n");
    printf("4) restock all\n");
    printf("5) return to the main menu\n");
    printf("Please enter one of the following options: ");
    scanf("%s",str);
    if(strcmp(str,"add") == 0) {
      scanf("%d \"%[^\"]\" %d %d %f",&key,name,&threshold,&stock,&price);
      addKey(key,name,stock,threshold,price);
      operation++; 
      continue;
    }
    else if(strcmp(str,"delete") == 0) {
      scanf("%d",&key);
      deleteKey(key);
      operation++;
      continue;
    }
    else if(strcmp(str,"restock") == 0) {
      scanf("%s", keystr);
      if(strcmp(keystr,"all") == 0) {
        restockAll();
      }
      else {
        int amount;
        scanf("%d",&amount);
        key = atoi(keystr);
        restock(key,amount);
      }
      operation++;
      continue;
    }
    else if(strcmp(str,"return") == 0) {
      n = 1;
    }
    else {
      printf("\nPlease enter a valid input.\n");
      continue;
    }
  }
}

//this is the main menu which allows the user to checkout their groceries or manage their inventory
void menu() {
  char str[50];
  char str2[50];
  char newfile[30];
  int n = 0;
  while(n != 1) {
    printf("\nMENU: \n");
    printf("1) Check-out Queue of Customers\n");
    printf("2) Manage Inventory\n");
    printf("3) Close Program\n");
    printf("Please enter one of the following options: ");
    scanf("%s",str);
    if(strcmp(str,"checkout") == 0) {
        scanf("%s",str2);
        strcpy(newfile,str2);
        for(int i = 0; i < strlen(newfile);i++) {
          if(newfile[i] == '.') {
            newfile[i] = '\0';
            break;
          }
        }
      strcat(newfile,"_Receipt.txt");
      FILE *fp;
      fp = fopen(str2,"r");
      queueCustomers(fp,newfile);
      fclose(fp);
      continue;
    }
    else if(strcmp(str,"inventory") == 0) {
      submenu();
      continue;
    }
    else if(strcmp(str,"quit") == 0) {
      fprintInventory();
      printf("Closing program...\n");
      n = 1;
    }
    else {
      printf("\nPlease enter a valid input\n");
      continue;
    }
  }
}

//the hash table is created here in the main function
int main() {
    FILE *fpointer;
    fpointer = fopen("inventory.txt","r");
    createInventory();
    while(!feof(fpointer)) {
        insertToHash(fpointer);
    }
    fclose(fpointer);
    menu();
    return 0;
}