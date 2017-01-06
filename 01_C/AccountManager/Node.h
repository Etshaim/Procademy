#ifndef NODE_H
#define NODE_H

typedef struct DATA
{
	char	id[12];
	char	name[12];
	int		password;
}Data;

typedef struct NODE
{
	struct NODE *pNext;
	struct NODE *pPrev;

	Data _data;
}Node;

Node* pHead;
Node* pTail;

typedef struct HASH
{
	Data _hsData;
	int key;

	struct HASH *pNext;
	struct HASH *pPrev;
}Hash;

Hash *hashTable[10];


#endif //NODE_H