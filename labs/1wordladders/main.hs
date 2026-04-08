
import qualified Data.Map.Strict as Map
import qualified Data.Set        as Set
import qualified Data.Sequence   as Seq
import           Data.Sequence   (Seq(..), (|>))
import           Data.Maybe      (fromMaybe)
import           Control.Monad (replicateM)


-- Graph for the problem, keyvalue pairs that maps a key(node) to
-- neighbors(A set of strings)
type Graph = Map.Map String (Set.Set String)

--main loop for the program
main :: IO ()
main = do
    (first, second) <- readFirstLine
    nodes <- readInput first
    let graph = buildGraph nodes     
    connections <- readInput second
    mapM_ (\q -> putStrLn (check graph q)) connections

check :: Graph -> String -> String
check graph s = 
    let wordList = words s 
    in 
        bfs (wordList !! 0) (wordList !! 1) graph 

-- Input for the program 

-- Input for the First Line, Getting the 2 numbers
readFirstLine :: IO (Int, Int)
readFirstLine = do 
    firstLine <- getLine
    let parts = words firstLine
        [a, b] = map read parts :: [Int]
    return (a, b)

-- Input for n number of lines
readInput :: Int -> IO [String]
readInput n = do
    replicateM n getLine


-- Checks if we have an edge from one node to another
hasEdge :: String -> String -> Bool
hasEdge w1 w2 =
  let firstFourLettersOfW1 = drop 1 w1
  in  all (\c -> charCount c firstFourLettersOfW1 <= charCount c w2) firstFourLettersOfW1

charCount :: Char -> String -> Int
charCount c = length . filter (== c)


-- BUilds the graph 
buildGraph :: [String] -> Graph
buildGraph nodes =
  Map.fromListWith Set.union
    [ (x, Set.singleton y)
    | x <- nodes
    , y <- nodes
    , x /= y
    , hasEdge x y ]


bfs :: String -> String -> Graph -> String
bfs start end graph 
    | start == end = "0"
    | otherwise    = go (Seq.singleton (start, 0)) (Set.singleton start)
    where 
        go Empty _ = "Impossible"
        go ((node, depth) :<| rest) visited 
            | end `Set.member` newNeighbours = show (depth + 1)
            | otherwise = go newQueue newVisited 
            where 
                allNeighbours = Map.findWithDefault Set.empty node graph
                newNeighbours = Set.difference allNeighbours visited
                newVisited    = Set.union visited newNeighbours
                newQueue      = foldl (|>) rest [(n, depth + 1) | n <- Set.toList newNeighbours]

                 