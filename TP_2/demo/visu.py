import matplotlib.pyplot as plt

t, x, y = [], [], []
with open("../resultats.txt") as f:
    for line in f:
        vals = line.split()
        t.append(float(vals[0]))
        x.append(float(vals[1]))
        y.append(float(vals[2]))

nb_part = 4
noms = ["Soleil", "Terre", "Jupiter", "Halley"]
for i in range(nb_part):
    xi = x[i::nb_part]
    yi = y[i::nb_part]
    plt.plot(xi, yi, label=noms[i])

plt.xlabel("x")
plt.ylabel("y")
plt.title("Trajectoires des particules")
plt.legend()
plt.axis("equal")
plt.grid(True)
plt.savefig("trajectoires.png")
plt.show()