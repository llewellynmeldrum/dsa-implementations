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
make run "ARGS=-v"
# OR 
./bin/test -v
# to run in verbose mode.
# verbose shows each test cases expected and receieved,
# along with a small diff checker between them.
``` 

# Cool stuff
- Implemented optional leetcode testcase style inputs to the binary tree constructor (`BinaryTree<T>(const std::string& s);`)
- Made testing class to group results and pretty print stuff
<img width="500" alt="image" src="https://github.com/user-attachments/assets/6532fc05-7734-4ef3-97c0-ad2f6635eed4" />

> above example showing
> 1. regular, correct test,
> 2. a verbose, correct test, and
> 3. an incorrect test (all incorrect results become verbose output)

# Dependencies 
- gnu make
- c++20 capable compiler (i like std::print)

# References
- ADM (The Algorithm Design Manual by Steven S Skiena)
