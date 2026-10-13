//Cioranu Nickolass-Alexandru

#ifndef __STRUCTSFUNCTIONS_H__
#define __STRUCTSFUNCTIONS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structs.h"

/*
	The list used for storing the files
*/

FilesList initFileNode(char *id, int score) {
	FilesList node = allocFilesList();
	node->id = allocStr(ID_LENGTH);
	strcpy(node->id, id);
	node->score = score;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

FilesList addFileNode(FilesList list, char *id, int score) {
	FilesList newNode = initFileNode(id, score);
	if (list == NULL)
		return newNode;
	FilesList iter = list;
	while (iter->next != NULL) {
		iter = iter->next;
	} 
	iter->next = newNode;
	newNode->prev = iter;
	return list;
}

FilesList getFileNode(FilesList list, char *id) {
	FilesList iter = list;
	while (iter != NULL) {
		if (strcmp(iter->id, id) == 0) {
			return iter;
		}
		iter = iter->next;
	}
	return NULL;
}

/*
	The children list for a tree node
*/

ListTree initList() {
	ListTree newList = allocListTree();
	newList->size = 0;
	newList->first = NULL;
	newList->last = NULL;
	return newList;
}

NodeListTree *initNode(Tree data) {
	NodeListTree* newNode = allocNodeListTree();
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//returns the node from the given position
NodeListTree *findNode(ListTree list, int pos) {
	if (list == NULL || pos < 0 || pos >= list->size)
		return NULL;
	NodeListTree *iter = list->first;
	int i = 0;
	while (iter != NULL && i != pos) {
		iter = iter->next;
		i++;
	}
	return iter;
}

ListTree addToList(ListTree list, Tree data, int pos) {
	if (list == NULL) {
		if (pos != 0)
			return list;
		list = initList();
		NodeListTree *newNode = initNode(data);
		list->first = newNode;
		list->last = newNode;
		list->size++;
		return list;
	}
	if (pos < 0 || pos > list->size)
		return list;
	NodeListTree *newNode = initNode(data);
	if (list->size == 0) {
		list->first = newNode;
		list->last = newNode;
		list->size++;
		return list;
	}
	if (pos == list->size) {
		list->last->next = newNode;
		newNode->prev = list->last;
		list->last = newNode;
		list->size++;
		return list;
	}
	NodeListTree *current = findNode(list, pos);
	newNode->prev = current->prev;
	newNode->next = current;
	if (current->prev == NULL)
		list->first = newNode;
	else
		current->prev->next = newNode;
	current->prev = newNode;
	list->size++;
	return list;
}

/*
	The list with files assigned to a marked node
*/

FilesWithWord initFileWithWord(FilesList file) {
	FilesWithWord newNode = allocFilesWithWord();
	newNode->file = file;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//adds a file to a marked node's list with files that the word is assigned to
FilesWithWord addFileWithWord(FilesWithWord list, FilesList file) {
	if (list == NULL) {
		return initFileWithWord(file);
	}
	FilesWithWord iter = list, prev;
	while (iter != NULL) {
		if (iter->file == file)
			return list;
		prev = iter;
		iter = iter->next;
	} 
	FilesWithWord new_node = initFileWithWord(file);
	prev->next = new_node;
	new_node->prev = prev;
	return list;
}

/*
	The tree (multiway retrive/ Trie)
*/

Tree initTree(char value, bool end) {
	Tree root = allocTree();
	root->value = value;
	root->end = end;
	root->children = NULL;
	root->filesWithWord = NULL;
	return root;
}

int nrChildren(Tree tree) {
	if (tree == NULL || tree->children == NULL) {
		return 0;
	}
	return tree->children->size;
}

Tree insertChild(Tree tree, char value, bool end) {
	Tree node = initTree(value, end);
	if (tree == NULL)
		return node;
	if (tree->children == NULL) {
		tree->children = addToList(tree->children, node, 0);
		return tree;
	}
	tree->children = addToList(tree->children, node, tree->children->size);
	return tree;
}

Tree getChild(Tree tree, char value) {
	if (tree == NULL || tree->children == NULL)
		return NULL;
	NodeListTree *tmp = tree->children->first;
	while (tmp != NULL) {
		Tree treeNode = tmp->data;
		if (treeNode->value == value)
			return treeNode;
		tmp = tmp->next;
	} 
	return NULL;
}

//should insert a word, letter by letter, into the tree
//also adds to the file to the marked node list
Tree insertWord(Tree tree, char *word, int index, FilesList file) {
	if (index == (int) strlen(word)) {
		if (tree != NULL) {
			tree->end = true;
			tree->filesWithWord = addFileWithWord(tree->filesWithWord, file);	
		}
		return tree;
	}
	Tree child = getChild(tree, word[index]);
	if (child == NULL) {
		tree = insertChild(tree, word[index], false);
		child = getChild(tree, word[index]);
	} else {
    }
	child = insertWord(child, word, index + 1, file);
	return tree;
}

//returns the marked node of a word
Tree endOfWord(Tree tree, char *word, int index) {
	if (index == (int) strlen(word)) {
		if (tree != NULL && tree->end)
			return tree;
		return NULL;
	}
	Tree child = getChild(tree, word[index]);
	if (child == NULL)
		return NULL;
	return endOfWord(child, word, index + 1);
}

//returns size of a marked node's list
int count(Tree markedNode) {
	FilesWithWord iter = markedNode->filesWithWord;
	int size = 0;
	while (iter != NULL) {
		size++;
		iter = iter->next;
	}
	return size;
}

void fileOccurences(FilesList file, Tree tree, int *occurences) {
	if (tree == NULL)
		return;
	if (tree->end) {
		FilesWithWord iter = tree->filesWithWord;
		while (iter != NULL) {
			if (iter->file == file)
				(*occurences)++;
			iter = iter->next;
		}
	}
	if (tree->children != NULL) {
		NodeListTree *iter = tree->children->first;
		while (iter != NULL) {
			Tree child = iter->data;
			fileOccurences(file, child, occurences);
			iter = iter->next;
		}
	}
}

/*
	Heap
*/

PriQueue initQueue(int maxHeapSize) {
	PriQueue newQ = allocPriQueue();
	newQ->maxHeapSize = maxHeapSize;
	newQ->size = 0;
	newQ->elements = allocItem(maxHeapSize);
	return newQ;
}

Item getMax(PriQueue h) {
	return h->elements[0];
}

int getLeftChild(int i) {
	return 2 * i + 1;
}

int getRightChild(int i) {
	return 2 * i + 2;
}

int getParent(int i) {
	return (i - 1) / 2;
}

void siftUp(PriQueue h, int index) {
	if (index == 0)
		return;
	int parent = getParent(index);
	while (index > 0 && h->elements[parent].prior < h->elements[index].prior) {
		Item aux = h->elements[parent];
		h->elements[parent] = h->elements[index];
		h->elements[index] = aux;
		index = parent;
		parent = getParent(index);
	}
}

void insertInQueue(PriQueue h, FilesList file) {
	if (h->maxHeapSize == h->size) {
		h->maxHeapSize *= 2;
		h->elements = reallocItem(h);
	}
	h->size++;
	h->elements[h->size - 1].prior = file->score;
	h->elements[h->size - 1].file = file;
	siftUp(h, h->size - 1);
}

void siftDown(PriQueue h, int index) {
	int left = getLeftChild(index), right = getRightChild(index), big = index;
	if (left < h->size && h->elements[left].prior > h->elements[big].prior)
		big = left;
	if (right < h->size && h->elements[right].prior > h->elements[big].prior)
		big = right;
	if (big != index) {
		Item tmp = h->elements[index];
		h->elements[index] = h->elements[big];
		h->elements[big] = tmp;
		index = big;
		siftDown(h, index);
	}
}

void removeMax(PriQueue h) {
	if (h == NULL || h->size == 0)
		return;
	h->elements[0] = h->elements[h->size - 1];
	h->size--;
	if (h->size > 0) 
		siftDown(h, 0);
}

#endif