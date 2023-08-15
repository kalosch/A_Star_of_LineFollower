//class Node {
//  public:
//    Node() {}
//    Node(Node* parent, int positionX, int positionY) {
//      this->parent = parent;
//      this->positionX = positionX;
//      this->positionY = positionY;
//      this->g = 0;
//      this->h = 0;
//      this->f = 0;
//    }
//
//    bool operator==(const Node& other) const {
//      return positionX == other.positionX && positionY == other.positionY;
//    }
//
//    Node* parent;
//    int positionX;
//    int positionY;
//    int g;
//    int h;
//    int f;
//};
//
//Node* openList[100];
//Node* closedList[100];


//int maze[10][10] = {
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//};

// 0 -> not wall
// 1 -> wall





void A_star (int _Start_x, int Start_Y, int _End_x, int _End_y) {
  Node* startNode = new Node(nullptr, _Start_x, Start_Y);
  startNode->g = startNode->h = startNode->f = 0;
  Node* endNode = new Node(nullptr, _End_x, _End_y);
  endNode->g = endNode->h = endNode->f = 0;

  openList[0] = startNode;
  openCount++;

  while (openCount > 0) {
    Node* currentNode = openList[0];
    int currentIndex = 0;
    for (int i = 0; i < openCount; i++) {
      if (openList[i]->f < currentNode->f) {
        currentNode = openList[i];
        currentIndex = i;
      }
    }

    openList[currentIndex] = openList[openCount - 1];
    openCount--;
    closedList[closedCount] = currentNode;
    closedCount++;

    if (currentNode->positionX == endNode->positionX && currentNode->positionY == endNode->positionY) {
      Node* currentPathNode = currentNode;
      while (currentPathNode != nullptr) {
        path[pathCount] = currentPathNode;
        currentPathNode = currentPathNode->parent;
        pathCount++;
      }
      break;
    }

    Node* children[4];
    int numChildren = 0;
    int newPositions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    for (int i = 0; i < 4; i++) {
      int newPositionX = currentNode->positionX + newPositions[i][0];
      int newPositionY = currentNode->positionY + newPositions[i][1];

      if (newPositionX >= 0 && newPositionX < 10 && newPositionY >= 0 && newPositionY < 10 && maze[newPositionX][newPositionY] == 0) {
        children[numChildren] = new Node(currentNode, newPositionX, newPositionY);
        numChildren++;
      }
    }

    for (int i = 0; i < numChildren; i++) {
      for (int j = 0; j < closedCount; j++) {
        if (*(children[i]) == *(closedList[j])) {
          continue;
        }
      }

      children[i]->g = currentNode->g + 1;
      children[i]->h = (children[i]->positionX - endNode->positionX) * (children[i]->positionX - endNode->positionX) + 
                       (children[i]->positionY - endNode->positionY) * (children[i]->positionY - endNode->positionY);
      children[i]->f = children[i]->g + children[i]->h;

      for (int j = 0; j < openCount; j++) {
        if (*(children[i]) == *(openList[j]) && children[i]->g > openList[j]->g) {
          continue;
        }
      }

      openList[openCount] = children[i];
      openCount++;
    }
  }
}
