/* DO NOT CHANGE */
/* Only for implementation ease, not going to be used in evaluation */
#include "socialmedia.h"

int main(void) {
  int userCount;
  int postCount;
  int actionCount;
  int i;
  /* Read cache capacities */
  scanf("%d %d", &userCache.capacity, &postCache.capacity);

  /* Read users */
  scanf("%d", &userCount);
  for (i = 0; i < userCount; i++) {
    int userId;
    char username[USERNAME_MAX_LEN];
    int day, month, year;
    scanf("%d %s %d %d %d", &userId, username, &day, &month, &year);

    db.users[userId].userId = userId;
    db.users[userId].username = malloc(strlen(username));
    memcpy(db.users[userId].username, username, strlen(username) + 1);
    db.users[userId].birthday = malloc(sizeof(Date));
    db.users[userId].birthday->day = day;
    db.users[userId].birthday->month = month;
    db.users[userId].birthday->year = year;
    db.users[userId].followers_head = db.users[userId].followers_tail = NULL;
    db.users[userId].numOfFollowers = 0;
    db.users[userId].numOfFollowing = 0;
    db.users[userId].liked = 0;
  }

  /* Read posts */
  scanf("%d", &postCount);
  for (i = 0; i < postCount; i++) {
    int postId, authorId, content_length, j;
    char content[POST_CONTENT_MAX_LEN];
    scanf("%d %d %d ", &postId, &authorId, &content_length);
    for (j = 0; j < content_length; j++) {
      content[j] = getc(stdin);
    }
    content[content_length] = 0;

    db.posts[postId].postId = postId;
    db.posts[postId].author = &db.users[authorId];
    db.posts[postId].content = malloc(strlen(content));
    memcpy(db.posts[postId].content, content, strlen(content) + 1);
    db.posts[postId].likes = 0;
  }

  /* Do the actions */
  scanf("%d", &actionCount);
  for (i = 0; i < actionCount; i++) {
    char actionType[2];
    int id1, id2;
    if (i != 0) printf("\n\n");
    printf("Processing action %d\n", i + 1);
    scanf("%01s %d %d", actionType, &id1, &id2);
    if (actionType[0] == 'F') {
      /* Follow */
      followUser(id1, id2);
      printUserCache(&userCache);
    } else if (actionType[0] == 'L') {
      /* Like */
      likePost(id1, id2);
      printUserCache(&userCache);
      printf("\n");
      printPostCache(&postCache);
    } else {
      fprintf(stderr, "Unexpected action type '%s'!\n", actionType);
      exit(-1);
    }
  }
  /* At the end, print followers of all users in the UserCache */
  printf("\n\n");
  printFollowersInReverse();
  /* Free all */
  freeLinkedList(userCache.head);
  freeLinkedList(postCache.head);
  for (i = 0; i < DB_USER_CAPACITY; i++) {
    if (db.users[i].birthday) {
      free(db.users[i].birthday);
      free(db.users[i].username);
      freeLinkedList(db.users[i].followers_head);
    }
  }
  for (i = 0; i < DB_POST_CAPACITY; i++) {
    if (db.posts[i].content) {
      free(db.posts[i].content);
    }
  }
  return 0;
}