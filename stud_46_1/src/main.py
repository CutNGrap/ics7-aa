from func import *

def main():
    print("Insert 2 strings:")
    s1 = input()
    s2 = input()
    n = len(s1)
    m = len(s2)

    print("Levenstein answer: ",levenshtein(s1, s2, n, m))
    print("Damerau_Levenstein answer: ",damerau(s1, s2, n, m))
    print("Damerau_Levenstein recursive answer: ",damerau_recursive(s1, s2, n, m))
    print("Damerau_Levenstein recursive cache answer: ",damerau_recursive_cache(s1, s2, n, m))

if __name__ == "__main__":
    main()