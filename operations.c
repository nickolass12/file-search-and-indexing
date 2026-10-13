//Cioranu Nickolass-Alexandru

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"


void addFile(char *id, int score, int nrWords, char **words, FilesList *files,
			 Tree tree, FILE *fout) {
	FilesList iter = *files, currentFile;
	while (iter != NULL) {
		if (strcmp(id, iter->id) == 0) {
			fprintf(fout, "EXISTS\n");
			return;
		}
		iter = iter->next;
	}
	*files = addFileNode(*files, id, score);
	currentFile = getFileNode(*files, id);
	for (int i = 0; i < nrWords; i++) {
		//inserts every keyword into the tree
		insertWord(tree, words[i], 0, currentFile);
	}
	fprintf(fout, "OK\n");
}

void removeChildFromList(ListTree children, NodeListTree *node) {
	if (children == NULL || node == NULL)
		return;
	if (node->prev != NULL) {
		node->prev->next = node->next;
	} else {
		children->first = node->next;
	}
	if (node->next != NULL)
		node->next->prev = node->prev;
	else
		children->last = node->prev;
    free(node);
	children->size--;
}

//removes a file from the list of every marked node
//if the node has no more files, it is deleted and returns 1 to delete his parent
int removeFromMarkedNodes(FilesList file, Tree node, Tree root) {
	if (node->end == true) {
		//if the list isn't empty, deletes the file
		FilesWithWord iterFile = node->filesWithWord, nextIter;
		while (iterFile != NULL) {
			nextIter = iterFile->next;
			if (iterFile->file == file) {
				FilesWithWord prev = iterFile->prev, next = iterFile->next;				
				if (prev != NULL) {
					prev->next = next;
				} else {
					node->filesWithWord = next;
				}
				if (next != NULL) {
					next->prev = prev;
				}
				free(iterFile);
			}
			iterFile = nextIter;
		}
		//if the list becomes empty, unmarks the node
		if (node->filesWithWord == NULL) {
			node->end = false;
		}
	}
	//if a node has childern, goes through them
	if (node->children != NULL) {
		NodeListTree *iter = node->children->first;
		while (iter != NULL) {
			NodeListTree *next = iter->next;
			//returns 1 (freed), so the child should be removed from the list
			if (removeFromMarkedNodes(file, iter->data, root) == 1) {
				removeChildFromList(node->children, iter);
			}
			iter = next;
		}
		if (node->children->first == NULL) {
			free(node->children);
			node->children = NULL;
		}
	}
	if (node->end == false && node->children == NULL && node != root) {
		free(node);
		return 1;
	}
	return 0;
}

FilesList removeFromFilesList(FilesList files, FilesList currFile) {
	FilesList prev = currFile->prev, next = currFile->next;
	if (prev != NULL)
		prev->next = next;
	else 
		files = next;
	if (next != NULL) 
		next->prev = prev;
	free(currFile->id);
	free(currFile);
	return files;
}

FilesList removeFile(char *id, FilesList files, Tree tree, FILE *fout) {
	FilesList iter = files;
	while (iter != NULL) {
		if (strcmp(iter->id, id) == 0) {
			break;
		}
		iter = iter->next;
	}
	if (iter == NULL) {
		fprintf(fout, "NOT FOUND\n");
		return files;
	}
	//remove file from list of files of marked nodes
	removeFromMarkedNodes(iter, tree, tree);
	//remove file from files list
	files = removeFromFilesList(files, iter);
	fprintf(fout, "OK\n");
	return files;
}

void addKw(char *id, char *word, FilesList files, Tree tree, FILE *fout) {
	FilesList iter = files;
	while (iter != NULL) {
		if (strcmp(id, iter->id) == 0) 
			break;
		iter = iter->next;
	}
	if (iter == NULL) {
		fprintf(fout, "NOT FOUND\n");
		return;
	}
	tree = insertWord(tree, word, 0, iter);
	fprintf(fout, "OK\n");
}

