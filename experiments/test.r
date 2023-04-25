# install.packages("sna")
# install.packages("network")

library("sna")
library("network")

# m <- matrix(scan(file=dataset, what=integer(0), skip=0, sep=' '), ncol=3, byrow=TRUE)
# m[,c(1,2)] <- m[,c(2,1)]
# e <- m[,c(1,2)]
# net <- network(m, matrix.type="edgelist", directed=FALSE, multiple=TRUE)
# n <- network(dataset, matrix.type="edgelist", directed=TRUE, multiple=TRUE)

GetN = function(filepath) {
    n = 0
    con = file(filepath, "r")
    while ( TRUE ) {
        line = readLines(con, n = 1)
        if ( length(line) == 0 ) {
            break
        }
        vals <- strsplit(line, split = " ")
        for (i in 1:length(vals)) {
            vals[[i]] <- strtoi(vals[[i]], base = 0L)
        }

        if (length(vals[[1]]) > 1 && vals[[1]][1] > n) {n = vals[[1]][1]}
        if (length(vals[[1]]) > 2 && vals[[1]][2] > n) {n = vals[[1]][2]}
    }

    close(con)
    return(n)
}

BuildMatrix = function(filepath, n, undirected=TRUE, weighted=TRUE) {
    ADJ <- matrix(0,n,n)
    con = file(filepath, "r")
    while ( TRUE ) {
        line = readLines(con, n = 1)
        if ( length(line) == 0 ) {
            break
        }
        vals <- strsplit(line, split = " ")
        for (i in 1:length(vals)) {
            vals[[i]] <- strtoi(vals[[i]], base = 0L)
        }

        weight = 1
        if (weighted == TRUE)
        {
            weight = vals[[1]][3]
        }

        ADJ[vals[[1]][1],vals[[1]][2]] <- weight
        if (undirected == TRUE)
        {
            ADJ[vals[[1]][2],vals[[1]][1]] <- weight
        }
    }

    close(con)
    return (ADJ)
}

test_network <- "/home/cappa/Documents/Data/school/Semester8/CPTS_591/project/fc_centrality/networks/toy_undirected.txt"
n <- GetN(test_network)
matrix <- BuildMatrix(test_network, n)
e <- flowbet(matrix, gmode="graph")
print(e)

karate <- "/home/cappa/Documents/Data/school/Semester8/CPTS_591/project/fc_centrality/networks/karate_undirected.txt"
n <- GetN(karate)
matrix <- BuildMatrix(karate, n, weighted=FALSE)
e <- flowbet(matrix, gmode="graph")
print(e)

facebook <- "/home/cappa/Documents/Data/school/Semester8/CPTS_591/project/fc_centrality/networks/facebook_combined.txt"
n <- GetN(facebook)
matrix <- BuildMatrix(facebook, n, weighted=FALSE)
e <- flowbet(matrix, gmode="graph")
print(e)