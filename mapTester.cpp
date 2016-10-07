#include <cstdio>
#include "Map.hpp"

using namespace cs540;

int main() {
  Map<int, char> m;

  m.insert({4, 'a'});
  m.insert({16, 'b'});
  m.insert({256, 'c'});

  auto it = m.find(4);
  
  printf ("%d\n", (*it).first);
  it = m.find(16);
  printf ("%d\n", (*it).first);
  it = m.find(256);
  printf ("%d\n", (*it).first);
  it = m.find(1111);

  if (it == m.end()) printf("good news!\n");
  else printf ("terrible news!\n");

  m.print();

  printf("size: %lu\n", m.size());
  if (m.empty()) printf ("m is empty\n");
  else printf ("m is not empty\n");

  m.clear();
  m.clear();

  printf("size: %lu\n", m.size());
  if (m.empty()) printf ("m is empty\n");
  else printf ("m is not empty\n");
}
