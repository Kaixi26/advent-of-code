import Data.List
import Data.Set (Set)
import qualified Data.Set as S
import Data.Map (Map)
import qualified Data.Map.Strict as M
import Data.Maybe
import Data.Char
import Data.Tuple
import Debug.Trace

newtype Graph a = Graph 
    { verts :: Map a [a]
    } deriving Show

empty :: Graph a
empty = Graph{ verts = M.empty }

insertVert :: Ord a => (a,a) -> Graph a -> Graph a
insertVert (s,e) Graph{ verts = vs } =
    let v = (e:) $ fromMaybe [] $ M.lookup s vs
    in Graph{ verts = M.insert s v vs }

fromList :: Ord a => [(a,a)] -> Graph a
fromList = foldr insertVert empty

pathsP1 :: Graph String -> Set String -> String -> String -> [[String]]
pathsP1 graph visited start end
    | start == end = [[start]]
    | start `S.member` visited = []
    | otherwise = do
        adj <- fromMaybe [] $ M.lookup start $ verts graph
        let visited' = if all isUpper start
            then visited
            else S.insert start visited
        (start:) <$> pathsP1 graph visited' adj end

pathsP2 :: Graph String -> Map String Int -> String -> String -> [[String]]
pathsP2 graph visited start end
    | start == end = [[end]]
    | otherwise = do
        adj <- fromMaybe [] $ M.lookup start $ verts graph
        let visited' = M.alter (Just . fromMaybe 1 . fmap (+1)) adj visited
        if  all isUpper adj
        then (start:) <$> pathsP2 graph visited adj end
        else if adj == "start"
        then []
        else if (\l -> l == [] || l == [2]) $ filter (>=2) $ M.elems visited'
        then (start:) <$> pathsP2 graph visited' adj end
        else []

main = do
    input <- lines <$> readFile "input.txt"
    let vs = (fmap tail) <$> span (/='-') <$> input
    let g = fromList $ vs ++ (map swap vs)
    let ps1 = pathsP1 g S.empty "start" "end"
    let ps2 = pathsP2 g (M.insert "start" 1 M.empty) "start" "end"
    putStrLn $ concat ["Part 1: ", show (length ps1)]
    --print g
    --print ps2
    putStrLn $ concat ["Part 2: ", show (length ps2)]
