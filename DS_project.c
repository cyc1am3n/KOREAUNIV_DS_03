#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DLinkedList.h"

typedef struct _ual {
    int numV;
    int numE;
    List * adjList;
}ALGraph;

//list함수
void ListInit(List * plist)
{
	plist->head = (Node*)malloc(sizeof(Node));
	plist->head->next = NULL;
	plist->comp = NULL;
	plist->numOfData = 0;
}

void FInsert(List * plist, LData data)
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;

	newNode->next = plist->head->next;
	plist->head->next = newNode;

	(plist->numOfData)++;
}

void SInsert(List * plist, LData data)
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	Node * pred = plist->head;
	newNode->data = data;

	while(pred->next != NULL && plist->comp(data, pred->next->data) != 0)
	{
		pred = pred->next;
	}

	newNode->next = pred->next;
	pred->next = newNode;

	(plist->numOfData)++;
}

void LInsert(List * plist, LData data)
{
	if(plist->comp == NULL)
		FInsert(plist, data);
	else
		SInsert(plist, data);
}

int LFirst(List * plist, LData * pdata)
{
	if(plist->head->next == NULL)
		return FALSE;

	plist->before = plist->head;
	plist->cur = plist->head->next;

	*pdata = plist->cur->data;
	return TRUE;
}

int LNext(List * plist, LData * pdata)
{
	if(plist->cur->next == NULL)
		return FALSE;

	plist->before = plist->cur;
	plist->cur = plist->cur->next;

	*pdata = plist->cur->data;
	return TRUE;
}

LData LRemove(List * plist)
{
	Node * rpos = plist->cur;
	LData rdata = rpos->data;

	plist->before->next = plist->cur->next;
	plist->cur = plist->before;

	free(rpos);
	(plist->numOfData)--;
	return rdata;
}

int LCount(List * plist)
{
	return plist->numOfData;
}

void SetSortRule(List * plist, int (*comp)(LData d1, LData d2))
{
	plist->comp = comp;
}
//Graph 함수
void GraphInit(ALGraph * pg, int nv) {
    int i;

    pg->adjList = (List*)malloc(sizeof(List)*nv);
    pg->numV = nv;
    pg->numE = 0;

    for(i = 0; i < nv; i++) {
        ListInit(&(pg->adjList[i]));
    }
}

void GraphDestroy(ALGraph * pg) {
    if(pg->adjList != NULL)
        free(pg->adjList);
}

void AddEdge(ALGraph * pg, int fromV, int toV) {
    LInsert(&(pg->adjList[fromV]), toV);
    LInsert(&(pg->adjList[toV]), fromV);
    pg->numE += 1;
}

void ShowGraphEdgeInfo(ALGraph * pg) {
    int i;
    int vx;

    for(i = 0; i < pg->numV; i++) {
        printf("%d와 연결된 정점: ",i);

        if(LFirst(&(pg->adjList[i]), &vx)) {
            printf("%d ", vx);

            while(LNext(&(pg->adjList[i]), &vx))
                printf("%d ", vx);
        }
        printf("\n");
    }
}

//

typedef struct user_profile{
    int id_num;
    char date[6][9];
    char sc_name[21];
    int numF;
    int numT;
}User;

typedef struct word_tweet {
    User tweet_user;
    char tweet_date[6][9];
    char tweet_word[20];
    char us_name[21];
    int us_id;
}Word;

