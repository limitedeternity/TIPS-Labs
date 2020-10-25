import FibBase

fibEncode :: [Int] -> String
fibEncode xs = foldr (++) "" $ map (\x -> (flip (++)) "1" $ reverse $ fromDecimalToFib x) xs

fibDecode :: String -> [Int]
fibDecode str = map (\x -> fromFibToDecimal $ (++) "1" $ reverse x) $ init $ splitWithStr "11" str
    where
        splitWithStr :: Eq a => [a] -> [a] -> [[a]]
        splitWithStr x y = func x y [[]]
            where
                func x [] z = reverse $ map (reverse) z
                func x (y:ys) (z:zs) = if (take (length x) (y:ys)) == x then
                    func x (drop (length x) (y:ys)) ([]:(z:zs))
                else
                    func x ys ((y:z):zs)

test = (fibDecode $ fibEncode [1,2,3,4,5,6,7,8,9,10]) == [1,2,3,4,5,6,7,8,9,10]
