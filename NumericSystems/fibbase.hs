module FibBase where
import Data.List (tails, nub, elemIndex)
import Data.Maybe (fromJust, isNothing)

fromDecimalToFib :: Int -> String
fromDecimalToFib n = assemble $ traceSum $ combsBy2 fibRange 
    where 
        fibRange = let fibs = 0 : 1 : zipWith (+) fibs (tail fibs) in (drop 2 $ takeWhile (<=n) fibs)
        combsBy2 xs = [[x, y] | (x:ys) <- tails (nub xs), y <- ys]
        traceSum combs | not (isNothing targetIndex) = combs !! (fromJust targetIndex)
                       | otherwise = traceSum [x:xs | x <- fibRange, xs <- combs, not (x `elem` xs)]
                          where
                              sumOfCombs = map sum combs
                              targetIndex = elemIndex n sumOfCombs
        assemble trace = map (\x -> if x `elem` trace then '1' else '0') $ reverse fibRange

fromFibToDecimal :: String -> Int
fromFibToDecimal pn = sum $ map (\p -> fibRange !! (fst p)) $ filter (\p -> (snd p) == '1') $ zip [0..] $ reverse pn
    where
        fibRange = let fibs = 0 : 1 : zipWith (+) fibs (tail fibs) in (drop 2 $ take ((length pn) + 2) fibs)
