import sys
import numpy as np
from data_extractor import extract_data

def create_table(csv_files, ns):
    """
    CSVファイルからデータを読み込み、アルゴリズム名とGFLOPSの平均値のLaTeX表を生成する
    
    Args:
        csv_files: CSVファイルのパスのリスト
        ns: 使うNの値のリスト
    
    Returns:
        str: LaTeX形式の表
    """
    
    print('  \\hline')
    print("  Alg", end='')
    for n in ns:
        print(f" & {n}", end='')
    print(' \\\\')
    print('  \\hline')
    
    # データを抽出
    data = {}
    for csv_file in csv_files:  
        data.update(extract_data(csv_file))
    
    for alg in sorted(data.keys()):
        data_n = []
        for n in ns:
            if n not in data[alg]['N']:
                print(f"Missing data for {alg} at N={n}")
                exit(1)
            data_n.append(data[alg]['GFLOPS'][data[alg]['N'].index(n)])
        print(f"  {alg}", end='')
        for n in ns:
            print(f" & {float(data_n[ns.index(n)]):.2f}", end='')
        print(' \\\\')
        print('  \\hline')

if __name__ == "__main__":
    args = sys.argv[1:]
    
    if len(args) < 1:
        print("Usage: python table.py [csv_files...]")
        exit(1)
    
    csv_files = args
    
    # 表を生成（N=100を除外）
    create_table(csv_files, [500, 1000, 1500, 2000, 2500])
