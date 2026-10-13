# Cioranu Nickolass-Alexandru 314CD


# Implementation
The program represents an implementation of an file management and indexing system, based on keywords, using doubly linked lists, a Trie (retrieve multiway tree) and a binary heap (priority queue). The system allows inserting, deleting, updating and searching a file by a keyword.

Every file has a list of keywords, which will be represented in the Trie, and every operation of a file will be implemented within the tree. Every node in the tree that represents the last letter of a keyword will have a list of files that the word is assigned to.

# Input

The input process is based on file system operations:
* `ADD <id> <score> <n> <word1> ... <wordn>`: adds a file (id, score) to the system (files list) and inserts the words into the tree
* `DEL <id>`: removes a file from the system
* `ADDKW <id> <word>`: assigns a word to a file
* `DELKW <id> <word`: removes a word from a file (unassign)
* `FIND <word>`: prints all the files that have the keyword given
* `TOPK <word> <k>`: prints the first k files that have the keyword given (based on highest score) 
* `PRINT`: prints all the words in the tree and the files they are assigned to 
	