{-# LANGUAGE MultiWayIf #-}
module FactorBase where
import Data.List (elemIndex)
import Data.Maybe (fromMaybe)


fromDecimalToFactor :: Int -> String
fromDecimalToFactor n = map convert $ helper n 0
    where
        convert = (!!) (['0'..'9'] ++ ['a'..'z'])
        helper x i = let base = baseStream !! i 
                         in if | x < base -> [x]
                               | otherwise -> (helper (div x base) (i + 1)) ++ [mod x base]
            where
                baseStream = [2,3..]
        
fromFactorToDecimal :: String -> Int
fromFactorToDecimal pn = helper (map convert pn) ((length pn) - 1)
    where
        convert c = fromMaybe (error "RuntimeError") $ elemIndex c (['0'..'9'] ++ ['a'..'z'])
        helper lst i | i == 0 = head lst
                     | otherwise = (head lst) * (foldl (*) 1 (take i baseStream)) + (helper (tail lst) (i - 1))
            where
                baseStream = [2,3..]