//removes a file from a marked node's list
int deleteFromWord(FilesList file, Tree node, Tree root, char *word, int index) {
	if (index == (int) strlen(word)) {
		if (node->end == true) {
			FilesWithWord iterFile = node->filesWithWord, nextIter;
			//the list is not empty => deleting the file
			while (iterFile != NULL) {
				nextIter = iterFile->next;
				if (iterFile->file == file) {
					FilesWithWord prev = iterFile->prev, next = iterFile->next;				
					if (prev != NULL) {
						prev->next = next;
					} else
						node->filesWithWord = next;
					if (next != NULL) 
						next->prev = prev;
					free(iterFile);
				}
				iterFile = nextIter;
			}
			//list becomes empty => node unmarked
			if (node->filesWithWord == NULL)
				node->end = false;
		}
	} else if (node->children != NULL) {
			NodeListTree *iter = node->children->first, *next;
			while (iter != NULL) {
				next = iter->next;
				Tree child = iter->data;
				if (child->value == word[index]) {
					if (deleteFromWord(file, child, root, word, index + 1) == 1) {
						removeChildFromList(node->children, iter);
						if (node->children->first == NULL) {
                        	free(node->children);
                        	node->children = NULL;
                    	}
					}
				}
				iter = next;
			}
		}
	if (node->end == false && node->children == NULL && node != root) {
		free(node);
		return 1;
	}
	return 0;
}

void delKw(char *id, char *word, FilesList *files, Tree tree, FILE *fout) {
	FilesList iter = *files;
	while (iter != NULL) {
	if (strcmp(id, iter->id) == 0) 
		break;
	iter = iter->next;
	}
	if (iter == NULL) {
		fprintf(fout, "NOT FOUND\n");
		return;
	}
	deleteFromWord(iter, tree, tree, word, 0);
	fprintf(fout, "OK\n");
	//deletes file that have no more words
	int occurences = 0;
	fileOccurences(iter, tree, &occurences);
	if (occurences == 0)
		*files = removeFromFilesList(*files, iter);
}

void find(char *word, Tree tree, FILE *fout) {
	Tree markedNode = endOfWord(tree, word, 0);
	if (markedNode == NULL || markedNode->filesWithWord == NULL) {
		fprintf(fout, "EMPTY\n");
		return;
	}
	int size = count(markedNode);
	char **files = allocStrMatrix(size + 1, ID_LENGTH), *tmp;
	tmp = allocStr(ID_LENGTH);
	// put in strings
	FilesWithWord iter = markedNode->filesWithWord;
	int i = 0;
	while (iter != NULL) {
		strcpy(files[i++], iter->file->id);
		iter = iter->next;
	}
	//sort
	for (i = 0; i < size - 1; i++)
		for (int j = i + 1; j < size; j++)
			if (strcmp(files[i], files[j]) > 0) {
				strcpy(tmp, files[i]);
				strcpy(files[i], files[j]);
				strcpy(files[j], tmp);
			}
	free(tmp);
	fprintf(fout, "%d ", size);	
	for (i = 0; i < size; i++) 
		fprintf(fout, "%s ", files[i]);
	fprintf(fout, "\n");
	freeStrMatrix(files, size + 1);
}


void printWords(Tree tree, char *string, int index, FILE *fout, Tree root) {
	if (root == NULL || root->children == NULL) {
		fprintf(fout, "EMPTY\n");
		return;
	}
	if (tree->end) {
		string[index] = '\0';
		fprintf(fout, "%s ", string);
		find(string, root, fout);
	}
	if (tree->children != NULL) {
		for (char c = 'a'; c <= 'z'; c++) {
			NodeListTree *iter = tree->children->first;
			while (iter != NULL) {
				Tree child = iter->data;
				if (child->value == c) {
					string[index] = child->value;
					printWords(child, string, index + 1, fout, root);
				}
				iter = iter->next;
			}
		}
	}
}

void topK(char *word, int k, Tree tree, FILE *fout) {
	Tree markedNode = endOfWord(tree, word, 0);
	if (markedNode == NULL || markedNode->filesWithWord == NULL) {
		fprintf(fout, "EMPTY\n");
		return;
	}
	int size = count(markedNode);
	PriQueue heap = initQueue(size);
	FilesWithWord iter = markedNode->filesWithWord;
	while (iter != NULL) {
		insertInQueue(heap, iter->file);
		iter = iter->next;
	}
	if (size < k)
		k = size;
	fprintf(fout, "%d ", k);
	for (int i = 0; i < k; i++) {
		Item elem = getMax(heap);
		fprintf(fout, "%s ", elem.file->id);
		removeMax(heap); 
	}
	fprintf(fout, "\n");
	freeQueue(heap);
}