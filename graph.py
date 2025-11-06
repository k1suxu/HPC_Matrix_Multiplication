import csv
import sys
import matplotlib.pyplot as plt
import numpy as np
import os
import shutil


def check_missing_data(data):
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


def get_algorithms_with_max(csv_file, exclude_n=None):
    """
    各Nにおける最大GFLOPSを一度でも取ったアルゴリズムの集合を列挙する
    
    Args:
        csv_file: CSVファイルのパス
        exclude_n: 除外するNの値のリスト（Noneの場合は除外なし）
    
    Returns:
        set: 最大値を一度でも取ったアルゴリズム名の集合
    """
    # すべてのデータを抽出
    all_data = extract_data(csv_file, algs=None, exclude_n=exclude_n)
    
    if not all_data:
        return set()
    
    # すべてのNの値を取得
    all_n = set()
    for alg in all_data:
        all_n.update(all_data[alg]['N'])
    
    # 各Nにおける最大GFLOPSを取ったアルゴリズムを記録
    max_algorithms = set()
    
    for n in sorted(all_n):
        max_gflops = -1
        best_algs = []
        
        for alg in all_data:
            if n in all_data[alg]['N']:
                idx = all_data[alg]['N'].index(n)
                gflops = all_data[alg]['GFLOPS'][idx]
                if gflops > max_gflops:
                    max_gflops = gflops
                    best_algs = [alg]
                elif gflops == max_gflops:
                    best_algs.append(alg)
        
        # 最大値を取ったアルゴリズムを追加
        max_algorithms.update(best_algs)
    
    return max_algorithms


def create_graph(csv_file, algs, output, output_dir):
    """
    CSVファイルからデータを読み込み、指定されたアルゴリズムのGFLOPS vs Nのグラフを作成する
    
    Args:
        csv_file: CSVファイルのパス
        algs: プロット対象のアルゴリズム名のリスト
        output: 出力ファイルのパス（.pngなど）
        output_dir: 出力ディレクトリ
    """
    # データを抽出（N=100を除外）
    data = extract_data(csv_file, algs=algs, exclude_n=[100])
    
    # グラフを作成
    plt.figure(figsize=(10, 6))
    
    # 各アルゴリズムをプロット
    for alg in data:
        n_values = data[alg]['N']
        gflops_values = data[alg]['GFLOPS']
        plt.plot(n_values, gflops_values, marker='o', label=alg, linewidth=2, markersize=6)
    
    # 各Nにおける最適点（最高GFLOPS）を見つけてマーカーを付ける
    # すべてのNの値を取得
    all_n = set()
    for alg in data:
        all_n.update(data[alg]['N'])
    
    for n in sorted(all_n):
        max_gflops = -1
        best_alg = None
        
        for alg in data:
            if n in data[alg]['N']:
                idx = data[alg]['N'].index(n)
                gflops = data[alg]['GFLOPS'][idx]
                if gflops > max_gflops:
                    max_gflops = gflops
                    best_alg = alg
        
        if best_alg is not None:
            # 最適点に特別なマーカーを付ける
            n_idx = data[best_alg]['N'].index(n)
            plt.scatter(n, data[best_alg]['GFLOPS'][n_idx], 
                       s=200, marker='*', color='red', 
                       edgecolors='darkred', linewidths=2, zorder=10,
                       label='Best' if n == sorted(all_n)[0] else '')
    
    plt.xlabel('N', fontsize=12)
    plt.ylabel('GFLOPS', fontsize=12)
    plt.title('GFLOPS vs N by Algorithm', fontsize=14)
    if data:  # Only add legend if there's data to plot
        plt.legend(loc='best', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # ファイルに保存
    plt.savefig(output_dir + output, dpi=300, bbox_inches='tight')
    plt.close()
    
    print(f"Graph saved to {output_dir + output}")


if __name__ == "__main__":
    args = sys.argv[1:]

    if len(args) != 2:
        print("Usage: python graph.py <name> <data_file> <output_dir>")
        exit(1)

    # output directory ../PICS/{name}/...
    data_src = args[0]
    output_dir = f"../PICS/{args[1]}/"

    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)

    create_graph(data_src, ['ijk', 'ikj', 'jik', 'jki', 'kij', 'kji'], 'loop-interchange.png', output_dir)
    create_graph(data_src, ['i2kj', 'ik2j', 'ikj2', 'k2ij', 'ki2j', 'kij2'], 'loop-unrolling-cmp-variable.png', output_dir)

    create_graph(data_src, ['ik2j', 'ik4j', 'ik8j', 'ik16j', 'ik10j', 'ik20j'], 'ikj-loop-unrolling-width.png', output_dir)
    create_graph(data_src, ['k2ij', 'k4ij', 'k8ij', 'k16ij', 'k10ij', 'k20ij'], 'kij-loop-unrolling-width.png', output_dir)

    create_graph(data_src, ['ijk_block2', 'ijk_block4', 'ijk_block5', 'ijk_block10', 'ijk_block20', 'ijk_block50'], 'ijk-block-size.png', output_dir)
    create_graph(data_src, ['ikj_block2', 'ikj_block4', 'ikj_block5', 'ikj_block10', 'ikj_block20', 'ikj_block50'], 'ikj-block-size.png', output_dir)

    besties = get_algorithms_with_max(data_src)
    create_graph(data_src, besties, 'besties.png', output_dir)


    data = extract_data(data_src, algs=besties)
    with open(output_dir + 'max_data.csv', 'w') as f:
        f.write('N,Alg\n')
        # Get all N values from any algorithm (they should all have the same N values)
        all_n_values = set()
        for alg in besties:
            all_n_values.update(data[alg]['N'])
        
        max_cnt_alg = {alg: 0 for alg in besties}
        for n in sorted(all_n_values):
            cur_max = -1
            for alg in besties:
                cur_max = max(cur_max, data[alg]['GFLOPS'][data[alg]['N'].index(n)])
            max_algs = []
            for alg in besties:
                if data[alg]['GFLOPS'][data[alg]['N'].index(n)] == cur_max:
                    max_algs.append(alg)
            for alg in max_algs:
                max_cnt_alg[alg] += 1
            f.write(f"{n}, {', '.join(max_algs)}\n")
        print(max_cnt_alg)

    pass

