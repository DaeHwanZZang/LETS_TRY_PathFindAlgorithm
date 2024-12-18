#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;

// 노드를 표현하는 구조체
struct Node {
    int x, y;
    double g, h, f;
    Node* parent;
    Node(int x, int y, double g = 0, double h = 0, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

// 비교 연산자 정의 (우선순위 큐에서 f 값을 기준으로 정렬)
struct CompareNode {
    bool operator()(Node* a, Node* b) { return a->f > b->f; }
};

// 맨해튼 거리 휴리스틱 함수
double heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// A* 알고리즘 함수
vector<pair<int, int>> aStar(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
    int rows = grid.size(), cols = grid[0].size();
    priority_queue<Node*, vector<Node*>, CompareNode> openList;
    unordered_map<int, Node*> closedList;

    // 시작 노드 추가
    Node* startNode = new Node(start.first, start.second, 0, heuristic(start.first, start.second, goal.first, goal.second));
    openList.push(startNode);

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        // 목표 노드 도달 시 경로 추적
        if (current->x == goal.first && current->y == goal.second) {
            vector<pair<int, int>> path;
            while (current) {
                path.push_back({current->x, current->y});
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 닫힌 리스트에 현재 노드 추가
        closedList[current->x * cols + current->y] = current;

        // 방향 이동 (상, 하, 좌, 우)
        vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (auto& dir : directions) {
            int newX = current->x + dir.first;
            int newY = current->y + dir.second;

            // 격자 범위 확인 및 장애물 검사
            if (newX < 0 || newY < 0 || newX >= rows || newY >= cols || grid[newX][newY] == 1)
                continue;

            // 이미 닫힌 리스트에 있으면 무시
            if (closedList.count(newX * cols + newY))
                continue;

            // 새로운 g, h, f 값 계산
            double g = current->g + 1;
            double h = heuristic(newX, newY, goal.first, goal.second);
            double f = g + h;

            // 노드 생성 및 추가
            Node* neighbor = new Node(newX, newY, g, h, current);
            openList.push(neighbor);
        }
    }

    // 경로를 찾지 못한 경우 빈 경로 반환
    return {};
}

void printGridWithPath(vector<vector<int>>& grid, vector<pair<int, int>>& path) {
    vector<vector<char>> displayGrid(grid.size(), vector<char>(grid[0].size(), ' '));

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (grid[i][j] == 1) {
                displayGrid[i][j] = 'of';  // 장애물
                