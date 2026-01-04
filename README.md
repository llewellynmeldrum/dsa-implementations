# RSP - Implement data structures
From the Novice requirements:<br>
**Implement in either C++ / Java / Python:**
- [X] A Linked List.
- [X] A Binary Tree.
- [X] A Graph using an Adjacency List.
- [X] Depth First Search using recursion. (on the adj list graph)
- [X] Breadth First Search using a while loop. 

# usage
```bash
make run
# to build and run tests
```

```bash 
./bin/test -v
# to run in verbose mode.
# verbose shows each test cases expected and receieved,
# along with a small diff checker between them:
``` 

# Cool stuff
- Implemented optional leetcode testcase style inputs to the binary tree constructor (`BinaryTree<T>(const std::string& s);`)
- Made testing class to group results and pretty print stuff

# Dependencies 
- gnu make
- c++20 capable compiler (i like std::print)

# References
- ADM (The Algorithm Design Manual by Steven S Skiena)
