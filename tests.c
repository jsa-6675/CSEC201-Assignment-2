/**
* File: test.c 
* Desctiption: this program tests core functionanlity of user.c and program
* Author: Jodley A
*/

#include <string.h>
#include <stdio.h>
#include"user.h"
#include"hash.h"
#include "tests.h"


void test1() {
    char* expected[] = { "user3", "user2", "user1" };

    struct User* head = NULL;

    head = add(head, "user1");
    head = add(head, "user2");
    head = add(head, "user3");

    struct User* current = head;
    int i = 0;
    int passed = 1;

    while (current != NULL && i < 3) {
        if (strcmp(current->Username, expected[i]) != 0) {
            printf("TEST1 FAILED: expected %s, got %s\n",
                expected[i], current->Username);
            passed = 0;
        }

        current = current->next;
        i++;
    }

    if (i != 3 || current != NULL) {
        printf("TEST1 FAILED: expected exactly three blocks\n");
        passed = 0;
    }

    if (passed == 1) {
        printf("TEST1 PASSED\n");
    }

    while (head != NULL) {
        struct User* next = head->next;
        free(head);
        head = next;
    }
}

void test2() {
	char* names[] = { "user1", "user2", "user3" };
	time_t before[3];
	time_t after[3];
	struct User* head = NULL;
	int passed = 1;

	for (int i = 0; i < 3; i++) {
		before[i] = time(NULL);
		head = add(head, names[i]);
		after[i] = time(NULL);
	}

	struct User* current = head;
	int i = 2; 
	while (current != NULL && i >= 0) {
		if ( current->loginTime < before[i] || current->loginTime > after[i]) {
			printf("TEST2 FAILED: login time is outside the range for %s\n", names[i]);
			passed = 0;
		}
		current = current->next;
		i--;
	}

	if (i != -1 || current != NULL) {
		printf("TEST2 FAILED: expected exactly three blocks\n");
		passed = 0;
	}
	if (passed == 1) {
		printf("TEST2 PASSED\n");
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test3() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");

	int before = 0;
	struct User* current = head;
	while (current != NULL) {
		before++;
		current = current->next;
	}

	head = add(head, "uniqueUser");
	int after = 0;
	int found = 0;
	current = head;
	while (current != NULL) {
		after++;
		if (strcmp(current->Username, "uniqueUser") == 0) {
			found++;
		}
		current = current->next;
	}

	if (before == 2 && after == before + 1 && found == 1) {
		printf("TEST3 PASSED\n");
	}
	else {
		printf("TEST3 FAILED: before=%d, after=%d, new user matches=%d\n",
			before, after, found);
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test4() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");
	head = add(head, "user3");

	struct User* current = head;
	int passed = 1;
	int count = 0;
	while (current != NULL && count < 3) {
		if (current->next != NULL) {
			struct Digest expected;
			generateDigest(&expected, current->next);
			if (digest_equal(current->hash, expected) == 0) {
				printf("TEST4 FAILED: wrong stored hash at block %d\n", count + 1);
				passed = 0;
			}
		}
		count++;
		current = current->next;
	}

	if (count != 3 || current != NULL) {
		printf("TEST4 FAILED: expected exactly three blocks\n");
		passed = 0;
	}
	if (passed == 1) {
		printf("TEST4 PASSED\n");
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test5() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");
	head = add(head, "user3");
	printf("TEST5 - MANUAL CHECK\n");
	verify(head);
	printf("TEST5 passes only if the actual output matches the expected result.\n");

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test6() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");
	head = add(head, "user3");

	printf("TEST6 - MANUAL CHECK\n");
	printf("Before change: expected the entire chain to be valid.\n");
	verify(head);

	if (head != NULL && head->next != NULL) {
		strcpy(head->next->Username, "changedUser");
		printf("After username change: expected an integrity failure.\n");
		verify(head);
		printf("TEST6 passes only if the first check was valid and the second failed.\n");
	}
	else {
		printf("TEST6 FAILED: missing block to change\n");
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test7() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");
	head = add(head, "user3");

	printf("TEST7 - MANUAL CHECK\n");
	printf("Before change: expected the entire chain to be valid.\n");
	verify(head);

	if (head != NULL && head->next != NULL) {
		head->next->loginTime += 60;
		printf("After timestamp change: expected an integrity failure.\n");
		verify(head);
		printf("TEST7 passes only if the first check was valid and the second failed.\n");
	}
	else {
		printf("TEST7 FAILED: missing block to change\n");
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}

void test8() {
	struct User* head = NULL;
	head = add(head, "user1");
	head = add(head, "user2");
	head = add(head, "user3");

	printf("TEST8 - MANUAL CHECK\n");
	printf("Before change: expected the entire chain to be valid.\n");
	verify(head);

	if (head != NULL && head->next != NULL) {
		// Change one byte of the middle block's stored hash.
		if (head->next->hash.hash0 == 0) {
			head->next->hash.hash0 = 1;
		}
		else {
			head->next->hash.hash0 = 0;
		}
		printf("After hash change: expected an integrity failure.\n");
		verify(head);
		printf("TEST8 passes only if the first check was valid and the second failed.\n");
	}
	else {
		printf("TEST8 FAILED: missing block to change\n");
	}

	while (head != NULL) {
		struct User* next = head->next;
		free(head);
		head = next;
	}
}