int main()
{

    User twu[1000];
	int user_num = 0;
	FILE *user_f;
	user_f = fopen("user.txt","rt");

	while(!feof(user_f)) {
        fscanf(user_f,"%d",&twu[user_num].id_num);
        fscanf(user_f,"%s %s %s %s %s %s",twu[user_num].date[0],twu[user_num].date[1],twu[user_num].date[2],twu[user_num].date[3],twu[user_num].date[4],twu[user_num].date[5]);
        fscanf(user_f,"%s",twu[user_num].sc_name);
        user_num++;
	}

	fclose(user_f);

    ALGraph graph;
    GraphInit(&graph, user_num - 1);

    FILE *friend_f;
    friend_f = fopen("friend.txt","rt");
    int tmp1,tmp2,fr_num=0;

    while(!feof(friend_f)) {
        int sc1 = 0, sc2 = 0;
        fscanf(friend_f,"%d\n%d",&tmp1,&tmp2);
        while(1) {
            if(twu[sc1].id_num == tmp1) {
                twu[sc1].numF++;
                break;
            }
            else sc1++;
        }

        while(1) {
            if(twu[sc2].id_num == tmp2) {
                    twu[sc2].numF++;
                    break;
            }
            else sc2++;
        }
        AddEdge(&graph, sc1, sc2);
        fr_num++;
    }

    fclose(friend_f);

    Word tww[10000];
    int tw_num = 0, tmp3;
    FILE *word_f;
    word_f = fopen("word.txt","rt");

    while(!feof(word_f)) {
        fscanf(word_f,"%d",&tmp3);
        fscanf(word_f,"%s %s %s %s %s %s",tww[tw_num].tweet_date[0],tww[tw_num].tweet_date[1],tww[tw_num].tweet_date[2],tww[tw_num].tweet_date[3],tww[tw_num].tweet_date[4],tww[tw_num].tweet_date[5]);
        fscanf(word_f,"%s",tww[tw_num].tweet_word);
        tw_num++;
        int sc = 0;
        tww[tw_num].us_id = tmp3;
        while(1) {
            if(twu[sc].id_num == tmp3) {
                twu[sc].numT++;
                strcpy(tww[tw_num].us_name, twu[sc].sc_name);
                break;
            }
            else
                sc++;
        }

    }

    fclose(word_f);

    char wordsc[20];

    int menu;
    while(1) {
        printf("<MENU>\n");
        printf("0. Read data files\n");
        printf("1. Display statistics\n");
        printf("2. Top 5 most tweeted words\n");
        printf("3. Top 5 most tweeted users\n");
        printf("4. Find users who tweeted a word (e.g., ’연세대’)\n");
        printf("5. Find all people who are friends of the above users\n");
        printf("6. Delete all mentions of a word\n");
        printf("7. Delete all users who mentioned a word\n");
        printf("8. Find strongly connected component\n");
        printf("9. Find shortest path from a given user\n");
        printf("99. Quit\n");
        printf("Select Menu: ");
        scanf("%d",&menu);

        if(menu == 0) {
            printf("------------------------------\n0. Read data files\n");
            printf("Total users: %d\n",user_num - 1);
            printf("Total friendship records: %d\n", fr_num);
            printf("Total tweet: %d\n------------------------------\n", tw_num);
        }
        else if(menu == 1) {
            int avF = 0, i;
            for(i = 0; i < user_num - 1; i++) {
                avF += twu[i].numF;
            }
            avF = avF/(user_num - 1);

            int minF = twu[0].numF;
            for(i = 1; i < user_num - 1; i++) {
                if(minF > twu[i].numF) minF = twu[i].numF;
            }

            int maxF = twu[0].numF;
            for(i = 1; i < user_num - 1; i++) {
                if(maxF < twu[i].numF) maxF = twu[i].numF;
            }

            int avT = 0;
            for(i = 0; i < user_num - 1; i++) {
                avT += twu[i].numT;
            }
            avT = avT/(user_num - 1);

            int minT = twu[0].numT;
            for(i = 1; i < user_num - 1; i++) {
                if(minT > twu[i].numT) minT = twu[i].numT;
            }

            int maxT = twu[0].numT;
            for(i = 1; i < user_num - 1; i++) {
                if(maxT < twu[i].numT) maxT = twu[i].numT;
            }

            printf("------------------------------\n1. Display statistics\n");
            printf("Average number of friends: %d\n", avF);
            printf("Minimum friends: %d\n", minF);
            printf("Maximum number of friends: %d\n", maxF);

            printf("Average tweets per user: %d\n", avT);
            printf("Minimum tweets per user: %d\n", minT);
            printf("Maximum tweets per use: %d\n", maxT);
            printf("------------------------------\n");

        }
        else if(menu == 2) {
            int i;
            for(i = 0; i < tw_num; i++) {

            }
            printf("------------------------------\n2. Top 5 most tweeted words\n");
            printf("Top1: \n");
            printf("Top2: \n");
            printf("Top3: \n");
            printf("Top4: \n");
            printf("Top5: \n");
            printf("------------------------------\n");


        }
        else if(menu == 3) {
            int i,top1 = 0, top2 = 0, top3 = 0, top4 = 0, top5 = 0;
            for(i = 1; i < user_num - 1; i++) {
                if(twu[top1].numT < twu[i].numT) top1 = i;
            }

            for(i = 1; i < user_num - 1; i++) {
                if(twu[top2].numT < twu[i].numT && i != top1) top2 = i;
            }

            for(i = 1; i < user_num - 1; i++) {
                if(twu[top3].numT < twu[i].numT && i != top1 && i != top2) top3 = i;
            }

            for(i = 1; i < user_num - 1; i++) {
                if(twu[top4].numT < twu[i].numT && i != top1 && i != top2 && i != top3) top4 = i;
            }

            for(i = 1; i < user_num - 1; i++) {
                if(twu[top5].numT < twu[i].numT && i != top1 && i != top2 && i != top3 && i != top4) top5 = i;
            }

            printf("------------------------------\n3. Top 5 most tweeted users\n");
            printf("Top1: %s\n", twu[top1].sc_name);
            printf("Top2: %s\n", twu[top2].sc_name);
            printf("Top3: %s\n", twu[top3].sc_name);
            printf("Top4: %s\n", twu[top4].sc_name);
            printf("Top5: %s\n", twu[top5].sc_name);
            printf("------------------------------\n");
        }
        else if(menu == 4) {
            int i;
            //char wordsc[20];

            printf("------------------------------\n4. Find users who tweeted a word (e.g., ’연세대’)\n");
            printf("Word: ");
            scanf("%s",wordsc);

            for(i = 0; i < tw_num; i++) {
                if(!strcmp(wordsc, tww[i].tweet_word)) printf("User Screen Name: %s\n",tww[i].us_name);
            }
            printf("Search End\n------------------------------\n");
        }
        else if(menu == 5) {
            int i;
            printf("------------------------------\n5. Find all people who are friends of the above users\n");
            ALGraph pg = graph;
           /* for(i = 0; i < tw_num; i++) {
                if(!strcmp(wordsc, tww[i].tweet_word)) {
                int j;
                    for(j = 0; j < user_num; j++) {
                        if(tww[i].us_id == twu[j].id_num) {

                            int vx = 0;
                            printf("%s's Friends:\n",twu[j].sc_name);

                            if(LFirst(&(pg->adjList[j]), &vx)) {
                                printf("%s\n", twu[vx].sc_name);

                                while(LNext(&(pg->adjList[j]), &vx))
                                    printf("%s\n", twu[vx].sc_name);
                            }
                            printf("\n");
                        }
                    }
                }
            }*/


        }
        else if(menu == 6) {

        }
        else if(menu == 7) {

        }
        else if(menu == 8) {

        }
        else if(menu == 9) {

        }
        else if(menu == 99) {
            printf("------------------------------\nQuit\n------------------------------\n");
            break;
        }
        else{
            printf("------------------------------\nWrong Number! Please select menu again!\n------------------------------\n");
        }
    }

    return 0;
}
