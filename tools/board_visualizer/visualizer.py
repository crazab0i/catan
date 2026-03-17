import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.patches import RegularPolygon
import numpy as np
import re

# ── Parse input file ──────────────────────────────────────────────────────────
tile_types, tile_dice, tile_pts = [], [], []
edge_endpoints = {}

with open("input.txt") as f:
    content = f.read()

for m in re.finditer(
        r'tile id: \d+[^\n]*\ntype: (\w+)\s+dice value: (\d+).*?points: ([\d ,]+)',
        content, re.DOTALL):
    tile_types.append(m.group(1))
    tile_dice.append(int(m.group(2)))
    tile_pts.append([int(p) for p in re.findall(r'\d+', m.group(3))])

for m in re.finditer(r'Edge: (\d+)\na: (\d+) , b: (\d+)', content):
    edge_endpoints[int(m.group(1))] = (int(m.group(2)), int(m.group(3)))

# ── Geometry ──────────────────────────────────────────────────────────────────
ROWS = [3, 4, 5, 4, 3]
R    = 1.0
W    = R * np.sqrt(3)
H    = R * 1.5

hex_colors = {
    "Wood":   "#2d6a2d",
    "Sheep":  "#7ec850",
    "Wheat":  "#f5c842",
    "Brick":  "#c1440e",
    "Ore":    "#8c8c8c",
    "Desert": "#e8d5a3",
}

# Tile centres
centres = []
for row_i, n in enumerate(ROWS):
    x0 = -(n * W) / 2 + W / 2
    y  = (len(ROWS) - 1 - row_i) * H
    for col_i in range(n):
        centres.append((x0 + col_i * W, y))

# Point positions
# Tile point list order: upper-left, top, upper-right, lower-right, bottom, lower-left
# → angle for index k = 150° − k·60°
pt_pos = {}
for ti, (cx, cy) in enumerate(centres):
    for k, pid in enumerate(tile_pts[ti]):
        ang = np.radians(150 - k * 60)
        pt_pos[pid] = (cx + R * np.cos(ang), cy + R * np.sin(ang))

# Edge midpoints
edge_pos = {
    eid: ((pt_pos[a][0] + pt_pos[b][0]) / 2,
          (pt_pos[a][1] + pt_pos[b][1]) / 2)
    for eid, (a, b) in edge_endpoints.items()
    if a in pt_pos and b in pt_pos
}

# ── Plot ──────────────────────────────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(16, 16))

for ti, (cx, cy) in enumerate(centres):
    ax.add_patch(RegularPolygon(
        (cx, cy), numVertices=6, radius=R * 0.95,
        orientation=0,
        facecolor=hex_colors[tile_types[ti]],
        edgecolor='white', linewidth=2,
    ))
    dice     = tile_dice[ti]
    dice_str = "–" if dice == 255 else str(dice)
    dice_col = '#cc0000' if dice in (6, 8) else ('#aaaaaa' if dice == 255 else 'black')
    ax.text(cx, cy + 0.22, tile_types[ti],
            ha='center', va='center', fontsize=7.5, fontweight='bold', color='white')
    ax.text(cx, cy - 0.22, dice_str,
            ha='center', va='center', fontsize=13, fontweight='bold', color=dice_col)

for eid, (mx, my) in edge_pos.items():
    ax.text(mx, my, str(eid),
            ha='center', va='center', fontsize=5.5, color='#00008b',
            bbox=dict(boxstyle='round,pad=0.13', fc='#ddeeff',
                      ec='#00008b', lw=0.5, alpha=0.88))

for pid, (px, py) in pt_pos.items():
    ax.text(px, py, str(pid),
            ha='center', va='center', fontsize=5.5,
            color='white', fontweight='bold', zorder=6,
            bbox=dict(boxstyle='round,pad=0.14', fc='#8b0000',
                      ec='#600000', lw=0.5, alpha=0.92))

legend_patches = [mpatches.Patch(fc=c, ec='grey', label=t)
                  for t, c in hex_colors.items()]
legend_patches += [
    mpatches.Patch(fc='#ddeeff', ec='#00008b', label='Edge ID'),
    mpatches.Patch(fc='#8b0000', ec='#600000', label='Point ID'),
]
ax.legend(handles=legend_patches, loc='lower right', fontsize=9, framealpha=0.9)

ax.autoscale_view()
ax.margins(0.1)
ax.set_aspect('equal')
ax.axis('off')
plt.title('Catan Board – Tile Type · Dice Value · Point IDs · Edge IDs', fontsize=14)
plt.tight_layout()
plt.show()