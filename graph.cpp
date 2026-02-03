#include "raylib.h"
#include <math.h> // For sinf, cosf

#define NUM_NODES 30
#define NODE_RADIUS 55
#define LAYOUT_RADIUS NODE_RADIUS*(NUM_NODES / 2) // Distance from center to nodes

int main(void) {
    // Initialization
    const int screenWidth  = 2000;
    const int screenHeight = 2000;

    InitWindow(screenWidth, screenHeight, "Raylib - Graph from Adjacency Matrix");

    // 1. Define the Adjacency Matrix
    // 1 = Connected, 0 = Not connected
    // This represents a symmetric (undirected) graph
    int adjMatrix[NUM_NODES][NUM_NODES];
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = 0; j < NUM_NODES; j++) {
            if (j % 2 == 0) {
                adjMatrix[i][j] = 1;
            }
        }
    }
    // 2. Calculate Node Positions (Circular Layout)
    Vector2 nodePositions[NUM_NODES];
    Vector2 center = {(float) screenWidth / 2, (float) screenHeight / 2};

    for (int i = 0; i < NUM_NODES; i++) {
        // Calculate angle for this node: (2*PI * index) / total_nodes
        float angle = (2 * PI * i) / NUM_NODES;

        // Convert Polar coordinates to Cartesian (x, y)
        nodePositions[i].x = center.x + cosf(angle) * LAYOUT_RADIUS;
        nodePositions[i].y = center.y + sinf(angle) * LAYOUT_RADIUS;
    }

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // 3. Draw Edges
        // We iterate through the matrix to find connections.
        // We start j at (i + 1) to avoid drawing the same line twice for undirected graphs.
        for (int i = 0; i < NUM_NODES; i++) {
            for (int j = i + 1; j < NUM_NODES; j++) {
                if (adjMatrix[i][j] == 1) {
                    DrawLineEx(nodePositions[i], nodePositions[j], 3.0f, DARKGRAY);
                }
            }
        }

        // 4. Draw Nodes and Labels
        for (int i = 0; i < NUM_NODES; i++) {
            // Draw the node circle
            DrawCircleV(nodePositions[i], NODE_RADIUS, SKYBLUE);
            DrawCircleLines(nodePositions[i].x, nodePositions[i].y, NODE_RADIUS, DARKBLUE);

            // Draw the node index/ID
            // Measure text to center it perfectly inside the circle
            const char* text      = TextFormat("%d", i);
            int         textWidth = MeasureText(text, 20);
            DrawText(text, (int) nodePositions[i].x - textWidth / 2, (int) nodePositions[i].y - 10, 20, DARKBLUE);
        }

        DrawText("Adjacency Matrix Visualization", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
