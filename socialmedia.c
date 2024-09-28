#include "socialmedia.h"
#include <stdlib.h>

Database db;
Cache userCache;
Cache postCache;

void printUser(User *user) {
  printf("User: %d, Username: %s, Followers: %d, Following: %d, Liked: %d\n",
         user->userId, user->username, user->numOfFollowers,
         user->numOfFollowing, user->liked);
}

void printPost(Post *post) {
  printf("Post: %d, Author: %s, Likes: %d\n", post->postId,
         post->author->username, post->likes);
}

void printUserCache() {
    Node *curr = userCache.head;
  /* TODO: Start from head and printUser */
    printf("UserCache:\n");
    for(; curr != NULL; curr = curr -> next) {
        printUser((User *)curr -> data);
    }
}

void printPostCache() {
    Node *curr = postCache.tail;
  /* TODO: Start from tail and printPost */
    printf("PostCache:\n");
    for(; curr != NULL; curr = curr -> prev) {
        printPost((Post *)curr -> data);
    }
}

Node *createNode(void *data) {

    Node *new_node = (Node*)malloc(sizeof(Node));
    
    new_node -> data = data;
    new_node -> prev = NULL;
    new_node -> next = NULL;
    return new_node; 
   
}


void appendToCache(Cache *cache, void *data) {
    
    Node *new_node = createNode(data); 

    if(cache -> size == 0){
        cache -> head = new_node;
        cache -> tail = new_node;
    }
    else{
        new_node -> next = cache -> head;
        cache -> head -> prev = new_node;
        cache -> head = new_node;
    }
    (cache -> size)++;
    if(cache -> size > cache -> capacity) removeLast(cache);
    
}

void moveToFront(Cache *cache, Node *node) {

    if (cache -> head == node) return;
    if (cache -> tail == node){
        cache ->tail = node -> prev;
        cache -> tail -> next = NULL;
    }
    else{
    node -> prev -> next = node -> next;
    node -> next -> prev = node -> prev;
    }
    node -> next = cache -> head;
    node -> prev = NULL;
    cache -> head -> prev = node;
    cache ->  head = node;

}

void removeLast(Cache *cache) { 

    Node *last_node = cache -> tail;
    
    if(cache -> size == 1){
        cache -> head = NULL;
        cache -> tail = NULL;
    }
    else{
        cache -> tail = last_node -> prev;
        cache ->tail -> next =NULL;
    }
    (cache -> size)--;
    free(last_node);

}
User *fetchUser(int userId) {

    Node *curr = userCache.head;
    User *user;
    int i, user_count = 0;

    for(;curr != NULL; curr = curr -> next ){
        user = (User *)curr -> data;
        if(user -> userId == userId){
            moveToFront(&userCache, curr);
            return user;
        }
        
    }
    user_count = sizeof(db.users) / sizeof(User); 

    for(i = 0; i < user_count ;i++){
        if(db.users[i].userId == userId){
            appendToCache(&userCache, &db.users[i]);
            return &db.users[i];
        }
    }
    return NULL;

}

Post *fetchPost(int postId) {

    Node *curr = postCache.head;
    Post *post;
    int i, post_count = 0;
    for(;curr != NULL; curr = curr -> next ){
        post = (Post *)curr -> data;
        if(post -> postId == postId){
            moveToFront(&postCache, curr);
            return post;
        }
        
    }
    post_count = sizeof(db.posts) / sizeof(Post); 
    for(i = 0; i < post_count ; i++){
        if(db.posts[i].postId == postId){
            appendToCache(&postCache, &db.posts[i]);
            return &db.posts[i];
        }
    }
    return NULL;

}

void followUser(int followerId, int followedId) {

    User *follower = fetchUser(followerId);
    User *followed = fetchUser(followedId);
    Node *new_node = createNode(follower);
    

    if(followed -> followers_tail == NULL){
        followed -> followers_head = new_node;
        followed -> followers_tail = new_node;
    }
    else{
        followed -> followers_tail -> next = new_node;
        new_node -> prev = followed -> followers_tail;
        followed -> followers_tail = new_node;
    }

    (followed -> numOfFollowers)++;
    (follower -> numOfFollowing)++;


}

void likePost(int userId, int postId) {

    Post *post = (Post *)fetchPost(postId);
    User *user = (User *)fetchUser(userId);

    (post -> likes)++;
    (user -> liked)++;


}

void printFollowersInReverse() {

    Node *curr = userCache.tail;
    User *user;
    Node *follower_node;
    User *follower;
    printf("Followers of all users in UserCache in reverse:\n");
    for(;curr != NULL; curr = curr -> prev){
        user = curr -> data;
        printf("Followers of user with id %d in reverse:\n", user -> userId);
        follower_node = user -> followers_head;
        if(!follower_node) {
            continue;
        }
        for(;follower_node -> next != NULL; follower_node = follower_node -> next);
        for(;follower_node != NULL;follower_node = follower_node -> prev){
            follower = follower_node -> data;
            printf("[%d] ", follower -> userId);
        }
        printf("\n");
      }

}

void freeLinkedList(Node *head) {

    Node *curr = head;
    Node *temp_node;
    while(curr != NULL){
        temp_node = curr -> next;
        free(curr);
        curr = temp_node;
    }

}
