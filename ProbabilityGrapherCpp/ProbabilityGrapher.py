
import argparse
import matplotlib.pyplot as plt
import subprocess
import re

EXPR_PATTERN = re.compile(r'^(\d+):(\d+)$', re.MULTILINE)

def main():
    parser = argparse.ArgumentParser(prog="Probability Grapher", description="Gives the likelyhood of each possible result for a set of dice")
    parser.add_argument("expr", nargs='+', help="An expression in the ndN where n is the number of dice and N is their max value. Several expressions can be specified, for many different dice types.")
    args = parser.parse_args()
    exprs = args.expr 
    sumVsOccurences = callEngine(exprs)
    show(sumVsOccurences, exprs)
    
    
def callEngine(exprs: list) -> dict:
    """ Calls ProbabilityGrapherEngine to do additions and count occurences
    Args:
        exprs(list): ex: ['5d4', '2d6']
    Returns:
        A dict of value vs total occurences
    """
    args = ['./ProbabilityGrapherEngine']
    args.extend(exprs)
    completed = subprocess.run(args, capture_output=True, check=True, text=True)
    results = parseEngineResult(completed.stdout)
    print(results)
    return results
    
def parseEngineResult(result:str) -> dict:
    """ Take a ProbabilityGrapherEngine result in string form, and convert it to a dict 
    """
    parsed = dict()
    matchIter = EXPR_PATTERN.finditer(result)
    for matcher in matchIter:        
        val = int(matcher.group(1))
        occurences = int(matcher.group(2))
        parsed[val] = occurences
        
    return parsed

def show(catalogue:dict, exprs:list) -> None:
    """ Plot a graph of value vs occurences
    """
    x = catalogue.keys()
    y = catalogue.values()
    plt.bar(x, y)
    plt.xlim(min(x), max(x))
    plt.xlabel('Result')
    plt.ylabel('Occurences')
    
    formatted = '+'.join(exprs)
    
    plt.title("Dice Value Probabilities for '" + formatted + "'")
    plt.show()
    
    
if __name__ == '__main__':
    main()