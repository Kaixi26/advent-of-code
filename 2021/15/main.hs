import Data.Char
import Data.Array
import Data.Set (Set)
import qualified Data.Set as S
import Data.Map (Map)
import qualified Data.Map as M
import Debug.Trace
import Data.List
import Data.Tuple

type MinQueue a = [a]

push :: Ord a => a -> MinQueue a -> MinQueue a
push x [] = [x]
push x (h:t)
    | x < h = x:h:t
    | otherwise = h:push x t

pop :: Ord a => MinQueue a -> (a, MinQueue a)
pop (h:t) = (h, t)

data State = State 
    { _values :: !(Array Int (Array Int Int))
    , _visited :: !(Map (Int, Int) Int)
    , _queue :: !(MinQueue (Int, (Int, Int)))
    , _size :: !Int 
    } deriving Show

step :: State -> State
step st@State{_queue = queue, _visited = visited} = 
    let ((risk, coords@(x,y)), queue') = pop queue
        visited' = M.insert coords risk visited
        queue'' = filter (\(_,c) -> c /= coords) queue'
        pushIf cs q = if M.member cs visited then q else push (risk + riskLevel st cs, cs) q
        queue''' = pushIf (x,y+1) $ pushIf (x+1,y) queue''
    in st{_queue = queue''', _visited = visited'}

riskLevel :: State -> (Int, Int) -> Int
riskLevel State{_values = vs, _size = sz} (x, y) =
    let (xd, xm) = x `divMod` sz
        (yd, ym) = y `divMod` sz
    in (((vs!ym)!xm)+xd*sz+yd*sz-1) `mod` 9 + 1

findt :: State -> (Int, Int) -> Int
findt st@State{_visited = visited} target =
    case M.lookup target visited of
        Just r ->
            r
        Nothing ->
            findt (step st) target

main = do
    input <-  map (map digitToInt) . lines <$> readFile "example.txt"
    let size = length input
    let vs = listArray (0,size-1) $ map (listArray (0, size-1)) input
    let st = State{_values = vs, _visited = M.empty, _size = size, _queue = [(0,(0,0))]}
    let board = unlines [[intToDigit $ riskLevel st (x,y) | x<-[0..(size*5-1)]] | y<-[0..(size*5-1)]]
    let p1 = findt st (size-1,size-1)
    let p2 = findt st (size*5-1,size*5-1)
    putStrLn $ "Part 1: " ++ show p1
    putStrLn $ "Part 2: " ++ show p2
