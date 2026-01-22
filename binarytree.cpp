#include "raylib.h"
#include <stdio.h>  // For sprintf
#include <stdlib.h> // For malloc/free

// 1. Define the Tree Node Structure
struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
    Vector2   position; // We will calculate and store the visual position here
};

// Helper to create a new node
TreeNode* CreateNode(int value) {
    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    newNode->value    = value;
    newNode->left     = NULL;
    newNode->right    = NULL;
    newNode->position = {0, 0};
    return newNode;
}

// 2. Recursive Layout Algorithm
// This calculates the X, Y coordinates for every node based on depth and screen width.
// x, y: The position of the current node
// hOffset: How far apart the children should be (reduces as we go deeper)
void CalculateNodePositions(TreeNode* root, float x, float y, float hOffset) {
    if (root == NULL)
        return;

    root->position.x = x;
    root->position.y = y;

    float verticalSpacing = 80.0f; // Distance between levels

    // Recursively position left and right children
    // We reduce the horizontal offset by half (0.5) for the next level to prevent overlap
    if (root->left != NULL) {
        CalculateNodePositions(root->left, x - hOffset, y + verticalSpacing, hOffset * 0.5f);
    }
    if (root->right != NULL) {
        CalculateNodePositions(root->right, x + hOffset, y + verticalSpacing, hOffset * 0.5f);
    }
}

// 3. Recursive Drawing Function
void DrawTree(TreeNode* root) {
    if (root == NULL)
        return;

    // A. Draw Connections (Lines) first so they appear behind nodes
    if (root->left != NULL) {
        DrawLineEx(root->position, root->left->position, 2.0f, DARKGRAY);
        DrawTree(root->left); // Recurse
    }
    if (root->right != NULL) {
        DrawLineEx(root->position, root->right->position, 2.0f, DARKGRAY);
        DrawTree(root->right); // Recurse
    }

    // B. Draw the Node itself
    DrawCircleV(root->position, 20, MAROON);
    DrawCircleLines(root->position.x, root->position.y, 20, RAYWHITE);

    // C. Draw Value
    char text[10];
    sprintf(text, "%d", root->value);
    int textWidth = MeasureText(text, 20);
    DrawText(text, (int) root->position.x - textWidth / 2, (int) root->position.y - 10, 20, RAYWHITE);
}

// Helper to free memory
void FreeTree(TreeNode* root) {
    if (root == NULL)
        return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int main(void) {
    const int screenWidth  = 2400;
    const int screenHeight = 2000;

    InitWindow(screenWidth, screenHeight, "Raylib - Binary Tree Visualization");

    // 4. Manually Build a Sample Tree
    TreeNode* root = CreateNode(50);

    // Level 1
    root->left  = CreateNode(30);
    root->right = CreateNode(70);

    // Level 2
    root->left->left   = CreateNode(20);
    root->left->right  = CreateNode(40);
    root->right->left  = CreateNode(60);
    root->right->right = CreateNode(80);

    // Level 3 (Just adding a few to show depth)
    root->left->left->left    = CreateNode(10);
    root->right->right->right = CreateNode(90);

    // Calculate positions once before the loop
    // Start at center-top (width/2, 50px down)
    // Initial spread is 200px
    CalculateNodePositions(root, screenWidth / 2.0f, 50.0f, 200.0f);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Binary Tree Visualization", 10, 10, 20, DARKGRAY);

        // Start the recursive drawing from the root
        DrawTree(root);

        EndDrawing();
    }

    // Cleanup
    FreeTree(root);
    CloseWindow();

    return 0;
}
