import pandas as pd
import os

df_ts = pd.read_csv(os.path.join('..', 'out', 'ts.csv'))
df_aco = pd.read_csv(os.path.join('..', 'out', 'aco.csv'))

df_ts = df_ts.set_index('file_path')
df_aco = df_aco.set_index('file_path')

combined_df = pd.DataFrame(index=df_ts.index)
    
k_columns = ['k', 'k-1', 'k-2', 'k-3']

# Gerar o LaTeX
latex_table = []
latex_table.append(r"\documentclass{article}")
latex_table.append(r"\usepackage[utf8]{inputenc}")
latex_table.append(r"\usepackage{booktabs}")
latex_table.append(r"\usepackage{amsmath}") # Para \text
latex_table.append(r"\usepackage{amssymb}") # Para \text
latex_table.append(r"\usepackage{adjustbox}") # Para ajustar a largura da tabela se necessário
latex_table.append(r"\begin{document}")
latex_table.append(r"")
latex_table.append(r"\begin{table}[htbp]")
latex_table.append(r"    \centering")
latex_table.append(r"    \caption{Resultados dos Algoritmos TS e ACO}")
latex_table.append(r"    \label{tab:ts_aco_results}")
latex_table.append(r"    \adjustbox{max width=\textwidth}{%") # Ajusta a largura da tabela para caber na página
latex_table.append(r"    \begin{tabular}{l" + "rr" * len(k_columns) + "}")
latex_table.append(r"        \toprule")
latex_table.append(r"        \textbf{Instância} & \multicolumn{2}{c}{\textbf{k}} & \multicolumn{2}{c}{\textbf{k-1}} & \multicolumn{2}{c}{\textbf{k-2}} & \multicolumn{2}{c}{\textbf{k-3}} \\")
latex_table.append(r"        \cmidrule(lr){2-3} \cmidrule(lr){4-5} \cmidrule(lr){6-7} \cmidrule(lr){8-9}")
latex_table.append(r"        & \textbf{TS} & \textbf{ACO} & \textbf{TS} & \textbf{ACO} & \textbf{TS} & \textbf{ACO} & \textbf{TS} & \textbf{ACO} \\")
latex_table.append(r"        \midrule")

for instance in df_ts.index:
    row_data = [instance.replace('.col', '')] # Remover .col para nomes mais limpos
    for col in k_columns:
        ts_val = df_ts.loc[instance, col]
        aco_val = df_aco.loc[instance, col]

        # Comparar e aplicar negrito
        if ts_val < aco_val:
            row_data.append("\\textbf{" + str(round(ts_val, 4)) + "}")
            row_data.append("{:.4f}".format(aco_val))
        elif aco_val < ts_val:
            row_data.append("{:.4f}".format(ts_val))
            row_data.append("\\textbf{" + str(round(aco_val, 4)) + "}")
        else: # Empate
            row_data.append("\\textbf{" + str(round(ts_val, 4)) + "}")
            row_data.append("\\textbf{" + str(round(aco_val, 4)) + "}")
    
    latex_table.append("        " + " & ".join(row_data) + r" \\")

latex_table.append(r"        \bottomrule")
latex_table.append(r"    \end{tabular}")
latex_table.append(r"    }") # Fecha adjustbox
latex_table.append(r"\end{table}")
latex_table.append(r"")
latex_table.append(r"\end{document}")

# Salvar o código LaTeX em um arquivo
output_filename = os.path.join('data', "combined_results_table.tex")
with open(output_filename, "w") as f:
    f.write("\n".join(latex_table))

print(f"Tabela LaTeX gerada e salva em '{output_filename}'")