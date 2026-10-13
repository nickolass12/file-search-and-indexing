//Cioranu Nickolass-Alexandru

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

char *allocStr(int length) {
	char *string = (char *) calloc(length, sizeof(char));
	if (string == NULL) {
		perror("The string was not allocated");
		exit(1);
	}
	return string;
}

char **allocStrMatrix(int lines, int length) {
	char **matrix = (char **) malloc(lines * sizeof(char *));
	if (matrix == NULL) {
		perror("The matrix was not allocated");
		exit(1);
	}
	for (int i = 0; i < lines; i++) {
		matrix[i] = allocStr(length);
	}
	return matrix;
}

FilesList allocFilesList() {
	FilesList mem = (FilesList) malloc(sizeof(struct nodeFiles));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

ListTree allocListTree() {
	ListTree mem = (ListTree) malloc(sizeof(struct listTree));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}


NodeListTree *allocNodeListTree() {
	NodeListTree *mem = (NodeListTree*) malloc(sizeof(struct nodeListTree));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

FilesWithWord allocFilesWithWord() {
	FilesWithWord mem = (FilesWithWord) malloc(sizeof(struct listOfFiles));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

Tree allocTree() {
	Tree mem = (Tree) malloc(sizeof(struct tree));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

PriQueue allocPriQueue() {
	PriQueue mem = (PriQueue) malloc(sizeof(struct heap));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

Item *allocItem(int maxHeapSize) {
	Item *mem = (Item *) malloc(maxHeapSize * sizeof(Item));  
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

Item *reallocItem(PriQueue heap) {
	Item *mem = realloc(heap->elements, heap->maxHeapSize * sizeof(Item));
	if (mem == NULL) {
		perror("The memory was not allocated");
		exit(1);
	}
	return mem;
}

void freeStrMatrix(char **matrix, int lines) {
	for (int i = 0; i < lines; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void FreeFilesList(FilesList files) {
	if (files == NULL)
		return;
	FilesList tmp;
	while (files != NULL) {
		tmp = files;
		files = files->next;
		free(tmp->id);
		free(tmp);
	}
}

void freeList(ListTree list) {
	if (list == NULL)
		return;
	NodeListTree *iter = list->first, *tmp;
	while (iter != NULL) {
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	free(list);
}

Tree freeTree(Tree tree) {
	if (tree == NULL)
		return tree;
	if (tree->end) {
		FilesWithWord iter = tree->filesWithWord, tmp;
		while (iter != NULL) {
			tmp = iter;
			iter = iter->next;
			free(tmp);
		}
		tree->filesWithWord = NULL;
	}
	if (tree->children != NULL) {
		NodeListTree *iter = tree->children->first;
		while (iter != NULL) {
			Tree treeNode = iter->data;
			treeNode = freeTree(treeNode);
			iter = iter->next;
		}
		freeList(tree->children);
	}
	free(tree);
	return NULL;	
}

void freeQueue(PriQueue h) {
	if (h != NULL)
		free(h->elements);
	free(h);
}


