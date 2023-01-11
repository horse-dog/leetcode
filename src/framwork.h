#pragma once

#include <stdio.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;
