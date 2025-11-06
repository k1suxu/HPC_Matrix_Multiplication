import csv
import numpy as np


def check_missing_data(data):
    """
    データに欠損がないかチェックする
    
    Args:
        data: {アルゴリズム名: {'N': [N値のリスト], 'GFLOPS': [GFLOPS値のリスト]}}
    
    Returns:
        bool: 欠損がない場合True、欠損がある場合False
    """
    algs = data.keys()
    n_values = set()
    for alg in algs:
        n_values.update(data[alg]['N'])
    for alg in algs:
        for n in n_values:
            if n not in data[alg]['N']:
                print(f"Missing data for {alg} at N={n}")
                return False
    return True


def extract_data(csv_file, algs=None, exclude_n=None):
    """
    CSVファイルからデータを抽出する
    
    Args:
        csv_file: CSVファイルのパス
        algs: 抽出対象のアルゴリズム名のリスト（Noneの場合はすべて）
        exclude_n: 除外するNの値のリスト（Noneの場合は除外なし）
    
    Returns:
        dict: {アルゴリズム名: {'N': [N値のリスト], 'GFLOPS': [GFLOPS値のリスト]}}
    """
    data = {}
    exclude_n_set = set(exclude_n) if exclude_n else set()
    
    with open(csv_file, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            alg = row['Alg'].strip()
            n = int(row['N'].strip())
            gflops = float(row['GFLOPS'].strip())
            
            # アルゴリズムのフィルタリング
            if algs is not None and alg not in algs:
                continue
            
            # Nのフィルタリング
            if n in exclude_n_set:
                continue
            
            if alg not in data:
                data[alg] = {'N': [], 'GFLOPS': []}
            data[alg]['N'].append(n)
            data[alg]['GFLOPS'].append(gflops)
    
    # データをNでソート
    for alg in data:
        sorted_indices = np.argsort(data[alg]['N'])
        data[alg]['N'] = [data[alg]['N'][i] for i in sorted_indices]
        data[alg]['GFLOPS'] = [data[alg]['GFLOPS'][i] for i in sorted_indices]
    
    if not check_missing_data(data):
        print("Missing data found")
        exit(1)
    
    return data

