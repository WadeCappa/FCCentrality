Perform tests with synthetic networks to see how the algorithm scales as the input size increases. 

Use Igraph or some other tool to generate synthetic networks.

Measure the performance of other centrality measures on these same synthetic networks. Compare the quality of their most central vertices between each measure. 

Try to run your algorithm on some real world networks, show that it stops running at a certain size. If this information is bad, at least use table. 

# new plan
Plan: Most of this section is about performance. 

## Experiments
### Real world stress tests, just runtime -> not possible
Run tests for github (or some other network where parallel communication makes sense), see if your faster parallel flow-betweenness ever finishes. Compare this against flow-cost betweenness (which won’t finish), flow closeness, eigenvector centrality, katz index centrality, and the SNA flow betweenness. Record runtime and most central vertices. 

### Synthetic network scaling evaluation for all performance metrics
Then, just do synthetic networks, compare all algorithms on runtime and quality (two separate tables, runtime first). 
After showing runtime, talk about quality between flow betweenness and flow-cost betweenness, is there a justifiable reason for the increased runtime?
If not enough stuff, propose theoretical distributed memory algorithm for flow betweenness. 

### Show granular timings for the real world network -> collected data
To do this LOCK THE PROGRAM TO RUN SEQUENTIALLY

Get granular timings for flow-cost betweenness to discuss why it is so slow compared to flow betweenness, excellent opertuntity to discuss the nature of flow-cost betweenness. 

After getting results and analyzing them, circle back to the rest of the content in the paper. Do this for the synthetic datasets

### quality comparison
Compare the output of all the centrality measures. What is the advantage of one centrality measure over another?