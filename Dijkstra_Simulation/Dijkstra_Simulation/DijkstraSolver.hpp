#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Graph.hpp"
#include "PriorityQueuePanel.hpp"

class DijkstraSolver {
public:
    DijkstraSolver(Graph* graph, PriorityQueuePanel& queuePanel, sf::RenderWindow& window);

    void prepareSteps(int startId, int endId);
    void stepForward();
    void stepBackward();
    std::vector<int> run(int startId, int endId);
    void colorPath(const std::vector<int>& path);
    std::vector<int> extractFinalPath(int startId, int endId);
    bool isComplete() const;
    void reset();

    // Animation support
    void startAnimation();
    bool animating;
    sf::Clock animationClock;
    int highlightNode;
    int highlightFrom;

private:
    Graph* graph;
    PriorityQueuePanel& queuePanel;
    sf::RenderWindow& window;

    std::map<int, float> distances;
    std::map<int, int> previous;
    std::map<int, bool> visited;
    std::vector<int> visitOrder;
    int currentStep;
    bool complete;



    //changes made
    struct AnimationEvent {
        enum Type { Pop, Push } type;
        int nodeId;
        float distance; // For push
        std::vector<std::pair<int, float>> queueSnapshot;


        // For backtrace:
        sf::Color prevNodeColor;
        std::string prevNodeLabel;
        int prevFromId;
        sf::Color prevEdgeColor0, prevEdgeColor1;
    };
    std::vector<AnimationEvent> animationEvents;
    std::vector<int> finalPath; // To store the final path for green coloring
    bool finalPathShown = false;

};