#include "DijkstraSolver.hpp"
#include <queue>
#include <limits>
#include <algorithm>

DijkstraSolver::DijkstraSolver(Graph* graph, PriorityQueuePanel& queuePanel, sf::RenderWindow& window)
    : graph(graph), queuePanel(queuePanel), window(window), currentStep(0), complete(false), animating(false), highlightNode(-1), highlightFrom(-1) {
}


void DijkstraSolver::prepareSteps(int startId, int endId) {
    distances.clear();
    previous.clear();
    visited.clear();
    visitOrder.clear();
    currentStep = 0;
    complete = false;
    animating = false;
    highlightNode = -1;
    highlightFrom = -1;
    queuePanel.clear();
    graph->resetColors();
    graph->resetNodeLabels();
    animationEvents.clear();
    finalPath.clear();

    for (Node* node : graph->getNodes()) {
        distances[node->id] = std::numeric_limits<float>::infinity();
        visited[node->id] = false;
        previous[node->id] = -1;
    }

    distances[startId] = 0;

    std::map<int, float> localDistances = distances;
    auto cmp = [&](int a, int b) { return localDistances[a] > localDistances[b]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    auto snapshotQueue = [&](const std::priority_queue<int, std::vector<int>, decltype(cmp)>& pqRef) {
        std::vector<int> tempVec;
        auto tempPq = pqRef;
        while (!tempPq.empty()) {
            tempVec.push_back(tempPq.top());
            tempPq.pop();
        }
        std::sort(tempVec.begin(), tempVec.end(), [&](int a, int b) {
            return localDistances[a] < localDistances[b];
            });
        std::vector<std::pair<int, float>> currentQueue;
        for (int id : tempVec) {
            currentQueue.emplace_back(id, localDistances[id]);
        }
        return currentQueue;
        };

    pq.push(startId);
    animationEvents.push_back({ AnimationEvent::Push, startId, 0.0f, snapshotQueue(pq) });

    while (!pq.empty()) {
        int current = pq.top();
        pq.pop();

        Node* node = graph->getNodeById(current);
        sf::Color prevNodeColor = node ? node->shape.getFillColor() : sf::Color::White;
        std::string prevNodeLabel = node ? node->label.getString() : "";
        int prevFromId = previous[current];
        sf::Color prevEdgeColor0 = sf::Color::Black, prevEdgeColor1 = sf::Color::Black;
        if (prevFromId != -1) {
            for (Edge* edge : graph->getEdges()) {
                if ((edge->from->id == prevFromId && edge->to->id == current) ||
                    (edge->from->id == current && edge->to->id == prevFromId)) {
                    prevEdgeColor0 = edge->line[0].color;
                    prevEdgeColor1 = edge->line[1].color;
                    break;
                }
            }
        }
        animationEvents.push_back({ AnimationEvent::Pop, current, localDistances[current], snapshotQueue(pq), prevNodeColor, prevNodeLabel, prevFromId, prevEdgeColor0, prevEdgeColor1 });

        if (visited[current]) continue;
        visited[current] = true;

        for (Edge* edge : graph->getEdges()) {
            int neighbor = -1;
            if (edge->from->id == current) neighbor = edge->to->id;
            else if (edge->to->id == current) neighbor = edge->from->id;
            else continue;

            float newDist = localDistances[current] + edge->weight;
            if (newDist < localDistances[neighbor]) {
                localDistances[neighbor] = newDist;
                previous[neighbor] = current;
                pq.push(neighbor);
                animationEvents.push_back({ AnimationEvent::Push, neighbor, newDist, snapshotQueue(pq) });
            }
        }
    }

    distances = localDistances;
    visited.clear(); // We'll step through visually
    currentStep = 0;
}


void DijkstraSolver::stepForward() {
    if (currentStep >= animationEvents.size()) {
        complete = true;
        animating = false;
        highlightNode = -1;
        highlightFrom = -1;
        return;
    }
    const auto& event = animationEvents[currentStep];
    queuePanel.setElements(event.queueSnapshot);

    // Only animate pop events (not push)
    if (event.type == AnimationEvent::Pop) {
        Node* current = graph->getNodeById(event.nodeId);
        if (current && std::find(finalPath.begin(), finalPath.end(), event.nodeId) == finalPath.end()) {
            current->shape.setFillColor(sf::Color(255, 204, 0));
            current->setLabel("(" + std::to_string(current->id) + ", " + std::to_string(static_cast<int>(distances[event.nodeId])) + ")");
        }
        highlightNode = event.nodeId;
        highlightFrom = previous[event.nodeId];

        if (previous[event.nodeId] != -1 && std::find(finalPath.begin(), finalPath.end(), event.nodeId) == finalPath.end()) {
            int fromId = previous[event.nodeId];
            for (Edge* edge : graph->getEdges()) {
                if ((edge->from->id == fromId && edge->to->id == event.nodeId) ||
                    (edge->from->id == event.nodeId && edge->to->id == fromId)) {
                    edge->line[0].color = sf::Color(255, 204, 0);
                    edge->line[1].color = sf::Color(255, 204, 0);
                }
            }
        }
    }
    currentStep++;
}








void DijkstraSolver::stepBackward() {

    if (finalPathShown) {      // Reset all node and edge colors to default
        graph->resetColors();
        graph->resetNodeLabels();
        finalPathShown = false;
    }


    ////////////////////////////
    if (currentStep == 0) return;

    currentStep--;

    if (currentStep < animationEvents.size())
        queuePanel.setElements(animationEvents[currentStep].queueSnapshot);
    else
        queuePanel.clear();

    const auto& event = animationEvents[currentStep];
    if (event.type == AnimationEvent::Pop) {
        Node* current = graph->getNodeById(event.nodeId);
        if (current && std::find(finalPath.begin(), finalPath.end(), event.nodeId) == finalPath.end()) {
            current->resetColor();
            current->setLabel(std::to_string(current->id));
        }

        if (previous[event.nodeId] != -1 && std::find(finalPath.begin(), finalPath.end(), event.nodeId) == finalPath.end()) {
            int fromId = previous[event.nodeId];
            for (Edge* edge : graph->getEdges()) {
                if ((edge->from->id == fromId && edge->to->id == event.nodeId) ||
                    (edge->from->id == event.nodeId && edge->to->id == fromId)) {
                    edge->line[0].color = sf::Color::Black;
                    edge->line[1].color = sf::Color::Black;
                }
            }
        }
    }
    complete = false;
}

std::vector<int> DijkstraSolver::run(int startId, int endId) {
    prepareSteps(startId, endId);
    while (!isComplete()) {
        stepForward();
    }

    std::vector<int> path;
    for (int at = endId; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}




void DijkstraSolver::colorPath(const std::vector<int>& path) {
    finalPath = path;
    finalPathShown = true;
    for (size_t i = 1; i < path.size(); ++i) {
        int from = path[i - 1];
        int to = path[i];
        for (Edge* edge : graph->getEdges()) {
            if ((edge->from->id == from && edge->to->id == to) ||
                (edge->from->id == to && edge->to->id == from)) {
                edge->line[0].color = sf::Color::Green;
                edge->line[1].color = sf::Color::Green;
            }
        }
        Node* node = graph->getNodeById(path[i]);
        if (node) {
            node->shape.setFillColor(sf::Color::Green);
            node->setLabel("(" + std::to_string(node->id) + ", " + std::to_string(static_cast<int>(distances[node->id])) + ")");
        }
    }
}

std::vector<int> DijkstraSolver::extractFinalPath(int startId, int endId) {
    std::vector<int> path;
    int current = endId;
    while (current != -1 && previous.count(current)) {
        path.push_back(current);
        current = previous[current];
    }

    std::reverse(path.begin(), path.end());

    if (!path.empty() && path.front() == startId)
        return path;

    return {}; // No valid path
}

bool DijkstraSolver::isComplete() const {
    return complete;
}

void DijkstraSolver::reset() {
    distances.clear();
    previous.clear();
    visited.clear();
    visitOrder.clear();
    currentStep = 0;
    complete = false;
    animating = false;
    highlightNode = -1;
    highlightFrom = -1;
    queuePanel.clear();
    //queueStates.clear();
}

void DijkstraSolver::startAnimation() {
    animating = true;
    animationClock.restart();
}
