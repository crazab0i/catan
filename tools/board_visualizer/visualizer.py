import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import RegularPolygon
import numpy as np

fig, ax = plt.subplots(figsize=(8, 8))

# Hex grid layout: rows of 3, 4, 5, 4, 3
rows = [3, 4, 5, 4, 3]
hex_radius = 1.0

hex_colors = {
    "Wood":   "#2d6a2d",  # dark green
    "Sheep":  "#7ec850",  # light green
    "Wheat":   "#f5c842",  # golden wheat
    "Brick":    "#c1440e",  # terracotta red
    "Ore":"#8c8c8c",  # stone grey
    "Desert":   "#e8d5a3",  # sandy beige
}

colors = ['#e74c3c', '#3498db', '#2ecc71', '#f39c12', '#9b59b6',
          '#1abc9c', '#e67e22', '#34495e', '#e91e63', '#00bcd4',
          '#8bc34a', '#ff5722', '#607d8b', '#795548', '#ffc107',
          '#03a9f4', '#4caf50', '#ff9800', '#9c27b0']

colors = []

with open("input.txt") as file:
    for line in file:
        if line.startswith("type:"):
            hex_type = line[len("type: "):].strip()
            colors.append(hex_colors[hex_type])

# Flat-top hex dimensions
w = hex_radius * np.sqrt(3)   # horizontal spacing
h = hex_radius * 1.5          # vertical spacing

total_rows = len(rows)
hex_index = 0

for row_i, n_hexes in enumerate(rows):
    # Center each row horizontally
    row_width = n_hexes * w
    x_offset = -row_width / 2 + w / 2
    y = (total_rows - 1 - row_i) * h

    for col_i in range(n_hexes):
        x = x_offset + col_i * w
        color = colors[hex_index % len(colors)]

        hex_patch = RegularPolygon(
            (x, y),
            numVertices=6,
            radius=hex_radius * 0.95,   # slight gap between hexes
            orientation=0,              # flat-top
            facecolor=color,
            edgecolor='white',
            linewidth=2
        )
        ax.add_patch(hex_patch)

        # Optional: label each hex
        ax.text(x, y, str(hex_index), ha='center', va='center',
                fontsize=12, fontweight='bold', color='white')

        hex_index += 1

ax.autoscale_view()
ax.margins(0.1)
ax.set_aspect('equal')
ax.axis('off')
plt.title('Catan Board Visualizer', fontsize=16)
plt.tight_layout()
plt.show()