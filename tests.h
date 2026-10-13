#pragma once
#include <stdlib.h>
#include "user.h"
#include "hash.h"

#ifndef TESTS_H
#define TESTS_H

// test if each block should store the expected usernam
void test1();

// test if each block stores the user’s login timestamp
void test2();

// test if the program accuratly addes a new block to block chain
void test3();

// test if each block except the initial block should store the hash of the preceding block
void test4();

// test if the program validates the intergrirt of a chain
void test5();

// test if the program detects modification of stored usernames 
void test6();

// test if the program detects modification of stored login timestamps
void test7();

// test if the program detects modiftication of stored hashed data in each blovk
void test8();

#endif