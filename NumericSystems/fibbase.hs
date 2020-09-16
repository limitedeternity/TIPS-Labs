module FibBase where
import Data.List (isInfixOf)

fromDecimalToFib :: Int -> String
fromDecimalToFib num = assemble $ traceSum num
    where
        traceSum n | n == 0 = []
                   | otherwise = let takenFib = last fibRange in ([takenFib] ++ traceSum (n - takenFib))
                         where 
                             fibRange = let fibs = 0 : 1 : zipWith (+) fibs (tail fibs) in (drop 2 $ takeWhile (<=n) fibs)

        assemble trace = map (\x -> if x `elem` trace then '1' else '0') $ reverse fibRange
            where
                fibRange = let fibs = 0 : 1 : zipWith (+) fibs (tail fibs) in (drop 2 $ takeWhile (<=num) fibs)


fromFibToDecimal :: String -> Int
fromFibToDecimal pn | "11" `isInfixOf` pn = error "FibIncorrectError"
                    | otherwise = sum $ map (\p -> fibRange !! (fst p)) $ filter (\p -> (snd p) == '1') $ zip [0..] $ reverse pn
    where
        fibRange = let fibs = 0 : 1 : zipWith (+) fibs (tail fibs) in (drop 2 $ take ((length pn) + 2) fibs)
