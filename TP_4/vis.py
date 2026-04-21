import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import glob

fichiers = sorted(glob.glob("frames/frame_*.csv"), key=lambda x: int(x.split('_')[1].split('.')[0]))

if len(fichiers) == 0:
    print("Erreur : Aucun fichier trouvé. Relance le ./Simulation !")
    exit()

fig, ax = plt.subplots(figsize=(10, 5), facecolor='white')


ax.set_aspect('equal')
ax.set_xlim(0, 250)
ax.set_ylim(0, 120)

ax.axis('off') 

scat_carre = ax.scatter([], [], c='#CC0000', s=4, marker='s', label='Carré')
scat_rect  = ax.scatter([], [], c='#0000AA', s=4, marker='s', label='Rectangle')

def update(i):
    df = pd.read_csv(fichiers[i])
    carre = df[df['objet'] == 'carre']
    rect  = df[df['objet'] == 'rectangle']
    scat_carre.set_offsets(carre[['x', 'y']].values)
    scat_rect.set_offsets(rect[['x', 'y']].values)
    return scat_carre, scat_rect

ani = animation.FuncAnimation(fig, update, frames=len(fichiers), blit=True)

print(f"Création de la vidéo à partir de {len(fichiers)} frames...")
ani.save('collision.mp4', writer='ffmpeg', fps=10)
print("Fait ! Ouvre collision.mp4")