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
    // TODO
  }

  void init() {
    // TODO
  }

  void buildTree() {
    // TODO
  }

  void getRepr(string &buffer) {
    // TODO
  }

  void loadRepr(string &buffer) {
    // TODO
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
