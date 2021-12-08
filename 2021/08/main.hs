import Data.Map (Map)
import qualified Data.Map as M
import Data.Set (Set)
import qualified Data.Set as S
import System.IO
import Data.Maybe
import Debug.Trace

isSubsetof :: Ord a => Set a -> Set a -> Bool
isSubsetof s s' = s `S.intersection` s' == s

fst_nums :: [String] -> Map Int (Set Char)
fst_nums [] = M.empty
fst_nums (l:ls) = 
    let m = fst_nums ls 
        lset = S.fromList l
    in case length l of
        2 -> M.insert 1 lset m
        3 -> M.insert 7 lset m
        4 -> M.insert 4 lset m
        7 -> M.insert 8 lset m
        _ -> m

snd_nums :: Map Int (Set Char) -> [String] -> Map Int (Set Char)
snd_nums m [] = m
snd_nums m (l:ls) = 
    let m' = snd_nums m ls 
        lset = S.fromList l
    in case length l of
        6 -> 
            if (fromJust $ M.lookup 4 m) `S.isSubsetOf` lset
                then M.insert 9 lset m'
            else if (fromJust $ M.lookup 7 m) `S.isSubsetOf` lset
                then M.insert 0 lset m'
            else M.insert 6 lset m'
        _ -> m'

trd_nums :: Map Int (Set Char) -> [String] -> Map Int (Set Char)
trd_nums m [] = m
trd_nums m (l:ls) = 
    let m' = trd_nums m ls 
        lset = S.fromList l
    in case length l of
        5 -> 
            if lset `S.isSubsetOf` (fromJust $ M.lookup 6 m) 
                then M.insert 5 lset m'
            else if lset `S.isSubsetOf` (fromJust $ M.lookup 9 m) 
                then M.insert 3 lset m'
            else M.insert 2 lset m'
        _ -> m'

compute_line l =
    let wds = words l
        ns = take 10 wds
        ns' = map S.fromList $ drop 11 wds
        magic =  map (\(k,v) -> (v,k)) $ M.toList $ trd_nums(snd_nums (fst_nums ns) ns) ns
        nums = map (fromJust . flip lookup magic) ns'
    in sum $ zipWith (*) [1000,100,10,1] nums

main = do
    input <- lines <$> readFile "input.txt"
    print $ sum $ map compute_line $ input
