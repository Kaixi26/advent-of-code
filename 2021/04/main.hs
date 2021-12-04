import System.IO

data Bingo = Bingo [[(Int, Bool)]]

instance Show Bingo where
    show (Bingo l) = unlines $ map (unwords . map showCell) $ l
        where
            showCell (x, False) = show x ++ "\t"
            showCell (x, True)  = "x" ++ show x ++ "\t"

splitN :: Int -> [a] -> [[a]]
splitN _ [] = []
splitN n lst = let (l,r) = splitAt n lst in l : splitN n r

mark :: Int -> Bingo -> Bingo
mark n (Bingo b) = Bingo $ map (map (\(x,m) -> (x, m || x == n))) b

won :: Bingo -> Bool
won (Bingo b) = 
    let bb = map (map snd) b
        lbb = length bb - 1
    in any and [[(bb !! i) !! j | i<-[0..lbb]] | j<-[0..lbb]] || any and bb

part2 :: [Int] -> [Bingo] -> (Bingo, Int)
part2 (i:is) bs = case filter (not.won) $ map (mark i) bs of
    []  -> (mark i $ head bs, i)
    nbs -> part2 is nbs

main = do
    (nss:bss) <- (filter (/="") . lines) <$> readFile "input.txt"
    let ns = (map read . words . map (\x -> if x == ',' then ' ' else x) $ nss)::[Int]
    let bs = map (Bingo . map (map (\x -> (read x, False)) . words)) $ splitN 5 bss
    let (Bingo b, i) = part2 ns bs
    print $ (*i) . sum . map (sum . map (\(x,m) -> if m then 0 else x)) $ b
    print (part2 ns bs)


