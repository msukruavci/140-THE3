/* DO NOT CHANGE */
#ifndef SOCIALMEDIA_H
#define SOCIALMEDIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_USER_CAPACITY 1000
#define DB_POST_CAPACITY 1000
#define USERNAME_MAX_LEN 100
#define POST_CONTENT_MAX_LEN 1000

typedef struct {
  int day;
  int month;
  int year;
} Date;

typedef struct Node {
  void *data;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct {
  int userId;
  char *username;
  Date *birthday;
  Node *followers_head;
  Node *followers_tail;
  int numOfFollowers;
  int numOfFollowing;
  int liked;
} User;

typedef struct {
  int postId;
  char *content;
  User *author;
  int likes;
} Post;

typedef struct Database {
  User users[DB_USER_CAPACITY];
  Post posts[DB_POST_CAPACITY];
} Database;

typedef struct Cache {
  int capacity;
  int size;
  Node *head;
  Node *tail;
} Cache;

void printUser(User *user);
void printPost(Post *post);
void printUserCache();
void printPostCache();
void printFollowersInReverse();

Node *createNode(void *data);
void appendToCache(Cache *cache, void *data);
void moveToFront(Cache *cache, Node *node);
void removeLast(Cache *cache);
void freeLinkedList(Node *head);

User *fetchUser(int userId);
Post *fetchPost(int postId);
void followUser(int followerId, int followedId);
void likePost(int userId, int postId);

extern Database db;
extern Cache userCache;
extern Cache postCache;

#endif