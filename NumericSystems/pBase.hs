{-# LANGUAGE MultiWayIf #-}
module PBase where
import Data.List (elemIndex)
import Data.Maybe (fromMaybe)

fromDecimalToP :: Int -> Int -> String
fromDecimalToP _ 0 = error "IllegalBaseError"
fromDecimalToP 0 _ = "0"
fromDecimalToP n p | (abs p) == 1 = error "IllegalBaseError" 
                   | otherwise = map convert $ helper n 0
    where
        convert = (!!) (['0'..'9'] ++ ['a'..'z'])
        helper x i = let base = baseStream !! i
                         modulo = mod x base
                         in if | x == 0 -> []
                               | otherwise -> if | modulo >= 0 -> (helper (div x base) (i + 1)) ++ [modulo]
                                                 | otherwise -> (helper ((div x base) + 1) (i + 1)) ++ [modulo + (abs base)]
            where
                baseStream = [p,p..]

fromPToDecimal :: String -> Int -> Int
fromPToDecimal _ 0 = error "IllegalBaseError"
fromPToDecimal pn p | (abs p) == 1 = error "IllegalBaseError"
                    | otherwise = helper (map convert pn) ((length pn) - 1)
    where
        convert c = fromMaybe (error "RuntimeError") $ elemIndex c (['0'..'9'] ++ ['a'..'z'])
        helper lst i | i == 0 = head lst
                     | otherwise = (head lst) * (foldl (*) 1 (take i baseStream)) + (helper (tail lst) (i - 1))
            where
                baseStream = [p,p..]
