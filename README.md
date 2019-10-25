## Main Logic
### Task 1
I just read the graph. I precompute all the distances and print
the result.
Complexity for precompute: O(numNodes * (numNodes + numEdges))
Complexity for queries: O(numQueries)

### Task 2
I just read queries and print the precomputed distances.
Complexity: O(numQueries)

### Task 3
I read operations and solve the queries with a BFS. After this, I precompute 
all the distances again to update to current state.
Complexity of remove edge: O(numNodes)
Complexity of commands: O(numNodes * numCommands)
Complexity of queries: O(numQueries * (numNodes + numEdges))

### Task 4
For "add" I iterate through all the drivers and I update the driver if found,
or added if not.
For "remove" I iterate through all the drivers and I update the status of the driver.
For "ride" operation I search the nearest available driver and I update his stats.
For "top" operation I print the ordered list for their specific type.
For "info" I iterate through all the drivers and I print his stats if found.
Complexity for add/remove/info/top: O(numDrivers)
Complexity for ride: amortized O(numDrivers) as inaccessible destinations are very rare

### Task 5
I sort the destination by remaining fuel using the ordered linked list.

## Graph
I used a hashmap to map every node of type T an id. And a vector
that maps every id to the node of type T.
I used adjacency lists to store the neighbours.
I used BFS to compute distances.
For the offline queries I precompute all the distances.
For the online queries, at task 3, I call BFS every time.

## HashMap
I implemented a hashmap with open adressing and linear probing, 
because it is faster than separate chaining. I used the std::hash<T>
as the hash function. The capacity is recommended to be power of two
as I used (hashValue & capacity) instead of (hashValue % capacity). 
I initialized the capacity with 2^15 because it is large enough
and provides less chance of collision without being too large.
I implemented with same interface as std::unordered_map using operator [].

## OrderedLinkedList
I kept the rankings for rides, distance and rating ordered in 
ordered linked lists. I also implemented my own iterator for an 
easier interaction.
Complexity for print: O(numDriversToPrint)
Complexity for update: O(numDrivers)

## Driver
I made a structure to keep driver's informations.



