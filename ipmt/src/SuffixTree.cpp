#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <algorithm>

using namespace std;

class SuffixTree {
public:
  struct Edge;
  struct Node;
  struct Suffix;

  struct Edge {
    int l, r, startNode, endNode;
    Edge(int ll = 0, int rr = 0, int sn = -1, int en = -1): l(ll), r(rr), startNode(sn), endNode(en) {}
  };

  struct Node {
    int id, suffixLink;
    Node(int i = -1, int sf = -1): id(i), suffixLink(sf) {}
  };

  struct Suffix {
    int node, suffixPos, l, r;
    Suffix(int n = -1, int ll = -1, int rr = -1): node(n), l(ll), r(rr) {}
  };

  Node root;
  Node *nodes;
  Edge *edges;
  list<char> *outEdges;
  map< pair<int, char> , int > edgesMap;
  string str;
  int count;
  int stringLen;

  void addNode(Node &node) {
    nodes[node.id] = node;
  }

  void addEdge(Edge &edge) {
    edges[edge.endNode] = edge;
    edgesMap[make_pair(edge.startNode, str[edge.l])] = edge.endNode;
  }

  Suffix canonize(Suffix sf) {
    if (sf.l <= sf.r) {
      int edgeId = edgesMap[make_pair(sf.node, str[sf.l])];

      Edge edge = edges[edgeId];
      while (edge.r - edge.l <= sf.r - sf.l) {
        sf.node = edge.endNode;
        sf.l += (edge.r - edge.l) + 1;

        if (sf.l <= sf.r) {
          edgeId = edgesMap[make_pair(sf.node, str[sf.l])];
          edge = edges[edgeId];
        }
      }
    }
    return sf;
  }

  pair<bool, int> testAndSplit(Suffix sf, int p) {
    if(sf.l <= sf.r){
      int edgeIndex = edgesMap[make_pair(sf.node, str[sf.l])];
      Edge edge = edges[edgeIndex];

      if(str[edge.l + (sf.r - sf.l) + 1] == str[p]){
        return make_pair(true, edge.startNode);
      } else {
        Node newNode(count++, sf.node);
        addNode(newNode);

        edgesMap.erase(make_pair(edge.startNode, str[edge.l]));

        Edge newEdge;
        newEdge = Edge(edge.l, edge.l + (sf.r - sf.l), sf.node, newNode.id);
        addEdge(newEdge);

        nodes[newNode.id].suffixLink = sf.node;

        newEdge =  Edge(edge.l + (sf.r - sf.l ) + 1, edge.r, newNode.id, edge.endNode);
        addEdge(newEdge);

        return make_pair(false, newNode.id);
      }
    } else {
      if(edgesMap.count(make_pair(sf.node, str[p])) > 0){
        return make_pair(true, sf.node);
      } else {
        return make_pair(false, sf.node);
      }
    }
  }

  Suffix update(Suffix sf, int p) {
    pair<bool, int> ret = testAndSplit(sf, p);

    bool isTerm = ret.first;
    int word1 = ret.second, word2 = -1;

    while(!isTerm){
      Node node(count++, sf.node);
      addNode( node );

      Edge edge(p, stringLen, word1, node.id);
      addEdge(edge);

      if(word2 != -1){
        nodes[word2].suffixLink = word1;
      }
      word2 = word1;

      if(sf.node == root.id){
        sf.l++;
      } else {
        sf.node = nodes[sf.node].suffixLink;
      }

      sf = canonize(Suffix(sf.node, sf.l, sf.r));
      ret = testAndSplit(sf, p);
      isTerm = ret.first;
      word1 = ret.second;
    }

    if(word2 != root.id){
      nodes[word2].suffixLink = word1;
    }
    sf.r++;
    return sf;
  }

  void prepareOutEdges() {
    outEdges = new list<char>[count + 1];

    map< pair<int,char> , int >::iterator it;
    for(it = edgesMap.begin(); it != edgesMap.end(); it++){
      Edge edge = edges[it->second];
      outEdges[edge.startNode].push_back(str[edge.l]);
    }
  }

  void init() {
    count = 0;
    stringLen = str.size();

    nodes = new Node[2*stringLen + 10];
    edges = new Edge[2*stringLen + 10];

    edgesMap.clear();

    root = Node(count++, -1);
    addNode(root);
  }

  void buildTree() {
    init();

    Suffix sf(root.id, 0, -1);
    for (int i = 0; i < stringLen; i++) {
      sf = update(sf, i);
      sf = canonize(sf);
    }

    prepareOutEdges();
  }

  void getRepr(string &buffer) {
    buffer = "";

    ostringstream os;
    os << count << "\n";

    for (int id = 1; id < count; id++) {
      Edge e = edges[id];
      os << e.l << "\n";
      os << e.r << "\n";
      os << e.startNode << "\n";
      os << e.endNode << "\n";
    }
    buffer = os.str();
  }

  void loadRepr(string &buffer) {
    init();

    stringstream ss(buffer);
    ss >> count;

    outEdges = new list<char>[count + 1];
    for (int id = 1; id < count; id++) {
      Edge e;
      ss >> e.l;
      ss >> e.r;
      ss >> e.startNode;
      ss >> e.endNode;
      addEdge(e);
      outEdges[e.startNode].push_back(str[e.l]);
    }
  }

  SuffixTree(const string &s) {
    str = s;
    buildTree();
  }

  SuffixTree(const string &s, string &buffer) {
    str = s;
    loadRepr(buffer);
  }

  ~SuffixTree() {
    delete[] nodes;
    delete[] edges;
    delete[] outEdges;
  }

  void matchAux(int nodeId, int currentSuffixSize, vector<int> &occ) {
    if(outEdges[nodeId].size() == 0){
    	occ.push_back(stringLen - currentSuffixSize);
    }

    list<char> &currentNodeEdges = outEdges[nodeId];

    list<char>::iterator it;
    for(it = currentNodeEdges.begin(); it != currentNodeEdges.end(); it++){
    	Edge edge = edges[edgesMap[make_pair(nodeId, *it)]];
    	int span = edge.r - edge.l + 1;
    	if(edge.r == stringLen) span--;
    	matchAux(edge.endNode, currentSuffixSize + span, occ);
    }
  }

  vector<int> match(string &pattern) {
    vector<int> occ;

    Edge edge;

    int patternPos = 0;
    int currentSuffixSize = 0;
    int currentNode = root.id;

    bool searching = true;

    if(pattern.size() == 0) return occ;

    while(searching) {
    	bool found = true;

      if(edgesMap.count(make_pair(currentNode, pattern[patternPos])) > 0) {
    		edge = edges[edgesMap[make_pair(currentNode, pattern[patternPos])]];
    	} else {
    		break;
    	}

    	for(int textPos = edge.l ; textPos <= edge.r && textPos < stringLen && patternPos < (int) pattern.size(); ++textPos , ++patternPos) {
        if(pattern[patternPos] != str[textPos]) {
    			found = false;
    			break;
    		}
    	}

    	if(found) {
    		currentSuffixSize += edge.r - edge.l + 1;

    		if(edge.r == stringLen) currentSuffixSize--;

    		if(patternPos == (int) pattern.size()) {
    			matchAux(edge.endNode, currentSuffixSize, occ);
    			searching = false;
    		}

    		currentNode = edge.endNode;
    	} else {
    		searching = false;
    	}
    }

    sort(occ.begin(), occ.end());
    return occ;
  }

};
