#include <bits/stdc++.h>

using namespace std;

class SuffixTree {
public:
  struct Edge;
  struct Node;
  struct Suffix;

  struct Edge {
    int l, r, startNode, endNode;
    Edge(int ll, int rr, int sn, int en): l(ll), r(rr), startNode(sn), endNode(en) {}
  }

  struct Node {
    int id, suffixLink;
    Node(int i, int sf): id(i), suffixLink(sf) {}
  }

  struct Suffix {
    int node, suffixPos, l, r;
    Suffix(int n, int ll, int rr): node(n), l(ll), r(rr) {}
  }

  Node root;
  Node *nodes;
  Edge *edges;
  list<char> *outEdges;
  map< pair<int, char> , int > edgesMap;
  string str;
  int count;
  int stringLen;

  void addNode(Node *node) {
    nodes[node.id] = node;
  }

  void addEdge(Edge *edge) {
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
    // TODO
  }

  Suffix update(Suffix sf, int p) {
    // TODO
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

  vector<int> match(string &pat) {
    // TODO
  }

}
