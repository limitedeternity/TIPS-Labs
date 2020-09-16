import PBase
import FactorBase
import FibBase
import Data.List (tails)

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
    (all (&&True) $ map (\pair -> (fst pair) == (flip fromPToDecimal (snd pair) $ uncurry fromDecimalToP pair)) $ [(x, y) | (x:ys) <- tails [-200..200], y <- [(-16)..(-2)]]) &&
    (all (&&True) $ map (\pair -> (fst pair) == (flip fromPToDecimal (snd pair) $ uncurry fromDecimalToP pair)) $ [(x, y) | (x:ys) <- tails [0..200], y <- [2..16]]) &&
    (all (&&True) $ map (\n -> n == (fromFactorToDecimal $ fromDecimalToFactor n)) [0..10000]) &&
    (all (&&True) $ map (\n -> n == (fromFibToDecimal $ fromDecimalToFib n)) [3..10000])
