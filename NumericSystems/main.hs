import PBase
import FactorBase
import FibBase

help :: IO ()
help = mapM_ (putStrLn) [
        "fromDecimalToP :: Int (number) -> Int (base) -> String (result)",
        "fromPToDecimal :: String (number) -> Int (base) -> Int (result)",
        "fromDecimalToFactor :: Int (number) -> String (result)",
        "fromFactorToDecimal :: String (number) -> Int (result)",
        "fromDecimalToFib :: Int (number) -> String (result)",
        "fromFibToDecimal :: String (number) -> Int (result)"
    ]

test = 
    (all (&&True) $ map (\pair -> (fst pair) == (flip fromPToDecimal (snd pair) $ uncurry fromDecimalToP pair)) $ zip [0..1000] [1..32]) &&
        (all (&&True) $ map (\n -> n == (fromFactorToDecimal $ fromDecimalToFactor n)) [0..1000]) &&
            (all (&&True) $ map (\n -> n == (fromFibToDecimal $ fromDecimalToFib n)) [3..200])
