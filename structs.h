//Cioranu Nickolass-Alexandru

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdbool.h>

#define WORD_LENGTH 101
#define ID_LENGTH 51

/*
	Double linked list for files
*/

typedef struct nodeFiles {
	char *id;
	int score;
	struct nodeFiles *prev, *next;
} *FilesList;

/*
	Trie (Retrieve multiway tree)
*/
struct tree;

typedef struct nodeListTree {
	struct tree *data;
	struct nodeListTree *prev;
	struct nodeListTree *next;
} NodeListTree;

typedef struct listTree {
	int size;
	NodeListTree *first, *last;
} *ListTree;

//List for marked notes, contans files which the word is assigned to
typedef struct listOfFiles {
	FilesList file; // pointer to file node from files list
	struct listOfFiles *next, *prev;
} *FilesWithWord;

typedef struct tree {
	char value;
	bool end; //node is marked
	ListTree children;
	FilesWithWord filesWithWord;
} *Tree;

/*
	Heap
*/

typedef struct item{
	int prior;
	FilesList file;
} Item;

typedef struct heap{
	long maxHeapSize;
	long size;
	Item *elements;
} *PriQueue;




//Function declarations
FilesList initFileNode(char *id, int score);
FilesList addFileNode(FilesList l, char *id, int score);
FilesList getFileNode(FilesList list, char *id);

ListTree initList();
NodeListTree *initNode(Tree data);
NodeListTree *findNode(ListTree list, int pos);
ListTree addToList(ListTree list, Tree data, int pos);

FilesWithWord initFileWithWord(FilesList file);
FilesWithWord addFileWithWord(FilesWithWord l, FilesList file);

Tree initTree(char value, bool end);
int nrChildren(Tree tree);
Tree insertChild(Tree tree, char value, bool end);
Tree getChild(Tree tree, char value);
Tree insertWord(Tree tree, char *word, int index, FilesList file);
Tree endOfWord(Tree tree, char *word, int index);

int count(Tree MarkedNode);
void fileOccurences(FilesList file, Tree tree, int *occurences);

PriQueue initQueue(int maxHeapSize);
Item getMax(PriQueue h);
int getLeftChild(int i);
int getRightChild(int i);
int getParent(int i);
void siftUp(PriQueue h, int index);
void insertInQueue(PriQueue h, FilesList file);
void siftDown(PriQueue h, int index);
void removeMax(PriQueue h);


//operations
void addFile(char *id, int score, int nrWords, char **words, FilesList *files,
	Tree tree, FILE *fout);
void removeChildFromList(ListTree children, NodeListTree *node);
int removeFromMarkedNodes(FilesList file, Tree node, Tree root);
FilesList removeFile(char *id, FilesList files, Tree tree, FILE *fout);
void addKw(char *id, char *word, FilesList files, Tree tree, FILE *fout);
int deleteFromWord(FilesList file, Tree node, Tree root, char *word, int index);
void delKw(char *id, char *word, FilesList *files, Tree tree, FILE *fout);
void find(char *word, Tree tree, FILE *fout);
void printWords(Tree tree, char *string, int length, FILE *fout, Tree root);
void topK(char *word, int k, Tree tree, FILE *fout);

//memory
char *allocStr(int length);
char **allocStrMatrix(int lines, int length);
FilesList allocFilesList();
ListTree allocListTree();
NodeListTree *allocNodeListTree();
FilesWithWord allocFilesWithWord();
Tree allocTree();
PriQueue allocPriQueue();
Item *allocItem(int maxHeapSize);
Item *reallocItem(PriQueue heap);

void FreeFilesList(FilesList files);
void freeList(ListTree list);
void freeStrMatrix(char **matrix, int lines);
Tree freeTree(Tree tree);
void freeQueue(PriQueue h);

#endif